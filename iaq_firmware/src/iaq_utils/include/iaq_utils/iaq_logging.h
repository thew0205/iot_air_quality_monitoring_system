#ifndef __LOGGING_H__
#define __LOGGING_H__

#ifdef __cplusplus
extern "C"
{
#endif
    enum LogLevel
    {
        VERBOSE = 0,
        DEBUG,
        INFO,
        WARNING,
        ERROR
    };

    void set_log_level(enum LogLevel level);
    enum LogLevel get_log_level();

    void iaq_log(enum LogLevel level, const char *tag, const char * function_name, int line_number, const char *format, ...);

#define LOGV(tag, format, ...) iaq_log(VERBOSE, tag, __FUNCTION__, __LINE__, format, ##__VA_ARGS__)
#define LOGD(tag, format, ...) iaq_log(DEBUG, tag, __FUNCTION__, __LINE__, format, ##__VA_ARGS__)
#define LOGI(tag, format, ...) iaq_log(INFO, tag, __FUNCTION__, __LINE__, format, ##__VA_ARGS__)
#define LOGW(tag, format, ...) iaq_log(WARNING, tag, __FUNCTION__, __LINE__, format, ##__VA_ARGS__)
#define LOGE(tag, format, ...) iaq_log(ERROR, tag, __FUNCTION__, __LINE__, format, ##__VA_ARGS__)
#ifdef __cplusplus
}
#endif

#endif // __LOGGING_H__