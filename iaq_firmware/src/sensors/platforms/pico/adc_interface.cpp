#include "../platforms/adc_interface.h"

#include "hardware/adc.h"


float read_adc_voltage(uint gpio_pin)
{
    uint channel = gpio_pin - 26; // converts gpio to ADC channel
    adc_select_input(channel);
    uint16_t raw = adc_read();
    return (raw * ADC_VREF) / ADC_RESOLUTION;
}


// void init_adc_sensors()
// {
//     adc_init();
//     adc_gpio_init(26); // MQ-136
//     adc_gpio_init(27); // MQ-137
// }