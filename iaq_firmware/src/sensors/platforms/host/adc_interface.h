#pragma once
#include <cstdint>

void adc_init(void);
void init_adc_pin(unsigned int gpio_pin);
float read_adc_voltage(unsigned int gpio_pin);



