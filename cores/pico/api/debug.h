////////////////////////////////////////////////////////////////////////////////////////
//
// Copyright 2020 Georgi Angelov ver 1.0
//
// Licensed under the Apache License, Version 2.0 (the "License");
// you may not use this file except in compliance with the License.
// You may obtain a copy of the License at
//
//     http://www.apache.org/licenses/LICENSE-2.0
//
// Unless required by applicable law or agreed to in writing, software
// distributed under the License is distributed on an "AS IS" BASIS,
// WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
// See the License for the specific language governing permissions and
// limitations under the License.
//
////////////////////////////////////////////////////////////////////////////////////////

#ifndef _DBG_H_
#define _DBG_H_
#ifdef __cplusplus
extern "C"
{
#endif

#include <stdarg.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include "hardware/uart.h"
#include "hardware/gpio.h"
#include "hardware/irq.h"

#define DBG_UART PICO_DEFAULT_UART_INSTANCE
#define DBG_BUFFER_SIZE 256
    extern char DBG_BUFFER[DBG_BUFFER_SIZE];

    extern bool DBG_IS_OPEN;
#define DBG(FRM, ...) /* Serial must be open */                                         \
    if (DBG_IS_OPEN)                                                                    \
    {                                                                                   \
        uint32_t _prim = save_and_disable_interrupts();                                 \
        sprintf(DBG_BUFFER, FRM, ##__VA_ARGS__);                                        \
        uart_write_blocking(DBG_UART, (const uint8_t *)DBG_BUFFER, strlen(DBG_BUFFER)); \
        restore_interrupts(_prim);                                                      \
    }

#define DBG_INIT()                                         \
    {                                                      \
        gpio_set_function(0, 2);                           \
        gpio_set_function(1, 2);                           \
        uart_init(DBG_UART, 115200);                       \
        uart_set_hw_flow(DBG_UART, false, false);          \
        uart_set_format(DBG_UART, 8, 1, UART_PARITY_NONE); \
        uart_set_fifo_enabled(DBG_UART, false);            \
        DBG_IS_OPEN = true;                                \
        memset(DBG_BUFFER, 0, DBG_BUFFER_SIZE);            \
        DBG("[SYS] DEBUG MODE\n");                         \
    }

    typedef struct
    {
        void *ctx;
        int (*write_r)(struct _reent *r, _PTR ctx, const char *buf, int len);
        int (*read_r)(struct _reent *r, _PTR ctx, char *buf, int len);
    } drv_t;

    extern drv_t stdio_drv;

    void dbg_retarget(void *p);

#ifdef __cplusplus
}
#endif
#endif