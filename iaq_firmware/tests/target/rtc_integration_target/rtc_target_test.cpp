#include "CppUTest/TestHarness.h"
#include "CppUTest/CommandLineTestRunner.h"
#include "rtc.h"
#include "pico/stdlib.h"
void set_initial_time_once() {
    // ----------------------------------------------------
    // *** 1. DEFINE YOUR DESIRED TIME HERE ***
    // Format: (year, month, day, dotw, hour, minute, second)
    // ----------------------------------------------------
    
    // Example: Tuesday, November 4, 2025, at 14:38:00
    datetime_t t = {
            .year  = 2025,
            .month = 11,
            .day   = 9,
            .dotw  = 0, // 0 is Sunday, 1 is Monday, 2 is Tuesday
            .hour  = 19,
            .min   = 30,
            .sec   = 00
    };

    printf("Attempting to set RTC time...\n");
    
    // 2. Call the set_time function
    IAQ_RTC::set_time(&t);
    printf("✅ DS3231 time set successfully!\n");
}

TEST_GROUP(RTCTargetTestGroup)
{
    void setup()
    {
        stdio_init_all();
        IAQ_RTC::init();
    }

    void teardown()
    {
    }
};

TEST(RTCTargetTestGroup, GetTime)
{
    datetime_t t;
    bool result = IAQ_RTC::get_time(&t);
    char datetime_buf[256];
    char *datetime_str = &datetime_buf[0];
        if (IAQ_RTC::get_time(&t)) {
            datetime_to_str(datetime_str, sizeof(datetime_buf), &t);
            std::printf("RTC DATETIME: %s\n", datetime_str);
        }

    CHECK_TRUE(result);
}

int main(int ac, char** av)
{


    return CommandLineTestRunner::RunAllTests(ac, av);
}
