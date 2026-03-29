#ifndef __SYSTEM_STATE_TASK_H__
#define __SYSTEM_STATE_TASK_H__

enum SystemState
{
    NORMAL = 0x0000,
    WIFI_CONNECTED,
    TLS_CONNECTED ,
    MQTT_CONNECTED,
    SYS_ERROR = 10,
};
void systemStateTask(void *para);
#ifdef __cplusplus

extern "C"
{
#endif
    bool sendState(enum SystemState state);
#ifdef __cplusplus
}
#endif

#endif // __SYSTEM_STATE_TASK_H__