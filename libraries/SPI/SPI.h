/*
  This library is free software; you can redistribute it and/or
  modify it under the terms of the GNU Lesser General Public
  License as published by the Free Software Foundation; either
  version 2.1 of the License, or (at your option) any later version.
  This library is distributed in the hope that it will be useful,
  but WITHOUT ANY WARRANTY; without even the implied warranty of
  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
  Lesser General Public License for more details.
  You should have received a copy of the GNU Lesser General Public
  License along with this library; if not, write to the Free Software
  Foundation, Inc., 51 Franklin St, Fifth Floor, Boston, MA  02110-1301  USA   

    Author: Georgi Angelov  
 */

#ifndef __SPI_CLASS_H__
#define __SPI_CLASS_H__

#ifdef __cplusplus

#include "interface.h"
#include "constants.h"
#include "Stream.h"
#include "variant.h"
#include "RingBuffer.h"
#include "hardware/spi.h"

typedef enum
{
    SPI_MODE0 = 0, // CPOL : 0  | CPHA : 0
    SPI_MODE1,     // CPOL : 0  | CPHA : 1
    SPI_MODE2,     // CPOL : 1  | CPHA : 0
    SPI_MODE3      // CPOL : 1  | CPHA : 1
} SPIDataMode;

class SPISettings
{
public:
    SPISettings(uint32_t clockFrequency, uint8_t bitOrder, uint8_t dataMode)
    {
        clock = clockFrequency;
        order = bitOrder;
        mode = dataMode;
    }

    SPISettings()
    {
        clock = 1000; // kHz
        order = MSBFIRST;
        mode = SPI_MODE0;
    }

private:
    uint16_t clock;
    uint8_t order;
    uint8_t mode;
    friend class SPIClass;
};

class SPIClass
{
private:
    spi_inst_t *ctx;
    int _sck, _miso, _mosi, _ss;
    int _clk_polarity;
    int _clk_format;
    int _bit_order;
    int _data_bits;
    uint32_t _speed;

public:
    SPIClass(uint8_t spi)
    {
        spi ? ctx = spi1 : spi0;    // spi0, has two identical SPI
        _clk_polarity = 0;          // cpol
        _clk_format = 0;            // cpha
        _bit_order = SPI_LSB_FIRST; // LSBFIRST = 0
        _data_bits = 8;             // 4..16
        _speed = 1000000;
    }

    SPIClass(spi_inst_t *port)
    {
        ctx = port;                 // spi0, has two identical SPI
        _clk_polarity = 0;          // cpol
        _clk_format = 0;            // cpha
        _bit_order = SPI_LSB_FIRST; // LSBFIRST = 0
        _data_bits = 8;             // 4..16
        _speed = 1000000;
    }

    void begin(int8_t sck = -1, int8_t miso = -1, int8_t mosi = -1, int8_t ss = -1)
    {
        if (sck == -1 && miso == -1 && mosi == -1 && ss == -1)
        {
            return; // SELECT PINS ?
        }
        else
        {
            _sck = sck;
            _miso = miso;
            _mosi = mosi;
            _ss = ss;
        }
        gpio_set_function(_miso, GPIO_FUNC_SPI);
        gpio_set_function(_mosi, GPIO_FUNC_SPI);
        gpio_set_function(_sck, GPIO_FUNC_SPI);
        if (_ss > -1)
        {
            gpio_init(ss);
            gpio_set_dir(ss, GPIO_OUT);
            gpio_put(ss, 1);
        }
        spi_init(ctx, _speed); // settings.clock
    }

    void beginTransaction(SPISettings settings)
    {
        setFrequency(settings.clock);
        setDataMode(settings.mode);
        setBitOrder(settings.order);
        spi_set_format(ctx, _data_bits,
                       (spi_cpol_t)_clk_polarity,
                       (spi_cpha_t)_clk_format,
                       (spi_order_t)_bit_order);
    }

    void end()
    {
        if (_ss > -1)
            gpio_set_function(_ss, GPIO_FUNC_XIP);
        gpio_set_function(_sck, GPIO_FUNC_XIP);
        gpio_set_function(_miso, GPIO_FUNC_XIP);
        gpio_set_function(_mosi, GPIO_FUNC_XIP);
        _miso = -1;
        _mosi = -1;
        _sck = -1;
        _ss = -1;
        spi_deinit(ctx);
    }

    uint8_t transfer(uint8_t data)
    {
        if (_ss > -1)
        {
            asm volatile("nop \n nop \n nop");
            gpio_put(_ss, 0);
            asm volatile("nop \n nop \n nop");
        }
        uint8_t rx = 0;
        spi_write_read_blocking(ctx, &data, &rx, 1); // Returns: Number of bytes written/read
        if (_ss > -1)
        {
            asm volatile("nop \n nop \n nop");
            gpio_put(_ss, 1);
            asm volatile("nop \n nop \n nop");
        }
        return rx; // Returns the received data
    }

    uint16_t transfer16(uint16_t data)
    {
        union
        {
            uint16_t val;
            struct
            {
                uint8_t lsb;
                uint8_t msb;
            };
        } t;
        t.val = data;
        if (_bit_order == SPI_LSB_FIRST)
        {
            t.lsb = transfer(t.lsb);
            t.msb = transfer(t.msb);
        }
        else
        {
            t.msb = transfer(t.msb);
            t.lsb = transfer(t.lsb);
        }
        return t.val;
    }

    int transfer(uint8_t *buf, size_t count)
    {
        return spi_write_read_blocking(ctx, buf, buf, count);
    }

    void write(uint8_t data) { transfer(data); }

    void write16(uint16_t data) { transfer16(data); }

    void write(uint8_t *buf, size_t count) { transfer(buf, count); }

    void setDataMode(uint8_t mode)
    {
        switch (mode)
        {
        case SPI_MODE1:
            _clk_polarity = SPI_CPOL_0;
            _clk_format = SPI_CPHA_1;
            break;
        case SPI_MODE2:
            _clk_polarity = SPI_CPOL_1;
            _clk_format = SPI_CPHA_0;
            break;
        case SPI_MODE3:
            _clk_polarity = SPI_CPOL_1;
            _clk_format = SPI_CPHA_1;
            break;
        default:
            _clk_polarity = SPI_CPOL_0;
            _clk_format = SPI_CPHA_0;
            break;
        }
    }

    void setFrequency(uint16_t kHz)
    {
        _speed = kHz * 1000;
        spi_set_baudrate(ctx, _speed);
    }

    void setBitOrder(uint8_t order) { _bit_order = order; }

    uint32_t getClockDivider() { return 0; }
    void setClockDivider(uint8_t div) {}

    void endTransaction(void) {}
    void usingInterrupt(int interruptNumber) {}
    void notUsingInterrupt(int interruptNumber) {}
    void attachInterrupt(){};
    void detachInterrupt(){};
};

extern SPIClass SPI;

#endif // __cplusplus
#endif //__SPI_CLASS_H__