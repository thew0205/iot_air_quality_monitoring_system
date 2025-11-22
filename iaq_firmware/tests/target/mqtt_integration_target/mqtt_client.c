

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

#ifndef MQTT_TOPIC_LEN
#define MQTT_TOPIC_LEN 100
#endif
#include "FreeRTOS.h"
#include "task.h"

typedef struct
{
    mqtt_client_t *mqtt_client_inst;
    struct mqtt_connect_client_info_t mqtt_client_info;
    char data[MQTT_OUTPUT_RINGBUF_SIZE];
    char topic[MQTT_TOPIC_LEN];
    uint32_t len;
    ip_addr_t mqtt_server_address;
    bool connect_done;
    int subscribe_count;
    bool stop_client;
} MQTT_CLIENT_DATA_T;

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

static float read_onboard_temperature(const char unit)
{

    const float conversionFactor = 3.3f / (1 << 12);

    float adc = (float)adc_read() * conversionFactor;
    float tempC = 27.0f - (adc - 0.706f) / 0.001721f;

    if (unit == 'C' || unit != 'F')
    {
        return tempC;
    }
    else if (unit == 'F')
    {
        return tempC * 9 / 5 + 32;
    }

    return -1.0f;
}

static void pub_request_cb(__unused void *arg, err_t err)
{
    if (err != 0)
    {
        ERROR_printf("pub_request_cb failed %d", err);
    }
}

static const char *full_topic(MQTT_CLIENT_DATA_T *state, const char *name)
{

    return name;
}

static void control_led(MQTT_CLIENT_DATA_T *state, bool on)
{

    const char *message = on ? "On" : "Off";
    if (on)
        cyw43_arch_gpio_put(CYW43_WL_GPIO_LED_PIN, 1);
    else
        cyw43_arch_gpio_put(CYW43_WL_GPIO_LED_PIN, 0);

    mqtt_publish(state->mqtt_client_inst, full_topic(state, "/led/state"), message, strlen(message), MQTT_PUBLISH_QOS, MQTT_PUBLISH_RETAIN, pub_request_cb, state);
}

static void publish_temperature(MQTT_CLIENT_DATA_T *state)
{
    static float old_temperature;
    const char *temperature_key = full_topic(state, "/test/topic");
    float temperature = read_onboard_temperature(TEMPERATURE_UNITS);
    char msg[100];
    int offset = 0;
    offset += sprintf(&msg[offset], "{");
    offset += sprintf(&msg[offset], "\"co2\":%.2f,", (float)(rand()) / (float)(RAND_MAX) * 39);
    offset += sprintf(&msg[offset], "\"pm2_5\":%.2f,", (float)(rand()) / (float)(RAND_MAX) * 1000);
    offset += sprintf(&msg[offset], "\"pm10\":%.2f,", (float)(rand()) / (float)(RAND_MAX) * 1240);
    offset += sprintf(&msg[offset], "\"no2\":%.2f,", (float)(rand()) / (float)(RAND_MAX) * 284);
    offset += sprintf(&msg[offset], "\"co\":%.2f,", (float)(rand()) / (float)(RAND_MAX) * 384);
    offset += sprintf(&msg[offset], "\"temp\":%.2f,", temperature);
    offset += sprintf(&msg[offset], "\"rHum\":%.2f", (float)(rand()) / (float)(RAND_MAX) * 80);
    offset += sprintf(&msg[offset], "}");

    // if (temperature != old_temperature)
    {
        old_temperature = temperature;

        char temp_str[16];
        snprintf(temp_str, sizeof(temp_str), "%.2f", temperature);
        INFO_printf("Publishing %s to %s\n", msg, temperature_key);
        mqtt_publish(state->mqtt_client_inst, temperature_key, msg, strlen(msg), MQTT_PUBLISH_QOS, MQTT_PUBLISH_RETAIN, pub_request_cb, state);
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

static void unsub_request_cb(void *arg, err_t err)
{
    MQTT_CLIENT_DATA_T *state = (MQTT_CLIENT_DATA_T *)arg;
    if (err != 0)
    {
        panic("unsubscribe request failed %d", err);
    }
    state->subscribe_count--;
    assert(state->subscribe_count >= 0);

    if (state->subscribe_count <= 0 && state->stop_client)
    {
        mqtt_disconnect(state->mqtt_client_inst);
    }
}

static void sub_unsub_topics(MQTT_CLIENT_DATA_T *state, bool sub)
{
    mqtt_request_cb_t cb = sub ? sub_request_cb : unsub_request_cb;
    mqtt_sub_unsub(state->mqtt_client_inst, full_topic(state, "/led"), MQTT_SUBSCRIBE_QOS, cb, state, sub);
    mqtt_sub_unsub(state->mqtt_client_inst, full_topic(state, "/print"), MQTT_SUBSCRIBE_QOS, cb, state, sub);
    mqtt_sub_unsub(state->mqtt_client_inst, full_topic(state, "/ping"), MQTT_SUBSCRIBE_QOS, cb, state, sub);
    mqtt_sub_unsub(state->mqtt_client_inst, full_topic(state, "/exit"), MQTT_SUBSCRIBE_QOS, cb, state, sub);
}

static void mqtt_incoming_data_cb(void *arg, const u8_t *data, u16_t len, u8_t flags)
{
    MQTT_CLIENT_DATA_T *state = (MQTT_CLIENT_DATA_T *)arg;

    const char *basic_topic = state->topic;

    strncpy(state->data, (const char *)data, len);
    state->len = len;
    state->data[len] = '\0';

    DEBUG_printf("Topic: %s, Message: %s\n", state->topic, state->data);
    if (strcmp(basic_topic, "/led") == 0)
    {
        if (lwip_stricmp((const char *)state->data, "On") == 0 || strcmp((const char *)state->data, "1") == 0)
            control_led(state, true);
        else if (lwip_stricmp((const char *)state->data, "Off") == 0 || strcmp((const char *)state->data, "0") == 0)
            control_led(state, false);
    }
    else if (strcmp(basic_topic, "/print") == 0)
    {
        INFO_printf("%.*s\n", len, data);
    }
    else if (strcmp(basic_topic, "/ping") == 0)
    {
        char buf[11];
        snprintf(buf, sizeof(buf), "%u", to_ms_since_boot(get_absolute_time()) / 1000);
        mqtt_publish(state->mqtt_client_inst, full_topic(state, "/uptime"), buf, strlen(buf), MQTT_PUBLISH_QOS, MQTT_PUBLISH_RETAIN, pub_request_cb, state);
    }
    else if (strcmp(basic_topic, "/exit") == 0)
    {
        state->stop_client = true;
        sub_unsub_topics(state, false);
    }
}

static void mqtt_incoming_publish_cb(void *arg, const char *topic, u32_t tot_len)
{
    MQTT_CLIENT_DATA_T *state = (MQTT_CLIENT_DATA_T *)arg;
    strncpy(state->topic, topic, sizeof(state->topic));
}

static void temperature_worker_fn(async_context_t *context, async_at_time_worker_t *worker)
{
    MQTT_CLIENT_DATA_T *state = (MQTT_CLIENT_DATA_T *)worker->user_data;
    publish_temperature(state);
    async_context_add_at_time_worker_in_ms(context, worker, TEMP_WORKER_TIME_S * 1000);
}
static async_at_time_worker_t temperature_worker = {.do_work = temperature_worker_fn};

static void mqtt_connection_cb(mqtt_client_t *client, void *arg, mqtt_connection_status_t status)
{
    MQTT_CLIENT_DATA_T *state = (MQTT_CLIENT_DATA_T *)arg;
    if (status == MQTT_CONNECT_ACCEPTED)
    {
        state->connect_done = true;
        sub_unsub_topics(state, true);

        if (state->mqtt_client_info.will_topic)
        {
            mqtt_publish(state->mqtt_client_inst, state->mqtt_client_info.will_topic, "1", 1, MQTT_WILL_QOS, true, pub_request_cb, state);
        }

        temperature_worker.user_data = state;
        async_context_add_at_time_worker_in_ms(cyw43_arch_async_context(), &temperature_worker, 0);
    }
    else if (status == MQTT_CONNECT_DISCONNECTED)
    {
        if (!state->connect_done)
        {
            panic("Failed to connect to mqtt server");
        }
    }
    else
    {
        panic("Unexpected status");
    }
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
}

static void dns_found(const char *hostname, const ip_addr_t *ipaddr, void *arg)
{
    MQTT_CLIENT_DATA_T *state = (MQTT_CLIENT_DATA_T *)arg;
    if (ipaddr)
    {
        state->mqtt_server_address = *ipaddr;
        start_client(state);
    }
    else
    {
        panic("dns request failed");
    }
}

static void mqtt_task(void *para)
{
    static MQTT_CLIENT_DATA_T state;

    if (cyw43_arch_init())
    {
        panic("Failed to inizialize CYW43");
    }

    char unique_id_buf[5];
    pico_get_unique_board_id_string(unique_id_buf, sizeof(unique_id_buf));
    for (int i = 0; i < sizeof(unique_id_buf) - 1; i++)
    {
        unique_id_buf[i] = tolower(unique_id_buf[i]);
    }

    char client_id_buf[sizeof(MQTT_DEVICE_NAME) + sizeof(unique_id_buf) - 1];
    memcpy(&client_id_buf[0], MQTT_DEVICE_NAME, sizeof(MQTT_DEVICE_NAME) - 1);
    memcpy(&client_id_buf[sizeof(MQTT_DEVICE_NAME) - 1], unique_id_buf, sizeof(unique_id_buf) - 1);
    client_id_buf[sizeof(client_id_buf) - 1] = 0;
    INFO_printf("Device name %s\n", client_id_buf);

    state.mqtt_client_info.client_id = client_id_buf;
    state.mqtt_client_info.keep_alive = MQTT_KEEP_ALIVE_S;

    state.mqtt_client_info.client_user = NULL;
    state.mqtt_client_info.client_pass = NULL;

    static char will_topic[MQTT_TOPIC_LEN];
    strncpy(will_topic, full_topic(&state, MQTT_WILL_TOPIC), sizeof(will_topic));
    state.mqtt_client_info.will_topic = NULL;
    state.mqtt_client_info.will_msg = MQTT_WILL_MSG;
    state.mqtt_client_info.will_qos = MQTT_WILL_QOS;
    state.mqtt_client_info.will_retain = true;

    state.mqtt_client_info.tls_config = altcp_tls_create_config_client_2wayauth(AWS_ROOT_CA, strlen(AWS_ROOT_CA) + 1,
                                                                                DEVICE_KEY, strlen(DEVICE_KEY) + 1, NULL, 0, DEVICE_CERT, strlen(DEVICE_CERT) + 1);

    cyw43_arch_enable_sta_mode();
    if (cyw43_arch_wifi_connect_timeout_ms(WIFI_SSID, WIFI_PASSWORD, CYW43_AUTH_WPA2_AES_PSK, 30000))
    {
        panic("Failed to connect");
    }
    INFO_printf("\nConnected to Wifi\n");

    cyw43_arch_lwip_begin();
    int err = dns_gethostbyname(MQTT_SERVER, &state.mqtt_server_address, dns_found, &state);
    cyw43_arch_lwip_end();

    if (err == ERR_OK)
    {

        start_client(&state);
    }
    else if (err != ERR_INPROGRESS)
    {
        panic("dns request failed");
    }

    while (!state.connect_done || mqtt_client_is_connected(state.mqtt_client_inst))
    {
        cyw43_arch_poll();
        cyw43_arch_wait_for_work_until(make_timeout_time_ms(1000));
    }
    INFO_printf("mqtt client exiting\n");
    vTaskDelete(NULL);
    return;
}

int main(void)
{
    stdio_init_all();

    srand(time(NULL));

    INFO_printf("mqtt client starting\n");

    adc_init();
    adc_set_temp_sensor_enabled(true);
    adc_select_input(4);

    // Create our blink task
    xTaskCreate(mqtt_task,
                "BlinkTask",
                10240, // stack words
                NULL,
                tskIDLE_PRIORITY + 1,
                NULL);

    // Start FreeRTOS scheduler
    vTaskStartScheduler();
    // We should never get here
    for (;;)
        tight_loop_contents();
    return 0;
}
