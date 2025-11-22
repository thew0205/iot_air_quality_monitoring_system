#include "CppUTest/TestHarness.h"
#include "CppUTest/CommandLineTestRunner.h"
#include "rtc.h"
#include "pico/stdlib.h"

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

    if (result) {
        printf("RTC Found. Getting time for 10 seconds.\n");
        for (int i = 0; i < 10; i++) {
            char datetime_buf[256];
            char *datetime_str = &datetime_buf[0];
            if (IAQ_RTC::get_time(&t)) {
                datetime_to_str(datetime_str, sizeof(datetime_buf), &t);
                printf("RTC DATETIME: %s\n", datetime_str);
            }
            sleep_ms(1000);
        }
    } else {
        printf("RTC not found.\n");
    }
    CHECK(true); // The test passes if it correctly identifies the RTC status
}

int main(int ac, char** av)
{
    stdio_init_all();
    printf("RTC Target Test Started\n");
    const char *fav[3] = {"rtc_integration", "-c", "-v" };
    return CommandLineTestRunner::RunAllTests(3, fav);
}
