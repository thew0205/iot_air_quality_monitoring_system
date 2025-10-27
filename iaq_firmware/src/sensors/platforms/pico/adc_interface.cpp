#include "../platforms/adc_interface.h"


float read_mq_adc(uint32_t adc_channel)
{
    adc_select_input(adc_channel);
    return adc_read();
}


void init_adc_sensors()
{
    adc_init();
    adc_gpio_init(26); // MQ-136
    adc_gpio_init(27); // MQ-137
}