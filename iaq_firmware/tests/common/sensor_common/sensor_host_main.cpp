#include "sensor_common_test.h"

#include <stdio.h>
#include <iostream>

#include "pico/stdlib.h"

#include "CppUTest/TestHarness.h"
#include "CppUTest/CommandLineTestRunner.h"

#include "sensors.h"

TEST_GROUP(SensorTest){
    void setup() override{
        // Code here will be called before each test in this group
    }

    void teardown() override{
        // Code here will be called after each test in this group
    }};

TEST(SensorTest, ValidateCheckSumCorrect)
{
    // This is a simple test to check if the reConnect function works
    const uint8_t buffer[] = {0xff, 0x86, 0x00, 0x65, 0x00, 0x36, 0x00, 0x96, 0x01, 0x9A,  0x00, 0x02, 0xFD, 0x00, 0x28, 0x00, 0x28, 0x00, 0x05, 0x00, 0x20, 0x00, 0x50, 0x00, 0x00, 0xEA};
    CHECK(validate_checksum(buffer, sizeof(buffer))); // Replace with actual checks as needed
}

int main(int ac, char **av)
{
    stdio_init_all();
    printf("\nRunning tests with CppUTest\n\n");

    const char *fav[3] = {"sensor", "-c", "-v"};

    return CommandLineTestRunner::RunAllTests(3, fav);
}
