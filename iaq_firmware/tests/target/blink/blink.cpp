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
#include "ff_headers.h"
#include "ff_sddisk.h"
#include "ff_stdio.h"
#include "ff_utils.h"
//
#include "hw_config.h"

    static inline void
    stop()
{
    fflush(stdout);
    __breakpoint();
}

// See https://www.freertos.org/FreeRTOS-Plus/FreeRTOS_Plus_FAT/Standard_File_System_API.html

static void SimpleTask(void *arg)
{
    (void)arg;

    printf("\n%s: Hello, world!\n", pcTaskGetName(NULL));

    FF_Disk_t *pxDisk = FF_SDDiskInit("sd0");
    configASSERT(pxDisk);
    FF_Error_t xError = FF_SDDiskMount(pxDisk);
    if (FF_isERR(xError) != pdFALSE)
    {
        FF_PRINTF("FF_SDDiskMount: %s\n",
                  (const char *)FF_GetErrMessage(xError));
        stop();
    }
    FF_FS_Add("/sd0", pxDisk);

    FF_FILE *pxFile = ff_fopen("/sd0/filename.txt", "a");
    if (!pxFile)
    {
        FF_PRINTF("ff_fopen failed: %s (%d)\n", strerror(stdioGET_ERRNO()),
                  stdioGET_ERRNO());
        stop();
    }
    if (ff_fprintf(pxFile, "Hello, world!\n") < 0)
    {
        FF_PRINTF("ff_fprintf failed: %s (%d)\n", strerror(stdioGET_ERRNO()),
                  stdioGET_ERRNO());
        stop();
    }
    if (-1 == ff_fclose(pxFile))
    {
        FF_PRINTF("ff_fclose failed: %s (%d)\n", strerror(stdioGET_ERRNO()),
                  stdioGET_ERRNO());
        stop();
    }
    FF_FS_Remove("/sd0");
    FF_Unmount(pxDisk);
    FF_SDDiskDelete(pxDisk);
    puts("Goodbye, world!");

    vTaskDelete(NULL);
}

#include "pico/stdlib.h"
#include "wifi.h"

#include "FreeRTOS.h"
#include "task.h"

// Pico W devices use a GPIO on the WIFI chip for the LED,
// so when building for Pico W, CYW43_WL_GPIO_LED_PIN will be defined
#ifdef CYW43_WL_GPIO_LED_PIN
#include "pico/cyw43_arch.h"
#endif

#ifndef LED_DELAY_MS
#define LED_DELAY_MS 250
#endif

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
    return cyw43_arch_init();
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

void mainTask2(void *param)
{

    stdio_init_all();
    int rc = pico_led_init();
    hard_assert(rc == PICO_OK);
    while (true)
    {
        reConnect();
        vTaskDelay(
            2000);
    }
}

/*
 * main.cpp
 *
 *  Created on: 21-Sep-2024
 *      Author: jondurrant
 */

#include <cstdio>

// Standard Task priority
#define TASK_PRIORITY (tskIDLE_PRIORITY + 1UL)

#define GP_BLINK 2

/***
 * Debug function to look at Task Stats
 */
void runTimeStats()
{
    TaskStatus_t *pxTaskStatusArray;
    volatile UBaseType_t uxArraySize, x;
    unsigned long ulTotalRunTime;

    // Get number of takss
    uxArraySize = uxTaskGetNumberOfTasks();
    printf("Number of tasks %d\n", uxArraySize);

    // Allocate a TaskStatus_t structure for each task.
    pxTaskStatusArray = (TaskStatus_t *)pvPortMalloc(uxArraySize * sizeof(TaskStatus_t));

    if (pxTaskStatusArray != NULL)
    {
        // Generate raw status information about each task.
        uxArraySize = uxTaskGetSystemState(pxTaskStatusArray,
                                           uxArraySize,
                                           &ulTotalRunTime);

        // Print stats
        for (x = 0; x < uxArraySize; x++)
        {
            printf("Task: %d \t cPri:%d \t bPri:%d \t hw:%d \t%s\n",
                   pxTaskStatusArray[x].xTaskNumber,
                   pxTaskStatusArray[x].uxCurrentPriority,
                   pxTaskStatusArray[x].uxBasePriority,
                   pxTaskStatusArray[x].usStackHighWaterMark,
                   pxTaskStatusArray[x].pcTaskName);
        }

        // Free array
        vPortFree(pxTaskStatusArray);
    }
    else
    {
        printf("Failed to allocate space for stats\n");
    }

    // Get heap allocation information
    HeapStats_t heapStats;
    vPortGetHeapStats(&heapStats);
    printf("HEAP avl: %d, blocks %d, alloc: %d, free: %d\n",
           heapStats.xAvailableHeapSpaceInBytes,
           heapStats.xNumberOfFreeBlocks,
           heapStats.xNumberOfSuccessfulAllocations,
           heapStats.xNumberOfSuccessfulFrees);
}

/***
 * Main task to boot the other Agents
 * @param params - unused
 */
void mainTask(void *params)
{
    auto res = xTaskCreate(
        mainTask2, /* Function that implements the task. */
        "pName",   /* Text name for the task. */
        10000,     /* Stack size in words, not bytes. */
        NULL,      /* Parameter passed into the task. */
        2,         /* Priority at which the task is created. */
        NULL);

    // printf("Boot task started\n");

    for (;;)
    {
        // reConnect();

        vTaskDelay(2000);
        runTimeStats();
    }
}

/***
 * Launch the tasks and scheduler
 */
void vLaunch(void)
{

    // Start blink task
    TaskHandle_t task;
    xTaskCreate(mainTask, "MainThread", 500, NULL, TASK_PRIORITY, &task);

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
