#include "hardware/watchdog.h"

void iaq_exit()
{

    watchdog_reboot(0, 0, 0);
}