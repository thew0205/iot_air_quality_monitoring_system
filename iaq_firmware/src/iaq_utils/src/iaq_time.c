#include "iaq_utils/iaq_time.h"

#ifdef USE_FREE_RTOS
#include <FreeRTOS.h>
#include <task.h>
#include <portmacro.h>
#else
#include "pico/stdlib.h"
#endif

uint32_t iaq_millis()
{
#ifdef USE_FREE_RTOS
    return xTaskGetTickCount() * portTICK_PERIOD_MS;
#else
    return (time_us_64() / 1000);
#endif
}