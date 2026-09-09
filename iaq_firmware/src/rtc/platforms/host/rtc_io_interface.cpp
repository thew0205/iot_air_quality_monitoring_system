#include "rtc_io_interface.h"
#include <stdio.h>
#include <time.h>
#include <stdint.h>

namespace IAQ_RTC
{
    void init_rtc()
    {

        printf("Mock RTC initialized.\n");
    }

    bool get_rtc_time(datetime_t *t)
    {

        if (t)
        {

            time_t raw_time = time(NULL);
            if (raw_time == (time_t)(-1))
            {
                return false;
            }

            struct tm *time_info = localtime(&raw_time);
            if (!time_info)
            {
                return false;
            }

            t->year = (int16_t)(time_info->tm_year + 1900);
            t->month = (int8_t)(time_info->tm_mon + 1);
            t->day = (int8_t)time_info->tm_mday;
            t->dotw = (int8_t)time_info->tm_wday;
            t->hour = (int8_t)time_info->tm_hour;
            t->min = (int8_t)time_info->tm_min;
            t->sec = (int8_t)time_info->tm_sec;

            return true;
        }
        return false;
    }

    void set_rtc_time(datetime_t *t)
    {
        if (t)
        {
            printf("Mock RTC time set to: %04d-%02d-%02d %02d:%02d:%02d (Day %d)\n",
                   t->year, t->month, t->day, t->hour, t->min, t->sec, t->dotw);
        }
    }
}
