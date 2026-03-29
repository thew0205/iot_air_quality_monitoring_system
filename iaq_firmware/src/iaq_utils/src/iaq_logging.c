#include "iaq_utils/iaq_logging.h"

#include <stdio.h>
#include <stdarg.h>

#include "iaq_utils/iaq_time.h"

const static enum LogLevel current_log_level = DEBUG;

// void set_log_level(enum LogLevel level)
// {
//     current_log_level = level;
// }

enum LogLevel get_log_level()
{
    return current_log_level;
}

  void iaq_log(enum LogLevel level, const char *tag, const char * function_name, int line_number, const char *format, ...)
{
    if (level < current_log_level)
    {
        return;
    }
    else
    {
        const char *color_code;
        char levelStr;
        switch (level)
        {
        case VERBOSE:
            levelStr = 'V';
            color_code = "\e[37m"; // White
            break;
        case DEBUG:
            levelStr = 'D';
            color_code = "\e[36m"; // Cyan
            break;
        case INFO:
            levelStr = 'I';
            color_code = "\e[32m"; // Green
            break;
        case WARNING:
            levelStr = 'W';
            color_code = "\e[33m"; // Yellow
            break;
        case ERROR:
            levelStr = 'E';
            color_code = "\e[31m"; // Red
            break;
        default:
            levelStr = 'U';
            color_code = "\e[0m"; // Reset
            break;
        }

        va_list args;
        va_start(args, format);
        printf("%s%c (%d) [%s] {%s:%d}:- ", color_code, levelStr, iaq_millis(), tag, function_name, line_number);
        vprintf(format, args);
        printf("\e[0m\n");
        va_end(args);
    }
}
