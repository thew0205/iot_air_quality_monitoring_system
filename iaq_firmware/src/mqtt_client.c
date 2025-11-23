

#include "pico/stdlib.h"


#include <stdlib.h> // For rand() and srand()
#include <time.h>

#include "mqtt_function.h"
#include "FreeRTOS.h"
#include "task.h"

#include "hardware/adc.h"
static void mqtt_task(void *para)
{

    wifi_init("lulo", "llllllll");
    mqtt_connect();

    while (mqtt_connected())
    {
        vTaskDelay(100);
    }
    printf("mqtt client exiting\n");
    vTaskDelete(NULL);
    return;
}

int main(void)
{
    stdio_init_all();

    srand(time(NULL));

    printf("mqtt client starting\n");

    adc_init();
    adc_set_temp_sensor_enabled(true);
    adc_select_input(4);

    // Create our blink task
    xTaskCreate(mqtt_task,
                "BlinkTask",
                10240, // stack words
                NULL,
                tskIDLE_PRIORITY + 1,
                NULL);

    // Start FreeRTOS scheduler
    vTaskStartScheduler();
    // We should never get here
    for (;;)
        tight_loop_contents();
    return 0;
}
