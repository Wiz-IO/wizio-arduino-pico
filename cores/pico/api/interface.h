////////////////////////////////////////////////////////////////////////////////////////
//
// Copyright 2021 Georgi Angelov
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

#ifndef INTERFACE_H_
#define INTERFACE_H_
#ifdef __cplusplus
extern "C"
{
#endif

#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>
#include <stdarg.h>
#include <string.h>
#include <errno.h>
#include <assert.h>
#include <limits.h>
#include <reent.h>
#include <unistd.h>
#include <fcntl.h>
#include <ctype.h>
#include <math.h>
#include <time.h>

#include "pico/config.h"
#include "pico/stdlib.h"
#include "pico/divider.h"
#include "pico/bootrom.h"
#include "pico/mutex.h"
#include "pico/multicore.h"
#include "hardware/flash.h"
#include "hardware/adc.h"
#include "hardware/pwm.h"
#include "hardware/gpio.h"
#include "hardware/uart.h"
#include "hardware/i2c.h"
#include "hardware/spi.h"
#include "hardware/rtc.h"
#include "hardware/divider.h"
#include "hardware/clocks.h"
#include "hardware/sync.h"
#include <debug.h>

#ifdef USE_FREERTOS

#include <FreeRTOS.h>
#include <task.h>
#include <semphr.h>
#include <timers.h>
#define MUTEX_PTYPE (SemaphoreHandle_t)
#define MUTEX_INIT(pM) pM = xSemaphoreCreateMutex()
#define MUTEX_LOCK(pM) xSemaphoreTake(MUTEX_PTYPE pM, portMAX_DELAY)
#define MUTEX_UNLOCK(pM) xSemaphoreGive(MUTEX_PTYPE pM)

#else // pico-sdk

#define MUTEX_PTYPE (mutex_t *)
#define MUTEX_INIT(pM)                           \
    pM = MUTEX_PTYPE calloc(1, sizeof(mutex_t)); \
    mutex_init(pM)
#define MUTEX_LOCK(pM) mutex_enter_blocking(MUTEX_PTYPE pM)
#define MUTEX_UNLOCK(pM) mutex_exit(MUTEX_PTYPE pM)

#endif // USE_FREERTOS

/*** preinit_array function() helper **************
#pragma GCC push_options
#pragma GCC optimize("-O0")
static void pre_lock_init(void){ do some... }
PRE_INIT_FUNC(pre_lock_init);
#pragma GCC pop_options
***************************************************/
#define PRE_INIT_FUNC(F) static __attribute__((section(".preinit_array"))) void (*__##F)(void) = F

#define INLINE inline __attribute__((always_inline))

    __attribute__((always_inline)) static __inline uint32_t __get_PRIMASK(void)
    {
        uint32_t primask;
        __asm volatile("mrs %0, PRIMASK"
                       : "=r"(primask)::);
        return primask;
    }

    __attribute__((always_inline)) static __inline void __set_PRIMASK(uint32_t primask)
    {
        __asm volatile("msr PRIMASK,%0" ::"r"(primask)
                       :);
    }

    __attribute__((always_inline)) static __inline uint32_t __disable_irq(void)
    {
        uint32_t primask = __get_PRIMASK();
        __asm volatile("cpsid i"
                       :
                       :
                       : "memory");
        return primask;
    }

    __attribute__((always_inline)) static __inline void __enable_irq(void)
    {
        __asm volatile("cpsie i"
                       :
                       :
                       : "memory");
    }

#define ENTER_CRITICAL() uint32_t _prim = save_and_disable_interrupts()
#define EXIT_CRITICAL() restore_interrupts(_prim)

    //

    static inline void interrupts(void) { __enable_irq(); }
    static inline void noInterrupts(void) { __disable_irq(); }
    void attachInterruptEx(uint8_t pin, void (*cb)(uint32_t pin_event), int mode); // enum gpio_irq_level

    //

    static inline unsigned int micros(void) { return to_us_since_boot(get_absolute_time()); }
    static inline unsigned int millis(void) { return to_ms_since_boot(get_absolute_time()); }
    static inline unsigned int seconds(void) { return millis() / 1000; }

    static inline void delayMicroseconds(unsigned int us) { sleep_us(us); };
    static inline void delay(unsigned int ms)
    {
#ifdef USE_FREERTOS
        vTaskDelay(pdMS_TO_TICKS(ms));
#else
    sleep_ms(ms);
#endif
    }
    static inline void delaySeconds(unsigned int s) { delay(s * 1000); };

    static inline void yield(void)
    {
#ifdef USE_FREERTOS
        vTaskDelay(0);
#endif
    }

    uint32_t clockCyclesPerMicrosecond(void);
    uint32_t clockCyclesToMicroseconds(uint32_t a);
    uint32_t microsecondsToClockCycles(uint32_t a);

    void analogInit(uint8_t adc_channel);
    int analogRead(uint8_t adc_channel);
    void analogWrite(uint8_t pin, int val);
    float temperatureRead(void);

    //

    char *utoa(unsigned int value, char *buffer, int radix);
    static inline char *ltoa(long value, char *result, int base) { return utoa(value, result, base); }
    static inline char *ultoa(unsigned long value, char *result, int base) { return utoa(value, result, base); }
    unsigned int strhash(char *src);
    
#define run_core(core1_loop_func) multicore_launch_core1(core1_loop_func)
#define reset_core multicore_reset_core1()

    //

#ifndef SERIAL_BUFFER_SIZE
#define SERIAL_BUFFER_SIZE 1024
#endif

#define digitalPinToPort(p)
#define digitalPinToBitMask(p)
#define digitalPinToClkid(p)
#define digitalPinSPIAvailiable(p)
#define digitalPinToSPIDevice(p)
#define digitalPinToSPIClockId(p)

#ifdef __cplusplus
}

#endif

#endif /* INTERFACE_H_ */
