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

#include "pico/stdlib.h"

#include "FreeRTOS.h"
#include "task.h"

#include <cstdio>

#include "CppUTest/TestHarness.h"
#include "CppUTest/CommandLineTestRunner.h"

// Standard Task priority
#define TASK_PRIORITY (tskIDLE_PRIORITY + 1UL)

#define GP_BLINK 2

static inline void stop()
{
    fflush(stdout);
    __breakpoint();
}

// See https://www.freertos.org/FreeRTOS-Plus/FreeRTOS_Plus_FAT/Standard_File_System_API.html

TEST_GROUP(SdCard){
    void setup() override{
        // Code here will be called before each test in this group
    }

    void teardown() override{
        // Code here will be called after each test in this group
    }};

TEST(SdCard, Write)
{

    FF_FILE *pxFile = fat_sd_card_open("/sd0/buffer1.bin", "w");
    CHECK(pxFile != NULL);
    int m = 0xdeadbeef;
    CHECK(fat_sd_card_write(&m, sizeof(m), pxFile));
    CHECK(fat_sd_card_close(pxFile));
}

TEST(SdCard, WriteRead)
{

    FF_FILE *pxFile = fat_sd_card_open("/sd0/buffer2.bin", "w");
    CHECK(pxFile != NULL);
    int m2 = 0xdeadbeef;
    CHECK(fat_sd_card_write(&m2, sizeof(m2), pxFile));
    CHECK(fat_sd_card_close(pxFile));

    pxFile = fat_sd_card_open("/sd0/buffer1.bin", "w");
    CHECK(pxFile != NULL);
    int m = 0xdeadbeef;
    CHECK(fat_sd_card_write(&m, sizeof(m), pxFile));
    CHECK(fat_sd_card_close(pxFile));

    pxFile = fat_sd_card_open("/sd0/buffer1.bin", "r");
    CHECK(pxFile != NULL);
    int m1 = 0;
    CHECK(fat_sd_card_read(&m1, sizeof(m1), pxFile));
    CHECK(fat_sd_card_close(pxFile));

    CHECK_EQUAL(0xdeadbeef, m1);

    pxFile = fat_sd_card_open("/sd0/buffer2.bin", "r");
    CHECK(pxFile != NULL);
    m2 = 0;
    CHECK(fat_sd_card_read(&m2, sizeof(m2), pxFile));
    CHECK(fat_sd_card_close(pxFile));

    LONGS_EQUAL(m1, m2);
}
static void SimpleTask(void *arg)
{
    (void)arg;
    fat_sd_card_init(false);

    printf("\n%s: Hello, world!\n", pcTaskGetName(NULL));

    const char *fav[3] = {"sensor", "-c", "-v"};

    CommandLineTestRunner::RunAllTests(3, fav);
    puts("Goodbye, world!");
    fat_sd_card_deinit();
    vTaskEndScheduler();
    vTaskDelete(NULL);
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
    vTaskStartScheduler();
}
