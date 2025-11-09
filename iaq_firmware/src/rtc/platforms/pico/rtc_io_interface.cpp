#include "platforms/rtc_io_interface.h"
#include "DS3231.hpp"

namespace IAQ_RTC {
    // I2C bus settings
    #define I2C_PORT i2c0
    #define I2C_SDA 4
    #define I2C_SCL 5

    // Create an instance of the DS3231 class
    DS3231 rtc(I2C_PORT, I2C_SDA, I2C_SCL);

    void init_rtc() {
        
    }

    bool get_rtc_time(datetime_t *t) {
        t->year = rtc.get_year();
        t->month = rtc.get_mon();
        t->day = rtc.get_day();
       // t->dotw = 0; // The new library does not support day of the week
        t->hour = rtc.get_hou();
        t->min = rtc.get_min();
        t->sec = rtc.get_sec();
        return true;
    }

    void set_rtc_time(datetime_t *t) {
        rtc.set_year(t->year);
        rtc.set_mon(t->month);
        rtc.set_day(t->day);
        rtc.set_hou(t->hour, false, false);
        rtc.set_min(t->min);
        rtc.set_sec(t->sec);
    }
}
