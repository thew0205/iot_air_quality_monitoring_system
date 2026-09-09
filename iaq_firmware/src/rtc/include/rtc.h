#pragma once

#include "rtc_io_interface.h"

namespace IAQ_RTC {
    void init();
    bool get_time(datetime_t *t);
    void set_time(datetime_t *t);
}