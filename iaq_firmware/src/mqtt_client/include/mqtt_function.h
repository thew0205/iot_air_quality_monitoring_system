#ifndef MQTT_WOLFSSL_H_
#define MQTT_WOLFSSL_H_

#include "core_mqtt.h"

#ifdef __cplusplus
extern "C"
{
#endif

    // enum ConnectionState
    // {
    //     WIFI_NO_INIT = 0,
    //     WIFI_INIT,
    //     WIFI_CONNECTED,
    //     TCP_CONNECTED,
    //     TLS_CONNECTED,
    //     MQTT_CONNECTED,
    // };

    void wifi_init(const char *ssid, const char *password);

    void init_conn();
    bool tcp_conn();
    bool tcp_is_connected();

    void init_tls();
    bool tls_connect();

    bool mqtt_connect();
    bool mqtt_publish(const char *topic, const char *payload, MQTTQoS_t qos);
    void close_conn();

    bool mqtt_connected();
    bool mqtt_loop();
    bool full_connecion();

#ifdef __cplusplus
}
#endif
#endif /* ifndef MQTT_WOLFSSL_H_ */