#include "lwip/api.h"

#include <string.h>
#include <time.h>

#include "pico/stdlib.h"
#include "pico/cyw43_arch.h"

#include "lwip/pbuf.h"
#include "lwip/tcp.h"

#include "FreeRTOS.h"
#include "task.h"

#define TCP_PORT 4242

#include "core_mqtt.h"
int32_t transport_send(NetworkContext_t *pNetworkContext,
                       const void *pBuffer,
                       size_t bytesToSend);

int32_t transport_recv(NetworkContext_t *pNetworkContext,
                       void *pBuffer,
                       size_t bytesToRecv);

typedef struct NetworkContext
{
    struct netconn *conn;
} NetworkContext_t;

int32_t transport_send(NetworkContext_t *ctx,
                       const void *buffer,
                       size_t bytesToSend)
{
    err_t err = netconn_write(ctx->conn, buffer, bytesToSend, NETCONN_COPY);
    return (err == ERR_OK) ? bytesToSend : -1;
}
int32_t transport_recv(NetworkContext_t *ctx,
                       void *buffer,
                       size_t bytesToRecv)
{
    struct netbuf *buf;
    void *data;
    u16_t len;
    size_t received = 0;

    // Check if data is available without blocking
    // Use timeout on the socket

    err_t err = netconn_recv(ctx->conn, &buf);
    if (err != ERR_OK)
    {
        // No data = return 0 (CoreMQTT behavior)
        return 0;
    }

    do
    {
        netbuf_data(buf, &data, &len);
        size_t copy_len = (len > (bytesToRecv - received)) ? (bytesToRecv - received) : len;

        memcpy((uint8_t *)buffer + received, data, copy_len);
        received += copy_len;

        if (received >= bytesToRecv)
            break;

    } while (netbuf_next(buf) >= 0);

    netbuf_delete(buf);
    return received;
}

uint32_t get_time_ms()
{
    return (time_us_64() / 1000);
}
void MQTTEventCallbackFun(struct MQTTContext *pContext,
                          struct MQTTPacketInfo *pPacketInfo,
                          struct MQTTDeserializedInfo *pDeserializedInfo)
{
    printf("MQTTEventCallbackFun: %s\n", pDeserializedInfo->pPublishInfo->pPayload);
}
static uint8_t mqtt_buffer[1024 * 10];
ip_addr_t mqtt_server_address;

void start_tcp_client()
{
    struct netconn *conn;
    struct netbuf *buf;
    void *data;
    u16_t len;

    MQTTContext_t mqttContext;
    MQTTFixedBuffer_t networkBuffer;
    NetworkContext_t networkContext;
    bool session_present = false;
    conn = netconn_new(NETCONN_TCP);
    conn->recv_timeout = 1; // 10ms
    // conn->send_timeout = 5000;

    networkContext.conn = conn;

    if (netconn_gethostbyname("broker.emqx.io", &mqtt_server_address) == ERR_OK)
    {
        printf("Dns successfully\n");
    }
    else
    {

        printf("Dns fail\n");
        panic("");
    }
    netconn_connect(conn, &mqtt_server_address, 1883);

    // const char *msg = "GET / HTTP/1.0\r\n\r\n";
    // netconn_write(conn, msg, strlen(msg), NETCONN_COPY);

    networkBuffer.pBuffer = mqtt_buffer;
    networkBuffer.size = sizeof(mqtt_buffer);
    const TransportInterface_t trans = {.recv = transport_recv, .send = transport_send, .pNetworkContext = &networkContext};
    MQTTStatus_t status = MQTT_Init(&mqttContext,
                                    &trans,
                                    get_time_ms,
                                    MQTTEventCallbackFun,
                                    &networkBuffer);
    MQTTPublishInfo_t willInfo = {0};

    MQTTConnectInfo_t connectInfo = {
        .cleanSession = true,
        .keepAliveSeconds = 60,
        .pClientIdentifier = "client_id",
        .clientIdentifierLength = strlen("client_id")};

    status = MQTT_Connect(&mqttContext,
                          &connectInfo,
                          NULL,
                          1000, // timeout
                          &session_present);

    MQTTSubscribeInfo_t sub =
        {
            .qos = MQTTQoS0,
            .pTopicFilter = "test/topic",
            .topicFilterLength = strlen("test/topic")};

    MQTT_Subscribe(&mqttContext, &sub, 1, MQTT_GetPacketId(&mqttContext));

    const char *hello = "my name is matthew busoye or not.";
    MQTTPublishInfo_t pub =
        {
            .qos = MQTTQoS0,
            .pTopicName = "test/topic",
            .topicNameLength = strlen("test/topic"),
            .pPayload = hello,
            .payloadLength = strlen(hello)};
    uint32_t i = 0;
    while (1)
    {
        /* code */
        MQTT_Publish(&mqttContext, &pub, i++);

        MQTT_ProcessLoop(&mqttContext);

        vTaskDelay(pdMS_TO_TICKS(5000));
    }

    if (netconn_recv(conn, &buf) == ERR_OK)
    {
        do
        {
            netbuf_data(buf, &data, &len);
            printf("%.*s\n", len, (char *)data);
        } while (netbuf_next(buf) >= 0);
        netconn_write(conn, data, len, NETCONN_COPY);

        netbuf_delete(buf);
    }

    netconn_close(conn);
    netconn_delete(conn);
}

/* Task that owns the FreeRTOS-backed async context */
static void blink_task(void *unused)
{
    if (cyw43_arch_init())
    {
        printf("failed to initialise\n");
        return;
    }

    cyw43_arch_enable_sta_mode();

    printf("Connecting to Wi-Fi...\n");
    if (cyw43_arch_wifi_connect_timeout_ms(WIFI_SSID, WIFI_PASSWORD, CYW43_AUTH_WPA2_AES_PSK, 30000))
    {
        printf("failed to connect.\n");
        panic("");
        vTaskDelete(NULL);
        return;
    }
    else
    {
        printf("Connected.\n");
    }
    start_tcp_client();
    cyw43_arch_deinit();
    vTaskDelete(NULL);
    return;
}

int main()
{
    stdio_init_all();

    // Create our blink task
    xTaskCreate(blink_task,
                "BlinkTask",
                1024, // stack words
                NULL,
                tskIDLE_PRIORITY + 1,
                NULL);

    // Start FreeRTOS scheduler
    vTaskStartScheduler();

    // We should never get here
    for (;;)
        tight_loop_contents();
}