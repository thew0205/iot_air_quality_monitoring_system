#pragma once

#include "pico/stdlib.h"
#include "pico/util/datetime.h"

namespace IAQ_RTC {
    void init_rtc();
    bool get_rtc_time(datetime_t *t);
    void set_rtc_time(datetime_t *t);
}
