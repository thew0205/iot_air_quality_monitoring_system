#include "CppUTest/TestHarness.h"
#include "rtc.h"
#include "pico/stdlib.h"

TEST_GROUP(RTCHostTestGroup)
{
    void setup()
    {
        IAQ_RTC::init();
    }

    void teardown()
    {
    }
};

TEST(RTCHostTestGroup, GetMockTime)
{
    datetime_t t;
    bool result = IAQ_RTC::get_time(&t);

    CHECK_TRUE(result);
    LONGS_EQUAL(2024, t.year);
    LONGS_EQUAL(10, t.month);
    LONGS_EQUAL(30, t.day);
    LONGS_EQUAL(4, t.dotw);
    LONGS_EQUAL(12, t.hour);
    LONGS_EQUAL(0, t.min);
    LONGS_EQUAL(0, t.sec);
}
