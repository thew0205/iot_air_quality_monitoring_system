#include "mqtt_function.h"

#include <string.h>
#include <time.h>

#include "iaq_utils/iaq_logging.h"
#include "iaq_utils/iaq_time.h"
#include "lwip/api.h"

#include "pico/stdlib.h"
#include "pico/cyw43_arch.h"

#include "lwip/pbuf.h"
#include "lwip/tcp.h"

#include "FreeRTOS.h"
#include "task.h"

#include "core_mqtt.h"

#include <wolfssl/ssl.h>
#include <wolfssl/certs_test.h>
#include <wolfssl/wolfcrypt/error-crypt.h>
#include <wolfssl/internal.h>

#include "iaq_firmware.h"
#include "temp_keys.h"


#include "/home/busoye_tm/Desktop/iot_air_quality_board/iaq_firmware/src/system_state_task.h"

#define TAG "MQTT_LOG"

bool full_connecion()
{
    wifi_init(WIFI_SSID, WIFI_PASSWORD);
    init_conn();
    (tcp_conn());
    init_tls();

    (tls_connect());
    return (mqtt_connect());
}
static bool wifi_inited = false;
void wifi_init(const char *ssid, const char *password)
{

    if (!wifi_inited)
    {
        close_conn();
        if (cyw43_arch_init() != 0)
        {
            PANIC("Failed to inizialize CYW43");
        }

        int retires = 5;
        while (retires-- > 0 && !wifi_inited)
        {
            cyw43_arch_enable_sta_mode();
            if (cyw43_arch_wifi_connect_timeout_ms(ssid, password, CYW43_AUTH_WPA2_AES_PSK, 30000) == 0)
            {
                wifi_inited = true;
                sendState(WIFI_CONNECTED);
                LOGI(TAG, "Conneted to Wifi %s", ssid);
            }
            else
            {
                sendState(SYS_ERROR);

                LOGW(TAG, "Failed to conect to wifi %s", ssid);
            }
        }
    }
    return;
}

// void wifi_deinit(const char *ssid, const char *password)
// {
//     if (wifi_inited)
//     {
//         cyw43_arch_deinit();

//         wifi_inited = false;
// }
// }
//
bool wifi_connected()
{
    int status = cyw43_tcpip_link_status(&cyw43_state, CYW43_ITF_STA);

    LOGV(TAG, "WiFi status: %d\n", status);

    return status == CYW43_LINK_UP;
}
struct netconn *conn;
static struct netbuf *buf = NULL;
static u16_t cur_data_len = 0;
static u16_t cur_data_pos = 0;
static void *data = NULL;
static bool tcp_connected = false;
static bool tls_connected = false;
static WOLFSSL *ssl = NULL;

typedef struct NetworkContext
{
    WOLFSSL *ssl;

} NetworkContext_t;
MQTTContext_t mqttContext;
static uint8_t mqtt_buffer[1024 * 10];
NetworkContext_t networkContext;

static int32_t mqtt_transport_send(NetworkContext_t *pNetworkContext,
                                   const void *pBuffer,
                                   size_t bytesToSend);

static int32_t mqtt_transport_recv(NetworkContext_t *pNetworkContext,
                                   void *pBuffer,
                                   size_t bytesToRecv);

static int32_t mqtt_transport_send(NetworkContext_t *ctx,
                                   const void *buffer,
                                   size_t bytesToSend)
{

    int ret = wolfSSL_write(ctx->ssl, buffer, bytesToSend);

    if (ret > 0)
        return ret;

    int err = wolfSSL_get_error(ctx->ssl, ret);
    LOGE(TAG, "wolfSSL_write error: %d\n", err);
    if (err == WOLFSSL_ERROR_WANT_WRITE)
        return 0; // CoreMQTT treat 0 as retry

    return -1;
}

static int32_t mqtt_transport_recv(NetworkContext_t *ctx,
                                   void *buffer,
                                   size_t bytesToRecv)
{

    int ret = wolfSSL_read(ctx->ssl, buffer, bytesToRecv);

    if (ret > 0)
        return ret;

    int err = wolfSSL_get_error(ctx->ssl, ret);
    if (err != WOLFSSL_ERROR_WANT_READ)
    {
        LOGE(TAG, "wolfSSL_read error: %d\n", err);
    }

    // Timeout/nonblocking requirement
    if (err == WOLFSSL_ERROR_WANT_READ)
        return 0;

    // Any other error
    return -1;
}

void MQTTEventCallbackFun(struct MQTTContext *pContext,
                          struct MQTTPacketInfo *pPacketInfo,
                          struct MQTTDeserializedInfo *pDeserializedInfo)
{
    LOGI(TAG, "MQTTEventCallbackFun: %s\n", pDeserializedInfo->pPublishInfo->pPayload);
}

/*****************************************************************************/
/* EthernetSend() to send a message string.                                  */
/*****************************************************************************/
int EthernetSend(WOLFSSL *ssl, char *message, int sz, void *ctx)
{
    err_t err = netconn_write(conn, message, sz, NETCONN_COPY);
    tcp_connected = err == ERR_OK;
    if (err != ERR_OK)
    {
        if (err == ERR_CONN || err == ERR_RST || err == ERR_CLSD || err == ERR_ABRT)
        {
            LOGE(TAG, "Connection error: %d\n", err);
            close_conn();
            sz = WOLFSSL_CBIO_ERR_CONN_CLOSE;
        }
        else
        {
            sz = WOLFSSL_CBIO_ERR_WANT_WRITE;
        }
        LOGE(TAG, "netconn_write to network error: %d\n", err);
    }
    return sz;
}

static int EthernetAvailable()
{
    // LOGI(TAG,"MEMP TCP_SEG free: %d\n", lwip_stats.memp[MEMP_TCP_SEG].avail);
    // LOGI(TAG,"PBUF free: %d\n", lwip_stats.tcp.[MEMP_PBUF].avail);
    if (buf == NULL)
    {
        err_t err = netconn_recv(conn, &buf);
        if (err != ERR_OK)
        {

            if (err == ERR_CONN || err == ERR_RST || err == ERR_CLSD || err == ERR_ABRT)
            {
                LOGE(TAG, "Connection error: %d\n", err);
                close_conn();

                return WOLFSSL_CBIO_ERR_CONN_CLOSE;
            }

            if (err != ERR_TIMEOUT)
            {
                LOGE(TAG, "netconn_recv from network error: %d\n", err);
            }
            return 0;
        }

        if (netbuf_data(buf, &data, &cur_data_len) != ERR_OK)
        {
            netbuf_delete(buf);
            buf = NULL;
            return 0;
        }

        cur_data_pos = 0;
    }
    if (cur_data_pos >= cur_data_len)
    {
        if (netbuf_next(buf) < 0)
        {
            netbuf_delete(buf);
            buf = NULL;
            return 0;
        }
        if (netbuf_data(buf, &data, &cur_data_len) != ERR_OK)
        {
            return 0;
        }
        cur_data_len = 0;
    }
    return cur_data_len - cur_data_pos;
}

int EthernetReceive(WOLFSSL *ssl, char *reply, int sz, void *ctx)
{
    size_t received = 0;
    while (sz != 0)
    {
        int current_available_data = EthernetAvailable();

        if (current_available_data < 0)
        {
            return current_available_data;
        }
        else if (current_available_data == 0)
        {
            break;
        }
        int copy_len = current_available_data > sz ? sz : current_available_data;

        memcpy(&reply[received], &((uint8_t *)data)[cur_data_pos], copy_len);
        received += copy_len;
        cur_data_pos += copy_len;
        sz -= copy_len;
    }
    if (received == 0)
    {
        LOGD(TAG, "No data available to read, retrying...\n");
    }
    return received == 0 ? WOLFSSL_CBIO_ERR_WANT_READ : received;
}

void init_conn()
{
    if (conn == NULL)
    {
        conn = netconn_new(NETCONN_TCP);
        conn->recv_timeout = 1000; // 10ms
    }
}

bool tcp_conn()
{
    bool success = true;

    if (tcp_is_connected())
    {
        success = true;
    }
    else
    {
        if (wifi_connected())
        {
            LOGI(TAG, "WiFi connected, proceeding to connect to MQTT server...\n");
        }
        else
        {
            LOGE(TAG, "WiFi not connected, cannot connect to MQTT server\n");
            return false;
        }
        LOGD(TAG, "Connecting to AWS IoT Core...\n");

        ip_addr_t mqtt_server_address = {.addr = 0};
        int retries = 5;
        while (netconn_gethostbyname(AWS_ENDPOINT, &mqtt_server_address) != ERR_OK && retries-- > 0)
        {
            LOGW(TAG, "Dns fail\n");
        }
        if (mqtt_server_address.addr == 0)
        {
            return false;
        }
        LOGI(TAG, "Dns successfully\n");

        if (success && netconn_connect(conn, &mqtt_server_address, AWS_PORT) == ERR_OK)
        {
            tcp_connected = true;

            success = true;
            LOGI(TAG, "Connected successfully\n");
        }
        else
        {
            success = false;
            LOGW(TAG, "Connection fail\n");
        }
        return success;
    }
}

void init_tls()
{

    if (tls_connected)
    {
        return;
    }
    static WOLFSSL_CTX *ctx = NULL;

    // wolfSSL_Debugging_ON();

    int ret = wolfSSL_Init();
    if (ret == WOLFSSL_SUCCESS)
    {
        LOGI(TAG, "Successfully called wolfSSL_Init\n");
    }
    else
    {
        LOGI(TAG, "ERROR: wolfSSL_Init failed\n");
        PANIC("");
    }
    WOLFSSL_METHOD *method;

    method = wolfSSLv23_client_method();

    if (method == NULL)
    {
        LOGI(TAG, "unable to get wolfssl client method\n");
        PANIC("");
    }
    ctx = wolfSSL_CTX_new(method);
    ctx->autoRetry = 1;
    if (ctx == NULL)
    {
        LOGI(TAG, "unable to get ctx\n");
        PANIC("");
    }

    /* Use built-in validation, No verification callback function: */
    wolfSSL_CTX_set_verify(ctx, SSL_VERIFY_PEER, 0);

    /* Certificate */
    LOGI(TAG, "Initializing certificates...\n");
    ret = wolfSSL_CTX_use_certificate_buffer(ctx,
                                             (const unsigned char *)DEVICE_CERT_STRING,
                                             strlen(DEVICE_CERT_STRING),
                                             WOLFSSL_FILETYPE_PEM);
    if (ret == WOLFSSL_SUCCESS)
    {
        LOGI(TAG, "Success: use certificate: ");
        LOGI(TAG, CTX_SERVER_CERT);
    }
    else
    {
        LOGI(TAG, "Error: wolfSSL_CTX_use_certificate_buffer failed: ");
        // wc_ErrorString(ret, wc_error_message);
        // Serial.println(wc_error_message);
        PANIC("");
    }

    /* Setup private client key */
    ret = wolfSSL_CTX_use_PrivateKey_buffer(ctx,
                                            (const unsigned char *)DEVICE_KEY_STRING,
                                            strlen(DEVICE_KEY_STRING),
                                            WOLFSSL_FILETYPE_PEM);
    if (ret == WOLFSSL_SUCCESS)
    {
        LOGI(TAG, "Success: use private key buffer: ");
        // LOGI(TAG,CTX_SERVER_KEY));
    }
    else
    {
        LOGE(TAG, "Error: wolfSSL_CTX_use_PrivateKey_buffer failed: \n");
        // wc_ErrorString(ret, wc_error_message);
        // Serial.println(wc_error_message);
        PANIC("");
    }

    ret = wolfSSL_CTX_load_verify_buffer(ctx,
                                         (const unsigned char *)AWS_ROOT_CA_STRING,
                                         strlen(AWS_ROOT_CA_STRING),
                                         WOLFSSL_FILETYPE_PEM);
    if (ret == WOLFSSL_SUCCESS)
    {
        LOGI(TAG, "Success: load_verify CTX_CA_CERT\n");
    }
    else
    {
        LOGI(TAG, "Error: wolfSSL_CTX_load_verify_buffer failed: \n");
        // wc_ErrorString(ret, wc_error_message);
        // Serial.println(wc_error_message);
        PANIC("");
    }

    /* Initialize wolfSSL using callback functions. */
    wolfSSL_SetIOSend(ctx, EthernetSend);
    wolfSSL_SetIORecv(ctx, EthernetReceive);

    ssl = wolfSSL_new(ctx);
}

bool tls_connect()
{

    if (tls_connected)
    {
        return true;
    }

    if (!tcp_is_connected())
    {
        return false;
    }
    int ret = 0;
    int err = 0;

    LOGI(TAG, "Connecting to wolfSSL TLS Secure Server...\n");

    TickType_t xLastWakeTime = xTaskGetTickCount();

    do
    {

        err = 0; /* reset error */
        LOGI(TAG, "wolfSSL_connect ...\n");
        ret = wolfSSL_connect(ssl);
        LOGI(TAG, "wolfSSL_connect return result =%d\n", ret);
        if (((ret != WOLFSSL_SUCCESS) && (ret != WC_PENDING_E)) || xTaskGetTickCount() > xLastWakeTime + 200000)
        {
            LOGI(TAG, "Failed connection, checking error.\n");
            PANIC("Failed connection, checking error.\n");
            err = wolfSSL_get_error(ssl, ret);
            break;
        }
        else
        {
            LOGI(TAG, "-");
        }
    } while (err == WC_PENDING_E);

    LOGI(TAG, "Connected! %d\n", err);
    LOGI(TAG, "SSL version is %d\n", wolfSSL_get_version(ssl));

    const char *cipherName = wolfSSL_get_cipher(ssl);
    LOGI(TAG, "SSL cipher suite is %s\n", cipherName);
    tls_connected = err == 0;
    if (err == 0)
    {
        sendState(TLS_CONNECTED);
    }
    return err == 0;
}

bool mqtt_connect()
{

    if (mqtt_connected())
    {
        return true;
    }
    if (!tcp_is_connected())
    {
        return false;
    }

    MQTTFixedBuffer_t networkBuffer;
    const TransportInterface_t trans = {.recv = mqtt_transport_recv, .send = mqtt_transport_send, .pNetworkContext = &networkContext};
    bool session_present = false;

    networkContext.ssl = ssl;

    networkBuffer.pBuffer = mqtt_buffer;
    networkBuffer.size = sizeof(mqtt_buffer);
    MQTTStatus_t status = MQTT_Init(&mqttContext,
                                    &trans,
                                    iaq_millis,
                                    MQTTEventCallbackFun,
                                    &networkBuffer);
    MQTTPublishInfo_t willInfo = {0};

    MQTTConnectInfo_t connectInfo = {
        .cleanSession = true,
        .keepAliveSeconds = 60,
        .pClientIdentifier = CLIENT_ID,
        .clientIdentifierLength = strlen(CLIENT_ID)};

    status = MQTT_Connect(&mqttContext,
                          &connectInfo,
                          NULL,
                          1000, // timeout
                          &session_present);
    LOGD(TAG, "MQTT Publish status: %d", status);
    if (status == MQTTSuccess)
    {
        sendState(MQTT_CONNECTED);
    }
    
    return status == MQTTSuccess;
}

bool mqtt_publish(const char *topic, const char *payload, MQTTQoS_t qos)
{

    if (!mqtt_connected())
    {
        return false;
    }

    MQTTPublishInfo_t pub =
        {
            .qos = qos,
            .pTopicName = topic,
            .topicNameLength = strlen(topic),
            .pPayload = payload,
            .payloadLength = strlen(payload)};

    /* code */
    MQTTStatus_t status = MQTT_Publish(&mqttContext, &pub, MQTT_GetPacketId(&mqttContext));
    LOGD(TAG, "MQTT Publish status: %d", status);
    return status == MQTTSuccess;
}

void close_conn()
{
    LOGW(TAG, "Closing connection\n");
    sendState(WIFI_CONNECTED);

    if (mqtt_connected())
    {
        MQTT_Disconnect(&mqttContext);
    }
    if (ssl != NULL)
    {
        wolfSSL_shutdown(ssl);
        wolfSSL_free(ssl);
        wolfSSL_Cleanup();
        tls_connected = false;
        ssl = NULL;
    }

    if (conn != NULL)
    {

        netbuf_delete(buf);
        netconn_close(conn);
        netconn_delete(conn);

        cur_data_pos = 0;
        cur_data_len = 0;
        data = NULL;
        buf = NULL;
        tcp_connected = false;

        conn = NULL;
    }
    // wifi_deinit(NULL, NULL);
}

bool tcp_is_connected()
{
    return wifi_connected() && conn != NULL && tcp_connected;
}

bool mqtt_connected()
{

    return tcp_is_connected() && mqttContext.connectStatus == MQTTConnected;
}

bool mqtt_loop()
{
    if (!mqtt_connected())
    {
        LOGW(TAG, "MQTT Not Connected");
        return false;
    }
    MQTTStatus_t status = MQTT_ProcessLoop(&mqttContext);
    LOGD(TAG, "MQTT Process Loop status: %d", status);
    return status == MQTTSuccess;
}
