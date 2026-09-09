#include "adc_interface.h"

#include "hardware/adc.h"

#define ADC_VREF 3.3f
#define ADC_RESOLUTION 4095.0f

void init_adc_pin(unsigned int gpio_pin)
{
    adc_gpio_init(gpio_pin);
}

float read_adc_voltage(unsigned int gpio_pin)
{
    unsigned int channel = gpio_pin - 26;
    adc_select_input(channel);
    uint16_t raw = adc_read();
    return (raw * ADC_VREF) / ADC_RESOLUTION;
}
