#include "pico/stdlib.h"
#include "pico/cyw43_arch.h"

#include "FreeRTOS.h"
#include "task.h"

#include "mqtt_wolfssl.h"

void start_tcp_client()
{

    while (1)
    {

        while (true)
        {
            if (!mqtt_connected())
            {
                init_conn();
                (tcp_conn());
                init_tls();

                (tls_connect());
                (mqtt_connect());
            }

            mqtt_publish("/test/topic", "{\"pm1\": 20.0000,\"pm25\": 25.0000,\"pm10\": 27.0000,\"co2\": 768.0000,\"voc\": 0.0000,\"temp\": 27.1000,\"hum\": 66.0000,\"ch2o\": 0.0380,\"co\": 0.5000,\"o3\": 0.0200,\"no2\": 0.0100,\"h2s\": 16266.2354,\"timestamp\": \"05:18:05-01:01:2000\"}", MQTTQoS0);
            // }
            mqtt_loop();
        }

        printf("Current time %lu\n", xTaskGetTickCount());
        printf("Current time %lu\n", xTaskGetTickCount());
        printf("Current time %lu\n", xTaskGetTickCount());
        printf("Current time %lu\n", xTaskGetTickCount());
        init_conn();
        (tcp_conn());
        init_tls();
        (tls_connect());
        (mqtt_connect());
        const char *hello = "my name  is matthew busoye or not. You can use the MQTT test client to monitor the MQTT messages being passed in your AWS account. Devices publish MQTT messages that are identified by topics to communicate their state to AWS IoT. AWS IoT also publishes MQTT messages to inform devices and apps of changes and events. You can subscribe to MQTT message topics and publish MQTT messages to topics by using the MQTT test client.my name is matthew busoye or not. You can use the MQTT test client to monitor the MQTT messages being passed in your AWS account. Devices publish MQTT messages that are identified by topics to communicate their state to AWS IoT. AWS IoT also publishes MQTT messages to inform devices and apps of changes and events. You can subscribe to MQTT message topics and publish MQTT messages to topics by using the MQTT test client.my name is matthew busoye or not. You can use the MQTT test client to monitor the MQTT messages being passed in your AWS account. Devices publish MQTT messages that are identified by topics to communicate their state to AWS IoT. AWS IoT also publishes MQTT messages to inform devices and apps of changes and events. You can subscribe to MQTT message topics and publish MQTT messages to topics by using the MQTT test client.my name is matthew busoye or not. You can use the MQTT test client to monitor the MQTT messages being passed in your AWS account. Devices publish MQTT messages that are identified by topics to communicate their state to AWS IoT. AWS IoT also publishes MQTT messages to inform devices and apps of changes and events. You can subscribe to MQTT message topics and publish MQTT messages to topics by using the MQTT test client.my name is matthew busoye or not. You can use the MQTT test client to monitor the MQTT messages being passed in your AWS account. Devices publish MQTT messages that are identified by topics to communicate their state to AWS IoT. AWS IoT also publishes MQTT messages to inform devices and apps of changes and events. You can subscribe to MQTT message topics and publish MQTT messages to topics by using the MQTT test client.";
        const char *topic = "test/topic";
        mqtt_publish(topic, hello, MQTTQoS0);
        printf("Current time %lu\n", xTaskGetTickCount());
        printf("Current time %lu\n", xTaskGetTickCount());
        printf("Current time %lu\n", xTaskGetTickCount());
        printf("Current time %lu\n", xTaskGetTickCount());

        vTaskDelay(pdMS_TO_TICKS(5000));
        close_conn();
    }
}

/* Task that owns the FreeRTOS-backed async context */
static void blink_task(void *unused)
{
    if (cyw43_arch_init())
    {
        printf("failed to initialise\n");
        return;
    }

    cyw43_arch_enable_sta_mode();

    printf("Connecting to Wi-Fi...\n");
    if (cyw43_arch_wifi_connect_timeout_ms("iaq_wifi", "1234567890", CYW43_AUTH_WPA2_AES_PSK, 30000))
    {
        printf("failed to connect.\n");
        panic("");
        vTaskDelete(NULL);
        return;
    }
    else
    {
        printf("Connected.\n");
    }
    start_tcp_client();
    cyw43_arch_deinit();
    vTaskDelete(NULL);
    return;
}

int main()

{
    stdio_init_all();

    // Create our blink task
    xTaskCreate(blink_task,
                "BlinkTask",
                10000, // stack words
                NULL,
                tskIDLE_PRIORITY + 1,
                NULL);

    // Start FreeRTOS scheduler
    vTaskStartScheduler();

    // We should never get here
    for (;;)
        tight_loop_contents();
}
