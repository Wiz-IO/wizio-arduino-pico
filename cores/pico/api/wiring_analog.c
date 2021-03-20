////////////////////////////////////////////////////////////////////////////////////////
//
// Copyright 2020 Georgi Angelov ver 2.0
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

#include <variant.h>
#include "hardware/gpio.h"
#include "hardware/adc.h"

void analogInit(uint8_t adc_channel)
{
    static int adc_once = 0;
    if (!adc_once)
    {
        adc_once = 1;
        adc_init();
    }
    adc_channel += 26;    // GPIO26
    if (adc_channel > 29) // GPIO29
        adc_set_temp_sensor_enabled(true);
    else
        adc_gpio_init(adc_channel); // Make sure GPIO is high-impedance, no pullups etc
}

INLINE int analogRead(uint8_t adc_channel)
{
    adc_select_input(adc_channel); // Select ADC_0 (GPIO26), ADC_T is TEMPERATURE
    return adc_read();
}

float temperatureRead(void)
{
    const float t = analogRead(ADC_T) * 3.3f / (1 << 12);
    return 27.0 - (t - 0.706) / 0.001721;
}

void analogWrite(uint8_t channel, int val)
{
    // PWM
}
