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

        char buffer[1000];
        int offset = 0;
        // Print readings to serial
        if (!data.valid)
        {
            std::printf("......SENSOR READINGS......\n");
            offset += sprintf(&buffer[offset], "PM1.0: %.3f  µg/m³ | PM2.5: %.3f  µg/m³ | PM10:  %.3f  µg/m³ \n", data.pm1, data.pm25, data.pm10);
            offset += sprintf(&buffer[offset], "CO₂: %.3f  ppm | VOC Grade: %.3f \n", data.co2, data.voc);
            offset += sprintf(&buffer[offset], "Temp: %.1f °C | Humidity: %.1f %%\n", data.temp, data.hum);
            offset += sprintf(&buffer[offset], "CH₂O: %.3f µg/m³ | CO: %.1f µg/m³ | O₃: %.2f µg/m³ | NO₂: %.2f µg/m³\n\n", data.ch2o_ugm3, data.co_ugm3, data.o3_ugm3, data.no2_ugm3);
        }
        else
        {
            offset += sprintf(&buffer[offset], "ZPHS01B not detected/Invalid response\n\n");
        }
        offset += sprintf(&buffer[offset], ".....MQ ANALOG SENSORS..........\n");
        offset += sprintf(&buffer[offset], "MQ-H2S (ADC0, GPIO26): %.3f µg/m³\n", data.h2s_ugm3);
        offset += sprintf(&buffer[offset], "MQ-NH3 (ADC1, GPIO27): %.3f µg/m³\n", data.nh3_ugm3);
        offset += sprintf(&buffer[offset], "timestamp: %d\n", time_us_64() / 1000);
        std::string data_str{buffer};
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

        fat_sd_card_write("\n=BEGIN=", data_str_p->length(), file);
        fat_sd_card_write(data_str_p->c_str(), data_str_p->length(), file);
        fat_sd_card_write("==END==\n", data_str_p->length(), file);

        fat_sd_card_close(file);
        vTaskDelay(pdMS_TO_TICKS(1 * 1000));
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