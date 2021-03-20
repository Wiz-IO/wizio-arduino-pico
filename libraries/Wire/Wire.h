/*
    2021 Georgi Angelov

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
 */

#ifndef __WIRE_H__
#define __WIRE_H__
#ifdef __cplusplus

#include "interface.h"
#include "Stream.h"
#include "variant.h"
#include "RingBuffer.h"
#include "hardware/i2c.h"

#define WIRE_PRINT // Serial.printf

class TwoWire : public Stream
{
private:
    int _sda, _scl, _speed;
    uint32_t _timeout_us;
    bool transmissionBegun;
    uint8_t _slave_address;
    i2c_inst_t *ctx;
    RingBuffer rx, tx;

public:
    TwoWire(i2c_inst_t *contex, uint32_t speed_Hz = 100000)
    {
        ctx = contex;
        _speed = speed_Hz;
        _timeout_us = 100 * 1000;
        _sda = PICO_DEFAULT_I2C_SDA_PIN;
        _scl = PICO_DEFAULT_I2C_SCL_PIN;
        transmissionBegun = false;
    }

    ~TwoWire() { end(); }

    void pins(int SDA, int SCL, bool enablePullup = true)
    {
        _sda = SDA;
        _scl = SCL;
        gpio_set_function(SDA, GPIO_FUNC_I2C);
        gpio_set_function(SCL, GPIO_FUNC_I2C);
        if (enablePullup)
        {
            gpio_pull_up(SDA);
            gpio_pull_up(SCL);
        }
        //WIRE_PRINT("[%s] %d, %d\n", __func__, sdaPin, sclPin);
    }

    void setTimeOut(uint32_t timeout_millis) { _timeout_us = timeout_millis * 1000; }

    void setClock(uint32_t speed_Hz)
    {
        i2c_set_baudrate(ctx, speed_Hz);
    }

    void xxxx(int SDA, int SCL, uint32_t speed_Hz)
    {
        pins(SDA, SCL);
        i2c_init(ctx, speed_Hz);
    }

    void begin(int SDA, int SCL, uint8_t address)
    {
        _slave_address = address;
        pins(SDA, SCL);
        i2c_init(ctx, _speed);
    }

    void begin(void) { begin(_sda, _scl, _slave_address); }

    void end()
    {
        gpio_set_function(_sda, GPIO_FUNC_XIP);
        gpio_set_function(_scl, GPIO_FUNC_XIP);
        i2c_deinit(ctx);
    }

    void beginTransmission(uint8_t);
    uint8_t endTransmission(bool stopBit);
    uint8_t endTransmission(void);

    uint8_t requestFrom(uint8_t address, size_t size, bool stopBit);
    uint8_t requestFrom(uint8_t address, size_t size);

    size_t write(uint8_t data);
    size_t write(const uint8_t *data, size_t size);

    virtual int available(void) { return rx.available(); }
    virtual int read(void) { return rx.read_char(); }
    virtual int peek(void) { return rx.peek(); }
    virtual void flush(void) {}

    using Print::write;

    void onService(void){};
    void onReceive(void (*)(int)){};
    void onRequest(void (*)(void)){};
};

extern TwoWire Wire;
extern TwoWire Wire1;

#endif // cpp
#endif // h