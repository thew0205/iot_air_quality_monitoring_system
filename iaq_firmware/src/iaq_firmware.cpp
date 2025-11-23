#include "iaq_firmware.h"

#include <cstdio>
#include "pico/stdlib.h"

#include "sensors.h"
#include "fat_sd_card.h"
#include "rtc.h"
#include "mqtt_function.h"

#include "FreeRTOS.h"
#include "task.h"

#include <string>
#include <memory>
#include <iomanip> // For std::setprecision, std::fixed
#include <sstream>
#include <pico/cyw43_arch.h>
        using std::shared_ptr;
using std::string;

// Start blink task
TaskHandle_t taskSensor;
TaskHandle_t taskStorage;
TaskHandle_t taskNetwork;
QueueHandle_t sensorToStorageQueue;
QueueHandle_t sensorToNetworkQueue;

void sensorTask(void *para)
{

    std::printf("\n=== IoT Air Quality Board ===\n");

    // Initialize hardware modules
    std::printf("Initialising sensors..\n");
    sensors_init();

    std::printf("\nSystem Ready.\n\n");
    std::ostringstream oss;
    while (true)
    {
        // Read all sensor data
        SensorData data = sensors_read_all();

        int offset = 0;
        // Print readings to serial
        oss << std::fixed << std::setprecision(4);
        oss << "{";
        if (data.valid)
        {

            oss << "\"pm1\":" << data.pm1 << ",";
            oss << "\"pm2.5\":" << data.pm25 << ",";
            oss << "\"pm10\":" << data.pm10 << ",";
            oss << "\"co2\":" << data.co2 << ",";
            oss << "\"voc\":" << data.voc << ",";
            oss << "\"temp\":" << data.temp << ",";
            oss << "\"rhum\":" << data.hum << ",";
            oss << "\"CH₂O\":" << data.ch2o << ",";
            oss << "\"co\":" << data.co << ",";
            oss << "\"O3\":" << data.o3 << ",";
            oss << "\"no2\":" << data.no2 << ",";
        }
        else
        {
            offset += sprintf(&buffer[offset], "ZPHS01B not detected/Invalid response\n\n");
        }

        oss << "\"H2S\":" << data.h2s_voltage << ",";
        oss << "\"SNO2\":" << data.sno2_voltage << ",";

        char buffer[100];

        datetime_t time;
        bool result = IAQ_RTC::get_time(&time);
        datetime_to_str(buffer, 100, &time);
        oss << "\"timestamp\":" << buffer;
        oss << "}";

        shared_ptr<string> data_str_p = std::make_shared<string>(oss.str().c_str());
        shared_ptr<string> data_str_p2 = data_str_p;
        std::printf("........................\n\n");
        xQueueSend(sensorToNetworkQueue, &data_str_p, portMAX_DELAY);
        xQueueSend(sensorToStorageQueue, &data_str_p2, portMAX_DELAY);

        // Clear the buffer and reset the stream state
        oss.str("");
        oss.clear(); // Good practice to clear any error flags

        // Wait for the next cycle
        vTaskDelay(pdMS_TO_TICKS(10 * 1000));
    }
}

void storageTask(void *para)
{
    configASSERT_PANIC(fat_sd_card_init(false));
    FF_FILE *file = fat_sd_card_open("/sd0/sensor_data.txt", "w");
    fat_sd_card_close(file);
    while (1)
    {
        shared_ptr<string> data_str_p{};

        xQueueReceive(sensorToStorageQueue, &data_str_p, portMAX_DELAY);
        FF_FILE *file = fat_sd_card_open("/sd0/sensor_data.txt", "a");
        fat_sd_card_write(data_str_p->c_str(), data_str_p->length(), file);
        fat_sd_card_close(file);
    }
    fat_sd_card_deinit();
}

void networkTask(void *para)
{

    wifi_init("lulo", "llllllll");

    mqtt_connect();
    while (mqtt_connected())
    {
        shared_ptr<string> data_str_p{};

        xQueueReceive(sensorToNetworkQueue, &data_str_p, portMAX_DELAY);

        mqtt_publish_server("/test/topic", data_str_p->c_str(), 1);
    }
    printf("mqtt client exiting\n");
    vTaskDelete(NULL);
}

int main()
{
    // Initialise standard I/O
    stdio_init_all();

    sensorToStorageQueue = xQueueCreate(1, sizeof(shared_ptr<string>));
    sensorToNetworkQueue = xQueueCreate(1, sizeof(shared_ptr<string>));
    xTaskCreate(sensorTask, "sensorThread", 5000, NULL, 2, &taskSensor);
    xTaskCreate(storageTask, "storageThread", 5000, NULL, 2, &taskStorage);
    xTaskCreate(networkTask, "NetworkThread", 5000, NULL, 2, &taskNetwork);
    // vTaskCoreAffinitySet(taskNetwork, (1 << 0));
    /* Start the tasks and timer running. */
    vTaskStartScheduler();

    /*while (1)
        printTest();
    return 0;*/
}