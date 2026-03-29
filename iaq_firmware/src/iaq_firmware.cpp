#include "iaq_firmware.h"

#include <cstdio>
#include "pico/stdlib.h"

#include "sensors.h"
#include "fat_sd_card.h"
#include "rtc.h"
#include "mqtt_function.h"
#include "system_state_task.h"
#include "FreeRTOS.h"
#include "task.h"
#include "queue.h"
#include "portmacro.h"
#include "projdefs.h"

#include <string>
using std::string;

#include "memcpy_shared_pointer.h"

using std::string;

#define SENSOR_TO_JSON_FORMAT ("{\"pm1\":%0.4f,\"pm25\": %0.4f,\"pm10\": %0.4f,\"co2\": %0.4f,\"voc\": %0.4f,\"temp\": %0.4f,\"hum\": %0.4f,\"ch2o\": %0.4f,\"co\": %0.4f,\"o3\": %0.4f,\"no2\": %0.4f,\"h2s\": %0.4f,\"timestamp\": \"%04d-%02d-%02dT%02d:%02d:%02d\"}")
#define TAG "MAIN"
// Start blink task
TaskHandle_t taskSensor;
TaskHandle_t taskStorage;
TaskHandle_t taskSystemState;
TaskHandle_t taskNetwork;
QueueHandle_t sensorToStorageQueue;
QueueHandle_t sensorToNetworkQueue;
QueueHandle_t systemStateQueue;

#ifdef CYW43_WL_GPIO_LED_PIN
#include "pico/cyw43_arch.h"
#endif
#include "iaq_utils/iaq_logging.h"

#ifndef LED_DELAY_MS
#define LED_DELAY_MS 250
#endif
#include "iaq_utils/iaq_time.h"

// Perform initialisation
int pico_led_init(void)
{
#if defined(PICO_DEFAULT_LED_PIN)
    // A device like Pico that uses a GPIO for the LED will define PICO_DEFAULT_LED_PIN
    // so we can use normal GPIO functionality to turn the led on and off
    gpio_init(PICO_DEFAULT_LED_PIN);
    gpio_set_dir(PICO_DEFAULT_LED_PIN, GPIO_OUT);
    return PICO_OK;
#elif defined(CYW43_WL_GPIO_LED_PIN)
    // For Pico W devices we need to initialise the driver etc
    // return cyw43_arch_init();
#endif
}

// Turn the led on or off
void pico_set_led(bool led_on)
{
#if defined(PICO_DEFAULT_LED_PIN)
    // Just set the GPIO on or off
    gpio_put(PICO_DEFAULT_LED_PIN, led_on);
#elif defined(CYW43_WL_GPIO_LED_PIN)
    // Ask the wifi "driver" to set the GPIO on or off
    cyw43_arch_gpio_put(CYW43_WL_GPIO_LED_PIN, led_on);
#endif
}

bool pico_get_led()
{
#if defined(PICO_DEFAULT_LED_PIN)
    // Just read the GPIO
    return gpio_get(PICO_DEFAULT_LED_PIN);
#elif defined(CYW43_WL_GPIO_LED_PIN)
    // Ask the wifi "driver" to read the GPIO
    return cyw43_arch_gpio_get(CYW43_WL_GPIO_LED_PIN);
#endif
}

void sensorTask(void *para)
{

    LOGI(TAG, "=== IoT Air Quality Board ===");

    // Initialize hardware modules
    LOGI(TAG, "Initialising sensors..");
    sensors_init();

    // printf("System Ready.\n\n", xTaskGetCurrentTaskHandle()->pxTaskName);
    TickType_t xLastWakeTime = xTaskGetTickCount();
    xTaskGetCurrentTaskHandle();
    while (true)
    {
        memcpy_shared_ptr<string> data_str_p = make_memcpy_shared_ptr<string>("");

        // Read all sensor data
        SensorData data = sensors_read_all();
        // pico_set_led(true);
        // vTaskDelay(100);
        // pico_set_led(false);

        // Print readings to serial
        if (!data.valid)
        {
            LOGW(TAG, "Invalid sensor data, skipping this cycle.");
            continue;
        }

        datetime_t dt;
        IAQ_RTC::get_time(&dt);

        // IAQ_RTC::set_time(&dt);
        int needed_size = sprintf(nullptr, SENSOR_TO_JSON_FORMAT, data.pm1, data.pm25, data.pm10, data.co2, data.voc, data.temp, data.hum, data.ch2o, data.co, data.o3, data.no2, data.h2s_ugm3, dt.year, dt.month, dt.day, dt.hour, dt.min, dt.sec);

        data_str_p->resize(needed_size + 1);
        sprintf(data_str_p->data(), SENSOR_TO_JSON_FORMAT, data.pm1, data.pm25, data.pm10, data.co2, data.voc, data.temp, data.hum, data.ch2o, data.co, data.o3, data.no2, data.h2s_ugm3, dt.year, dt.month, dt.day, dt.hour, dt.min, dt.sec);

        if (!data_str_p.memcpy_send(nullptr, [](void *, const memcpy_shared_ptr<string> *src)
                                    { return xQueueSend(sensorToStorageQueue, src, 10) == pdTRUE; }))
        {
            LOGW(TAG, "Failed to send data to storage queue\n");
        }

        memcpy_shared_ptr<string> data_str_p2 = make_memcpy_shared_ptr<string>(data_str_p->c_str());
        if (!data_str_p2.memcpy_send(nullptr, [](void *, const memcpy_shared_ptr<string> *src)
                                     { return xQueueSend(sensorToNetworkQueue, src, 10) == pdTRUE; }))
        {
            LOGD(TAG, "Failed to send data to network queue\n");
        }

        // Wait for the next cycle
        vTaskDelayUntil(&xLastWakeTime, pdMS_TO_TICKS(1000 * 10));
    }
}

void storageTask(void *para)
{
    configASSERT_PANIC(fat_sd_card_init(false));
    memcpy_shared_ptr<string> data_str_p{};

    while (1)
    {

        if (data_str_p.memcpy_receive(nullptr, [](memcpy_shared_ptr<string> *dest, const void *const src)
                                      { return xQueueReceive(sensorToStorageQueue, dest, 1000) == pdTRUE; }))
        {
            LOGV(TAG, "Received data for storage: %s\n", data_str_p->c_str());

            datetime_t dt;
            IAQ_RTC::get_time(&dt);
            char file_name_buffer[60];
            snprintf(file_name_buffer, sizeof(file_name_buffer), "/sd0/meter_data_%04d-%02d-%02d.json", dt.year, dt.month, dt.day);
            FF_FILE *file = fat_sd_card_open(file_name_buffer, "a");

            fat_sd_card_write("\n=BEGIN=", strlen("\n=BEGIN="), file);
            fat_sd_card_write(data_str_p->c_str(), data_str_p->length(), file);
            fat_sd_card_write("==END==\n", strlen("==END==\n"), file);

            fat_sd_card_close(file);
        }

        vTaskDelay(pdMS_TO_TICKS(1 * 1000));
    }
    fat_sd_card_deinit();
}

void networkTask(void *para)
{
    // pico_led_init();

    gpio_init(22);
    gpio_set_dir(22, GPIO_OUT);
    gpio_put(22, true);
    vTaskDelay(pdMS_TO_TICKS(60000));

    while (true)
    {

        memcpy_shared_ptr<string> data_str_p{};
        if (full_connecion())
        {
            if (data_str_p.memcpy_receive(nullptr, [](memcpy_shared_ptr<string> *dest, const void *const src)
                                          { return xQueueReceive(sensorToNetworkQueue, dest, 0) == pdTRUE; }))
            {
                LOGV(TAG, "Received data for storage: %s\n", data_str_p->c_str());

                mqtt_publish("test/topic", data_str_p->c_str(), MQTTQoS0);
                mqtt_loop();
            }
        }
        else
        {
            close_conn();
        }

        iaq_delay_ms(1);
    }
    printf("mqtt client exiting\n");
    vTaskDelete(NULL);
}

int main()
{
    // Initialise standard I/O
    stdio_init_all();
    IAQ_RTC::init();

    sensorToStorageQueue = xQueueCreate(1, sizeof(memcpy_shared_ptr<string>));
    sensorToNetworkQueue = xQueueCreate(1, sizeof(memcpy_shared_ptr<string>));
    systemStateQueue = xQueueCreate(5, sizeof(SystemState));
    xTaskCreate(sensorTask, "sensorThread", 5000, NULL, 2, &taskSensor);
    xTaskCreate(storageTask, "storageThread", 5000, NULL, 2, &taskStorage);
    xTaskCreate(networkTask, "NetworkThread", 5000, NULL, 2, &taskNetwork);
    xTaskCreate(systemStateTask, "SystemStateThread", 5000, NULL, 2, &taskSystemState);

    // vTaskCoreAffinitySet(taskNetwork, (1 << 0));
    /* Start the tasks and timer running. */
    vTaskStartScheduler();

    return 0;
}