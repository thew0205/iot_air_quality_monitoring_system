#pragma once

#include <cstdint>
#include "hardware/adc.h"

void init_adc();
void init_adc_pin(uint gpio_pin);
float read_adc_voltage(uint gpio_pin);


