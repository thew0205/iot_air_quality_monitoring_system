#include "iaq_firmware.h"

#include <cstdio>
#include "pico/stdlib.h"

#include "sensors.h"
#include "fat_sd_card.h"

#include "FreeRTOS.h"
#include "task.h"

#include <string>

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

    while (true)
    {
        // Read all sensor data
        SensorData data = sensors_read_all();

        char buffer[1000];
        int offset = 0;
        // Print readings to serial
        if (data.valid)
        {
            std::printf("......SENSOR READINGS......\n");
            offset += sprintf(&buffer[offset], "PM1.0: %.3f  µg/m³ | PM2.5: %.3f  µg/m³ | PM10:  %.3f  µg/m³ \n", data.pm1, data.pm25, data.pm10);
            offset += sprintf(&buffer[offset], "CO₂: %.3f  ppm | VOC Grade: %.3f \n", data.co2, data.voc);
            offset += sprintf(&buffer[offset], "Temp: %.1f °C | Humidity: %.1f %%\n", data.temp, data.hum);
            offset += sprintf(&buffer[offset], "CH₂O: %.3f mg/m³ | CO: %.1f ppm | O₃: %.2f ppm | NO₂: %.2f ppm\n\n", data.ch2o, data.co, data.o3, data.no2);
        }
        else
        {
            offset += sprintf(&buffer[offset], "ZPHS01B not detected/Invalid response\n\n");
        }
        offset += sprintf(&buffer[offset], ".....MQ ANALOG SENSORS..........\n");
        offset += sprintf(&buffer[offset], "MQ-H2S (ADC0, GPIO26): %.3f V\n", data.h2s_voltage);
        offset += sprintf(&buffer[offset], "MQ-SNO2 (ADC1, GPIO27): %.3f V\n", data.sno2_voltage);
        offset += sprintf(&buffer[offset], "timestamp: %d\n", time_us_64() / 1000);
        std::string data_str{buffer};
        std::printf("........................\n\n");
        xQueueSend(sensorToStorageQueue, &data_str, portMAX_DELAY);

        // Wait for the next cycle
        vTaskDelay(pdMS_TO_TICKS(1 * 1000));
    }
}

void storageTask(void *para)
{
    std::string data{};
    configASSERT_PANIC(fat_sd_card_init(false));

    while (1)
    {
        xQueueReceive(sensorToStorageQueue, &data, portMAX_DELAY);
        FF_FILE *file = fat_sd_card_open("/sd0/sensor_data.txt", "a");
        fat_sd_card_write(data.c_str(), data.length(), file);
        fat_sd_card_close(file);
        vTaskDelay(pdMS_TO_TICKS(1 * 1000));
    }
    fat_sd_card_deinit();
}
int main()
{
    // Initialise standard I/O
    stdio_init_all();
    sensorToStorageQueue = xQueueCreate(1, sizeof(std::string));
    xTaskCreate(sensorTask, "MainThread", 500, NULL, 2, &taskSensor);
    xTaskCreate(storageTask, "MainThread", 500, NULL, 2, &taskStorage);

    /* Start the tasks and timer running. */
    vTaskStartScheduler();

    /*while (1)
        printTest();
    return 0;*/
}