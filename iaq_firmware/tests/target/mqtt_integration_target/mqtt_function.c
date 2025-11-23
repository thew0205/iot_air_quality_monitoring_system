#include "mqtt_function.h"
#include "pico/stdlib.h"
#include "pico/cyw43_arch.h"
#include "pico/unique_id.h"
#include "hardware/gpio.h"
#include "hardware/irq.h"
#include "hardware/adc.h"
#include "lwip/apps/mqtt.h"
#include "lwip/apps/mqtt_priv.h"
#include "lwip/dns.h"
#include "lwip/altcp_tls.h"

#include <stdlib.h> // For rand() and srand()
#include <time.h>

#ifndef TEMPERATURE_UNITS
#define TEMPERATURE_UNITS 'C'
#endif

#include "mqtt_client.inc"

#include "mqtt_function.h"

#ifndef MQTT_TOPIC_LEN
#define MQTT_TOPIC_LEN 100
#endif
#include "FreeRTOS.h"
#include "task.h"

#ifndef DEBUG_printf
#ifndef NDEBUG
#define DEBUG_printf printf
#else
#define DEBUG_printf(...)
#endif
#endif

#ifndef INFO_printf
#define INFO_printf printf
#endif

#ifndef ERROR_printf
#define ERROR_printf printf
#endif

#define TEMP_WORKER_TIME_S 5

#define MQTT_KEEP_ALIVE_S 60

#define MQTT_SUBSCRIBE_QOS 1
#define MQTT_PUBLISH_QOS 1
#define MQTT_PUBLISH_RETAIN 0

#define MQTT_WILL_TOPIC "/online"
#define MQTT_WILL_MSG "0"
#define MQTT_WILL_QOS 1

#ifndef MQTT_DEVICE_NAME
#define MQTT_DEVICE_NAME "pico"
#endif

bool wifi_init(const char *ssid, const char *password)
{

    if (cyw43_arch_init())
    {
        panic("Failed to inizialize CYW43");
    }
    cyw43_arch_enable_sta_mode();
    if (cyw43_arch_wifi_connect_timeout_ms(ssid, password, CYW43_AUTH_WPA2_AES_PSK, 30000))
    {
        panic("Failed to connect");
    }
}

static void pub_request_cb(__unused void *arg, err_t err)
{
    if (err != 0)
    {
        ERROR_printf("pub_request_cb failed %d", err);
    }
}

static void sub_request_cb(void *arg, err_t err)
{
    MQTT_CLIENT_DATA_T *state = (MQTT_CLIENT_DATA_T *)arg;
    if (err != 0)
    {
        panic("subscribe request failed %d", err);
    }
    state->subscribe_count++;
}

static void mqtt_incoming_data_cb(void *arg, const u8_t *data, u16_t len, u8_t flags)
{
    MQTT_CLIENT_DATA_T *state = (MQTT_CLIENT_DATA_T *)arg;
}

static void mqtt_incoming_publish_cb(void *arg, const char *topic, u32_t tot_len)
{
    MQTT_CLIENT_DATA_T *state = (MQTT_CLIENT_DATA_T *)arg;
    strncpy(state->topic, topic, sizeof(state->topic));
}

static void mqtt_connection_cb(mqtt_client_t *client, void *arg, mqtt_connection_status_t status)
{
    printf("Connectio\n");
}

static void start_client(MQTT_CLIENT_DATA_T *state)
{
    const int port = MQTT_TLS_PORT;
    INFO_printf("Using TLS\n");

    state->mqtt_client_inst = mqtt_client_new();
    if (!state->mqtt_client_inst)
    {
        panic("MQTT client instance creation error");
    }
    INFO_printf("IP address of this device %s\n", ipaddr_ntoa(&(netif_list->ip_addr)));
    INFO_printf("Connecting to mqtt server at %s\n", ipaddr_ntoa(&state->mqtt_server_address));

    cyw43_arch_lwip_begin();
    if (mqtt_client_connect(state->mqtt_client_inst, &state->mqtt_server_address, port, mqtt_connection_cb, state, &state->mqtt_client_info) != ERR_OK)
    {
        panic("MQTT broker connection error");
    }
    mbedtls_ssl_set_hostname(altcp_tls_context(state->mqtt_client_inst->conn), MQTT_SERVER);
    mqtt_set_inpub_callback(state->mqtt_client_inst, mqtt_incoming_publish_cb, mqtt_incoming_data_cb, state);

    cyw43_arch_lwip_end();

    while (!mqtt_connected())
    {
        vTaskDelay(500);
    }
    cyw43_arch_lwip_begin();

    mqtt_sub_unsub(state->mqtt_client_inst, "/led", 0, sub_request_cb, state, true);
    cyw43_arch_lwip_end();

    while (mqtt_connected())
    {
        vTaskDelay(1000);
        cyw43_arch_lwip_begin();

        mqtt_publish(state->mqtt_client_inst, "/test/topic", "Hello world", 11, 1, 0, NULL, NULL);
        cyw43_arch_lwip_end();
    }

    state->connect_done = true;
}
static void dns_found(const char *hostname, const ip_addr_t *ipaddr, void *arg)
{
    MQTT_CLIENT_DATA_T *state = (MQTT_CLIENT_DATA_T *)arg;
    if (ipaddr)
    {
        state->mqtt_server_address = *ipaddr;
        state->dns_found = true;
    }
    else
    {
        state->dns_found = false;

        panic("dns request failed");
    }
}
static MQTT_CLIENT_DATA_T state;

void mqtt_connect()
{

    const char *client_id_buf = "iaq_pico";

    INFO_printf("Device name %s\n", client_id_buf);

    state.mqtt_client_info.client_id = client_id_buf;
    state.mqtt_client_info.keep_alive = MQTT_KEEP_ALIVE_S;

    state.mqtt_client_info.client_user = NULL;
    state.mqtt_client_info.client_pass = NULL;

    static char will_topic[MQTT_TOPIC_LEN];
    strncpy(will_topic, MQTT_WILL_TOPIC, sizeof(MQTT_WILL_TOPIC));
    state.mqtt_client_info.will_topic = NULL;
    state.mqtt_client_info.will_msg = MQTT_WILL_MSG;
    state.mqtt_client_info.will_qos = MQTT_WILL_QOS;
    state.mqtt_client_info.will_retain = true;

    state.mqtt_client_info.tls_config = altcp_tls_create_config_client_2wayauth(AWS_ROOT_CA, strlen(AWS_ROOT_CA) + 1,
                                                                                DEVICE_KEY, strlen(DEVICE_KEY) + 1, NULL, 0, DEVICE_CERT, strlen(DEVICE_CERT) + 1);

    INFO_printf("\nConnected to Wifi\n");

    cyw43_arch_lwip_begin();
    int err = dns_gethostbyname(MQTT_SERVER, &state.mqtt_server_address, dns_found, &state);
    cyw43_arch_lwip_end();
    while (!state.dns_found)
    {
        vTaskDelay(100);
    }
    start_client(&state);

    if (err == ERR_OK)
    {
    }
    else if (err != ERR_INPROGRESS)
    {
        panic("dns request failed");
    }
}

bool mqtt_connected()
{
    return mqtt_client_is_connected(state.mqtt_client_inst);
}