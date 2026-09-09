#pragma once
#include <cstdint>
#include "hardware/adc.h"

void init_adc_pin(unsigned int gpio_pin);
float read_adc_voltage(unsigned int gpio_pin);



