#include "iaq_firmware.h"

#include <cstdio>
#include "pico/stdlib.h"

#include "sensors.h"
#include "fat_sd_card.h"
#include "rtc.h"

#include "FreeRTOS.h"
#include "task.h"

#include <string>

#include "memcpy_shared_pointer.h"

using std::string;

#define SENSOR_TO_JSON_FORMAT ("{\"pm1\":%0.4f,\"pm25\": %0.4f,\"pm10\": %0.4f,\"co2\": %0.4f,\"voc\": %0.4f,\"temp\": %0.4f,\"hum\": %0.4f,\"ch2o\": %0.4f,\"co\": %0.4f,\"o3\": %0.4f,\"no2\": %0.4f,\"h2s\": %0.4f,\"timestamp\": \"%02d:%02d:%02d-%02d:%02d:%04d\"}")
// Start blink task
TaskHandle_t taskSensor;
TaskHandle_t taskStorage;
QueueHandle_t sensorToStorageQueue;

void sensorTask(void *para)
{

    std::printf("\n=== IoT Air Quality Board ===\n");

    // Initialize hardware modules
    std::printf("Initialising sensors..\n");
    sensors_init();

    std::printf("\nSystem Ready.\n\n");
    TickType_t xLastWakeTime = xTaskGetTickCount();

    while (true)
    {
        memcpy_shared_ptr<string> data_str_p = make_memcpy_shared_ptr<string>("");

        // Read all sensor data
        SensorData data = sensors_read_all();

        // Print readings to serial
        if (!data.valid)
        {
            continue;
        }

        datetime_t dt;
        IAQ_RTC::get_time(&dt);

        // IAQ_RTC::set_time(&dt);
        int needed_size = sprintf(nullptr, SENSOR_TO_JSON_FORMAT, data.pm1, data.pm25, data.pm10, data.co2, data.voc, data.temp, data.hum, data.ch2o, data.co, data.o3, data.no2, data.h2s_ugm3, dt.hour, dt.min, dt.sec, dt.day, dt.month, dt.year);

        data_str_p->resize(needed_size + 1);
        sprintf(data_str_p->data(), SENSOR_TO_JSON_FORMAT, data.pm1, data.pm25, data.pm10, data.co2, data.voc, data.temp, data.hum, data.ch2o, data.co, data.o3, data.no2, data.h2s_ugm3, dt.hour, dt.min, dt.sec, dt.day, dt.month, dt.year);

        // oss << "\"sno2\":" << data.sno2_voltage << ",";

        std::printf("........................\n\n");
        data_str_p.memcpy_send(nullptr, [](void *, const memcpy_shared_ptr<string> *src)
                               { return xQueueSend(sensorToStorageQueue, src, 10) == pdTRUE; });

        // Wait for the next cycle
        vTaskDelayUntil(&xLastWakeTime, pdMS_TO_TICKS(1000 * 10));
    }
}

void storageTask(void *para)
{
    std::string data{};
    configASSERT_PANIC(fat_sd_card_init(false));
    memcpy_shared_ptr<string> data_str_p{};

    while (1)
    {

        data_str_p.memcpy_receive(nullptr, [](memcpy_shared_ptr<string> *dest, const void *const src)
                                  { return xQueueReceive(sensorToStorageQueue, dest, portMAX_DELAY) == pdTRUE; });

        datetime_t dt;
        IAQ_RTC::get_time(&dt);
        char file_name_buffer[60];
        snprintf(file_name_buffer, sizeof(file_name_buffer), "/sd0/meter_data_%04d-%02d-%02d.json", dt.year, dt.month, dt.day);
        FF_FILE *file = fat_sd_card_open(file_name_buffer, "a");

        fat_sd_card_write("\n=BEGIN=", strlen("\n=BEGIN="), file);
        fat_sd_card_write(data_str_p->c_str(), data_str_p->length(), file);
        fat_sd_card_write("==END==\n", strlen("==END==\n"), file);

        fat_sd_card_close(file);
        vTaskDelay(pdMS_TO_TICKS(1 * 10));
    }
    fat_sd_card_deinit();
}
int main()
{
    // Initialise standard I/O
    stdio_init_all();
    IAQ_RTC::init();
    sensorToStorageQueue = xQueueCreate(1, sizeof(memcpy_shared_ptr<string>));
    xTaskCreate(sensorTask, "MainThread", 500, NULL, 2, &taskSensor);
    xTaskCreate(storageTask, "MainThread", 500, NULL, 2, &taskStorage);

    /* Start the tasks and timer running. */
    vTaskStartScheduler();

    return 0;
}