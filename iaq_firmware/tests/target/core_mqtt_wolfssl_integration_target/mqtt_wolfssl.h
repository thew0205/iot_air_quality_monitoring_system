#ifndef MQTT_WOLFSSL_H_
#define MQTT_WOLFSSL_H_

#include "core_mqtt.h"
void init_conn();
bool tcp_conn();
void init_tls();
bool tls_connect();

bool mqtt_connect();
bool mqtt_publish(const char *topic, const char *payload, MQTTQoS_t qos);
void close_conn();

#endif /* ifndef MQTT_WOLFSSL_H_ */