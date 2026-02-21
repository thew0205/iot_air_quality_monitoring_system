#include "iaq_utils/iaq_assert_panic.h"
#include "pico/stdlib.h"
#include "stdio.h"
#include "hardware/watchdog.h"
void my_assert_func_panic(const char *file, int line, const char *func, const char *pred)
{
    printf("\nAssertion failed: in %s at line %d with %s\n", func, line, pred);
    panic("");
}

void my_assert_func(const char *file, int line, const char *func, const char *pred)
{
    // error_message_printf_plain(
    //     "%s: assertion \"%s\" failed: file \"%s\", line %d, function: %s\n",
    //     pcTaskGetName(NULL), pred, file, line, func);
    // stdio_flush();
    // vTaskSuspendAll();
    // __disable_irq(); /* Disable global interrupts. */
    // capture_assert(file, line, func, pred);
}

void __attribute__((noreturn)) _exit(__unused int status)
{
    while (1)
        watchdog_reboot(0, 0, 0);
}