#pragma once

#include <cstdint>

void init_adc();
void init_adc_pin(uint gpio_pin);
float read_adc_voltage(uint gpio_pin);


