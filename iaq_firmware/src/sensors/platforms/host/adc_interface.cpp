#include "adc_interface.h"

#include <stdlib.h> // Required for rand() and srand()
#include <time.h>   // Required for time()

#define ADC_VREF 3.3f
#define ADC_RESOLUTION 4095.0f

void adc_init(void) {}

void init_adc_pin(unsigned int gpio_pin)
{
    srand(time(NULL));
}

float read_adc_voltage(unsigned int gpio_pin)
{
    float raw =
        (float)rand() / (float)RAND_MAX;
    return (raw * ADC_VREF);
}
