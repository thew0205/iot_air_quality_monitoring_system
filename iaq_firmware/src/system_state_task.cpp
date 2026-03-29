#include "system_state_task.h"

#include "pico/stdlib.h"
#include "iaq_utils/iaq_time.h"

#include "FreeRTOS.h"
#include "task.h"
#include "queue.h"

extern QueueHandle_t systemStateQueue;

static uint32_t systemStateToDelayTime(SystemState state)
{
    switch (state)
    {
    case SystemState::NORMAL:
        return 500;
    case SystemState::WIFI_CONNECTED:
        return 500;
    case SystemState::TLS_CONNECTED:
        return 500;
    case SystemState::MQTT_CONNECTED:
        return 500;
    case SystemState::SYS_ERROR:
        return 250;
    default:
        return 1000;
    }
}

static uint32_t systemStateToBlinkCount(SystemState state)
{
    switch (state)
    {
    case SystemState::NORMAL:
        return 1;
    case SystemState::WIFI_CONNECTED:
        return 2;
    case SystemState::TLS_CONNECTED:
        return 3;
    case SystemState::MQTT_CONNECTED:
        return 4;
    case SystemState::SYS_ERROR:
        return 10;
    default:
        return 1;
    }
}

extern "C" bool sendState(SystemState state)
{
    return xQueueSend(systemStateQueue, &state, 0) == pdTRUE;
}
void systemStateTask(void *para)
{

    gpio_init(16);
    gpio_set_dir(16, GPIO_OUT);
    // TickType_t xLastWakeTime = xTaskGetTickCount();
    SystemState state = SystemState::NORMAL;
    uint32_t delayTime = systemStateToDelayTime(state);
    uint32_t blinkCount = systemStateToBlinkCount(state);

    uint32_t currentBlinkCounter = 0;
    while (1)
    {
        if (xQueueReceive(systemStateQueue, &state, 0) == pdTRUE)
        {
            gpio_put(16, 0);

            delayTime = systemStateToDelayTime(state);
            blinkCount = systemStateToBlinkCount(state);
            currentBlinkCounter = 0;
        }
        if (currentBlinkCounter < 2 * blinkCount)
        {
            gpio_put(16, !gpio_get(16));
            currentBlinkCounter++;
        }
        else
        {
            gpio_put(16, 0);
            currentBlinkCounter = 0;
            iaq_delay_ms(3000);
        }
        iaq_delay_ms(delayTime);

        // vTaskDelayUntil(&xLastWakeTime, pdMS_TO_TICKS(delayTime)); ZMe:m:UKLkuW3NT
    }
}
