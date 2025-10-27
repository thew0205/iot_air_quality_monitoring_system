#pragma once

#include <cstdint>
#include "hardware/adc.h"

float read_mq_adc(uint32_t adc_channel);
void init_adc_sensors();