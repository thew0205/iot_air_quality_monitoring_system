/**
 * Copyright (c) 2020 Raspberry Pi (Trading) Ltd.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

#include "FreeRTOS.h" /* Must come first. */
//
#include <stdio.h>
//
#include "pico/stdlib.h"
//
#include "fat_sd_card.h"
//
#include "hw_config.h"
#include "pico/stdlib.h"

#include "FreeRTOS.h"
#include "task.h"

#include <cstdio>

// Standard Task priority
#define TASK_PRIORITY (tskIDLE_PRIORITY + 1UL)

#define GP_BLINK 2

static inline void stop()
{
    fflush(stdout);
    __breakpoint();
}

// See https://www.freertos.org/FreeRTOS-Plus/FreeRTOS_Plus_FAT/Standard_File_System_API.html

static void SimpleTask(void *arg)
{
    (void)arg;

    printf("\n%s: Hello, world!\n", pcTaskGetName(NULL));

    configASSERT(fat_sd_card_init());

    FF_FILE *pxFile = fat_sd_card_open("/sd0/buffer.bin", "w");
    configASSERT(pxFile);
    char buffer[100];

    int m = 0xdeadbeef;
    configASSERT(fat_sd_card_write(&m, sizeof(m), pxFile));

    // for (int i = 0; i < 10; i++)
    // {
    //     sprintf(buffer, "Test!! %d\n", i);
    //     configASSERT(fat_sd_card_write(buffer, strlen(buffer), pxFile));
    // }
    configASSERT(fat_sd_card_close(pxFile));

    configASSERT(fat_sd_card_deinit());
    puts("Goodbye, world!");

    vTaskDelete(NULL);
}

/***
 * Launch the tasks and scheduler
 */
void vLaunch(void)
{

    /* Start the tasks and timer running. */
    vTaskStartScheduler();
}

int main()
{

    stdio_init_all();
    sleep_ms(1000);

    printf("Go\n");

    // Start tasks and scheduler
    const char *rtos_name = "FreeRTOS";
    printf("Starting %s on core 0:\n", rtos_name);

    xTaskCreate(SimpleTask, "SimpleTask", 1024, NULL, configMAX_PRIORITIES - 1,
                NULL);
    vLaunch();

    for (;;)
    {
        sleep_ms(3000);
    }
}
