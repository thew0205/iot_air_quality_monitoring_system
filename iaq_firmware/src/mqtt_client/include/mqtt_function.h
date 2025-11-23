#pragma once

#include <stdbool.h>

#include "pico/stdlib.h"
#include "pico/cyw43_arch.h"
#include "pico/unique_id.h"
#include "hardware/gpio.h"
#include "hardware/irq.h"
#include "lwip/apps/mqtt.h"
#include "lwip/apps/mqtt_priv.h"
#include "lwip/dns.h"
#include "lwip/altcp_tls.h"

#include <stdlib.h> // For rand() and srand()
#include <time.h>
/* *INDENT-OFF* */
#ifdef __cplusplus
extern "C"
{
#endif

    typedef struct
    {
        mqtt_client_t *mqtt_client_inst;
        struct mqtt_connect_client_info_t mqtt_client_info;
        char data[MQTT_OUTPUT_RINGBUF_SIZE];
        char topic[25];
        uint32_t len;
        ip_addr_t mqtt_server_address;
        bool connect_done;
        int subscribe_count;
        bool stop_client;
        bool dns_found;
    } MQTT_CLIENT_DATA_T;

    bool wifi_init(const char *ssid, const char *password);

    void mqtt_connect();
    bool mqtt_connected();
    void mqtt_publish_server(const char *topic, const char *payload, uint8_t qos);

/* *INDENT-OFF* */
#ifdef __cplusplus
}
#endif