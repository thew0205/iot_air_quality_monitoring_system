#pragma once

#ifndef CUSTOM_SLEEP_MS
#include "FreeRTOS.h"
#include "task.h"

#define CUSTOM_SLEEP_MS(x) vTaskDelay(pdMS_TO_TICKS(x))
#endif //CUSTOM_SLEEP_MS
