/**
 * Copyright (c) 2020 Raspberry Pi (Trading) Ltd.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

#include "freertos_common_test.h"

#include "FreeRTOS.h"
#include "task.h"
#include <cstdio>

// Standard Task priority
#define TASK_PRIORITY (tskIDLE_PRIORITY + 1UL)

#define GP_BLINK 2
void mainTask2(void *param)
{

    while (true)
    {
        vTaskDelay(2000);
    }
}

/***
 * Debug function to look at Task Stats
 */
void runTimeStats()
{
    TaskStatus_t *pxTaskStatusArray;
    volatile UBaseType_t uxArraySize, x;
    uint32_t ulTotalRunTime;

    // Get number of takss
    uxArraySize = uxTaskGetNumberOfTasks();
    printf("Number of tasks %ld\n", uxArraySize);

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
            printf("Task: %ld \t cPri:%ld \t bPri:%ld \t hw:%d \t%s\n",
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
    printf("HEAP avl: %ld, blocks %ld, alloc: %ld, free: %ld\n",
           heapStats.xAvailableHeapSpaceInBytes,
           heapStats.xNumberOfFreeBlocks,
           heapStats.xNumberOfSuccessfulAllocations,
           heapStats.xNumberOfSuccessfulFrees);
}

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

