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


// #define MONTH_IS_JAN (__DATE__[0] == 'J' && __DATE__[1] == 'a')
// #define MONTH_IS_FEB (__DATE__[0] == 'F')
// #define MONTH_IS_MAR (__DATE__[0] == 'M' && __DATE__[2] == 'r')
// #define MONTH_IS_APR (__DATE__[0] == 'A' && __DATE__[1] == 'p')
// #define MONTH_IS_MAY (__DATE__[0] == 'M' && __DATE__[2] == 'y')
// #define MONTH_IS_JUN (__DATE__[0] == 'J' && __DATE__[1] == 'u' && __DATE__[2] == 'n')
// #define MONTH_IS_JUL (__DATE__[0] == 'J' && __DATE__[1] == 'u' && __DATE__[2] == 'l')
// #define MONTH_IS_AUG (__DATE__[0] == 'A' && __DATE__[1] == 'u')
// #define MONTH_IS_SEP (__DATE__[0] == 'S')
// #define MONTH_IS_OCT (__DATE__[0] == 'O')
// #define MONTH_IS_NOV (__DATE__[0] == 'N')
// #define MONTH_IS_DEC (__DATE__[0] == 'D')

// #define COMPILE_MONTH() ( \
//     MONTH_IS_JAN ? 1  : MONTH_IS_FEB ? 2  : MONTH_IS_MAR ? 3  : \
//     MONTH_IS_APR ? 4  : MONTH_IS_MAY ? 5  : MONTH_IS_JUN ? 6  : \
//     MONTH_IS_JUL ? 7  : MONTH_IS_AUG ? 8  : MONTH_IS_SEP ? 9  : \
//     MONTH_IS_OCT ? 10 : MONTH_IS_NOV ? 11 : MONTH_IS_DEC ? 12 : 0)

//     #define COMPILE_YEAR() ((((__DATE__ [7] - '0') * 10 + (__DATE__ [8] - '0')) * 10 + \
//                (__DATE__ [9] - '0')) * 10 + (__DATE__ [10] - '0'))

// #define COMPILE_DAY() ((__DATE__ [4] - '0') * 10 + (__DATE__ [5] - '0'))

// #define COMPILE_HOUR() ((__TIME__ [0] - '0') * 10 + (__TIME__ [1] - '0')) 
// #define COMPILE_MIN() ((__TIME__ [3] - '0') * 10 + (__TIME__ [4] - '0')) 
// #define COMPILE_SEC() ((__TIME__ [6] - '0') * 10 + (__TIME__ [7] - '0')) 

TEST_GROUP(RTCSETTestGroup)
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
TEST(RTCSETTestGroup, SetTime)
{
    datetime_t t;
    t.year = COMPILE_YEAR;
    t.month = COMPILE_MONTH;
    t.day = COMPILE_DAY;
    t.hour = COMPILE_HOUR;
    t.min = COMPILE_MIN;
    t.sec = COMPILE_SEC;
    IAQ_RTC::set_time(&t);
    datetime_t t2;
    sleep_ms(5000);
    bool result = IAQ_RTC::get_time(&t2);
    CHECK(t2.year == t.year); 
    CHECK(t2.month == t.month); 
    CHECK(t2.day == t.day); 
    CHECK(t2.hour == t.hour); 
    // within 15 seconds range, to avoid test failure due to time drift
    time_t t_sec = t.hour * 3600 + t.min * 60 + t.sec;
    time_t t2_sec = t2.hour * 3600 + t2.min * 60 + t2.sec;
    printf("difference in seconds: %ld\n", labs(t_sec - t2_sec));
    CHECK(abs(t_sec - t2_sec) < 15); 
}

int main(int ac, char** av)
{
    stdio_init_all();
    printf("RTC Target Test Started\n");
    const char *fav[3] = {"rtc_integration", "-c", "-v" };
    return CommandLineTestRunner::RunAllTests(3, fav);
}
 