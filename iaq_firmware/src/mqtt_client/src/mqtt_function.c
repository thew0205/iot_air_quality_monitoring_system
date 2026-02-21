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
#include "iaq_firmware.h"
#define AWS_ENDPOINT "aj8uipcillvb-ats.iot.eu-west-2.amazonaws.com"
#define AWS_PORT 8883

const char DEVICE_CERT_STRING[] =
    "-----BEGIN CERTIFICATE-----\n\
MIIDWTCCAkGgAwIBAgIUXuXbQFK4Mu9LorIzXT0tcadS5rUwDQYJKoZIhvcNAQEL\n\
BQAwTTFLMEkGA1UECwxCQW1hem9uIFdlYiBTZXJ2aWNlcyBPPUFtYXpvbi5jb20g\n\
SW5jLiBMPVNlYXR0bGUgU1Q9V2FzaGluZ3RvbiBDPVVTMB4XDTI1MTAzMDA1NTcz\n\
M1oXDTQ5MTIzMTIzNTk1OVowHjEcMBoGA1UEAwwTQVdTIElvVCBDZXJ0aWZpY2F0\n\
ZTCCASIwDQYJKoZIhvcNAQEBBQADggEPADCCAQoCggEBAKBhcwuY7zeEuEilxoMP\n\
fWizpfwte/rlEGbD/aKJxyfX8f/+57OsxWalnhCvdPkz0dqGvJDLlXJbyiT0P410\n\
IDngY6SIq139G9yVbRkfLKS4wAQRegec9S5rM9zGDArp/ZX9VmLpngau60T+bu2N\n\
e/FR1xYETaTvnFVEHOKYJ4fHW04zvy2dPvVbsTkIuwavQaI+4agLKUSkXDjjkais\n\
EqU+ZiO6JOogpe4qfuNa0vXCQRjfYfnvzqH0snKToFCAr3fyEKFIuImoVxI3+SCP\n\
kC6xaeNYM4DlCPU0mAdtS8xg3idezP/lFs0mTovMyzBYGfVCrPegKUPAJtID4/Ru\n\
RHcCAwEAAaNgMF4wHwYDVR0jBBgwFoAUV0+lhrkOkkthsXauedFon0pd7ZkwHQYD\n\
VR0OBBYEFEKQ6tUAEHRicwhCnomFlWmzy0uTMAwGA1UdEwEB/wQCMAAwDgYDVR0P\n\
AQH/BAQDAgeAMA0GCSqGSIb3DQEBCwUAA4IBAQA0nwKx+rlxlc1zawDrWLaEbuVJ\n\
wakuWKHLiO0hL0LnER4kYR6Ox0C9qTf9uEQ4MvW7IigSC8RnJFUFdfkVLX3M6AFM\n\
DsOI2QOT8LtaySwhRN14NC/pRY8MLvB6NtxtFqneG2k00LpB8M2cBZ8oIKIOp/jx\n\
5DIlIFPLExtcXc0ph7TikSkguM6VjNnWOGGJSFvAgpea+Ioxk3ECvmZvczKTHF3T\n\
qffSzheotq5bny4Rsv8ZYJY7i/ukdz+K/I+RMi6cPcMZXQQQt7yQW+FK/r+xoAv4\n\
BcivU51sKr7iuduE/zaMbDIK8BG0SUhd1cE64TXfp6GIZ/BZOcdlx/ia0fUm\n\
-----END CERTIFICATE-----\n\
";
const char DEVICE_KEY_STRING[] =
    "-----BEGIN RSA PRIVATE KEY-----\n\
MIIEowIBAAKCAQEAoGFzC5jvN4S4SKXGgw99aLOl/C17+uUQZsP9oonHJ9fx//7n\n\
s6zFZqWeEK90+TPR2oa8kMuVclvKJPQ/jXQgOeBjpIirXf0b3JVtGR8spLjABBF6\n\
B5z1Lmsz3MYMCun9lf1WYumeBq7rRP5u7Y178VHXFgRNpO+cVUQc4pgnh8dbTjO/\n\
LZ0+9VuxOQi7Bq9Boj7hqAspRKRcOOORqKwSpT5mI7ok6iCl7ip+41rS9cJBGN9h\n\
+e/OofSycpOgUICvd/IQoUi4iahXEjf5II+QLrFp41gzgOUI9TSYB21LzGDeJ17M\n\
/+UWzSZOi8zLMFgZ9UKs96ApQ8Am0gPj9G5EdwIDAQABAoIBAB19DXwqaIGaw4+H\n\
+5IJfRONRI2zh0JAb92VV8NU9etbh0F8ivm8YKdY+FE1EZrobkNx1qYA6L+YJPja\n\
d/z4RxydJ3UPNAMaEM9kIiUmWD0JQzT8SaWwtFwOlgCX4Yr4LwovBH03Wzj7XILZ\n\
DWhcRVe/pn6D2Arli6cZ2P3I4CvUr1lQFhFl0C+4oZcKD+KEG6yTgwL3mZ0FL032\n\
HqFcnGnm6VNLalcGzmsIrYptNhXFs0eSRqCPwM2XxoLN+t/Bb79+e13RuIrkTJ77\n\
dW1mY0gUulSyM8quRg4Pn66mrq0NiWWIhUN0cwfMEItBlxOgACXYlEX/ytt2Tvrc\n\
2XpSHmECgYEA1LAtrkXYntLE3UZQc8FWh1DQ8uYktrLnibyxHI++9Gmb3CxwdtIf\n\
GRB38WDiYwyY+oE/kwneFNZojuJDEbR42s4Cu8kky4uky4rZ7B+50biewI2fPNc5\n\
G63hIIKp6hQgHqG4BECxhuWiJazu9aNEuMtGhj+0TwRu9JvHhAm1gakCgYEAwQpi\n\
Xd1yE7d/eSegdu6VOp9Jp79AucTLDKEMPyWE8DSuUzFFOfXR7mISZaxkSUROWMsW\n\
sKrYdpclOhInDTET3abe6YJUp1a23JJe2XHyPXPn3Zv9MQZXDziBBW2ZPw3Hr4Xz\n\
YEPx8qUfyrpdd0iEXUIah1678tJ5+g7k4ZzjqR8CgYACCb4mUV0GCJcCV84ljdC6\n\
czfNXA82APjfBQPddjMqPGjV70cUnGbX4qSDl54TvOcqX1qwmgOax4Mw+1IRNV2D\n\
Gz5DdmX33Vqy6J21+YfaTp6W0cSY3KcbcMsbeKUQLSkxofMVd1k8Ja8sFUAk8m+S\n\
JcCDQNEFgNFfXq7GwSEQIQKBgQCmjG4H7AoDRtsdTXodgoSP2mQy7UhLvqUtFTm6\n\
tYU3tUv+DNFcS2E/Wa/jttOjfuJVq4q9C/nMf2Un8AdvUzt/jfxe3Qpqgd8s+MAK\n\
vUuxh3Gx+mZ9SKoOQMLYSxHfm+T/5ALxuE1j+uEuCJKhWpiQ2XqH9s0TmFKS0wzY\n\
NoMZIwKBgHSR/44zE5ONUBSfPetyLfJX1HvqF3EMachOqgBdsLahHRcSXD4YNKoG\n\
Aqrrd9acqC5BARnIJAhr1i/2TTFpv7dkhMw7DEaqPsR3Yr+/gg5CGSoWreWHHDfV\n\
+tPhXP8JWlY/z6dP9eYIl6z+ACFxDiCTGYRKArG2qXumy53zxFxZ\n\
-----END RSA PRIVATE KEY-----\n\
";

const char AWS_ROOT_CA_STRING[] =
    "-----BEGIN CERTIFICATE-----\n\
MIIDQTCCAimgAwIBAgITBmyfz5m/jAo54vB4ikPmljZbyjANBgkqhkiG9w0BAQsF\n\
ADA5MQswCQYDVQQGEwJVUzEPMA0GA1UEChMGQW1hem9uMRkwFwYDVQQDExBBbWF6\n\
b24gUm9vdCBDQSAxMB4XDTE1MDUyNjAwMDAwMFoXDTM4MDExNzAwMDAwMFowOTEL\n\
MAkGA1UEBhMCVVMxDzANBgNVBAoTBkFtYXpvbjEZMBcGA1UEAxMQQW1hem9uIFJv\n\
b3QgQ0EgMTCCASIwDQYJKoZIhvcNAQEBBQADggEPADCCAQoCggEBALJ4gHHKeNXj\n\
ca9HgFB0fW7Y14h29Jlo91ghYPl0hAEvrAIthtOgQ3pOsqTQNroBvo3bSMgHFzZM\n\
9O6II8c+6zf1tRn4SWiw3te5djgdYZ6k/oI2peVKVuRF4fn9tBb6dNqcmzU5L/qw\n\
IFAGbHrQgLKm+a/sRxmPUDgH3KKHOVj4utWp+UhnMJbulHheb4mjUcAwhmahRWa6\n\
VOujw5H5SNz/0egwLX0tdHA114gk957EWW67c4cX8jJGKLhD+rcdqsq08p8kDi1L\n\
93FcXmn/6pUCyziKrlA4b9v7LWIbxcceVOF34GfID5yHI9Y/QCB/IIDEgEw+OyQm\n\
jgSubJrIqg0CAwEAAaNCMEAwDwYDVR0TAQH/BAUwAwEB/zAOBgNVHQ8BAf8EBAMC\n\
AYYwHQYDVR0OBBYEFIQYzIU07LwMlJQuCFmcx7IQTgoIMA0GCSqGSIb3DQEBCwUA\n\
A4IBAQCY8jdaQZChGsV2USggNiMOruYou6r4lK5IpDB/G/wkjUu0yKGX9rbxenDI\n\
U5PMCCjjmCXPI6T53iHTfIUJrU6adTrCC2qJeHZERxhlbI1Bjjt/msv0tadQ1wUs\n\
N+gDS63pYaACbvXy8MWy7Vu33PqUXHeeE6V/Uq2V8viTO96LXFvKWlJbYK8U90vv\n\
o/ufQJVtMVT8QtPHRh8jrdkPSHCa2XV4cdFyQzR1bldZwgJcJmApzyMZFo6IQ6XU\n\
5MsI+yMRQ+hDKXJioaldXgjUkK642M4UwtBV8ob2xJNDd2ZhwLnoQdeXeGADbkpy\n\
rqXRfboQnoZsG4q5WTP468SQvvG5\n\
-----END CERTIFICATE-----";

#define TAG "MQTT"
void wifi_init(const char *ssid, const char *password)
{

#if 0
    if (cyw43_arch_init())
    {
        PANIC("Failed to inizialize CYW43");
    }
#endif
    cyw43_arch_enable_sta_mode();
    if (cyw43_arch_wifi_connect_timeout_ms(ssid, password, CYW43_AUTH_WPA2_AES_PSK, 30000))
    {
        PANIC("Failed to connect");
    }
}

struct netconn *conn;
static struct netbuf *buf = NULL;
static u16_t cur_data_len = 0;
static u16_t cur_data_pos = 0;
static void *data = NULL;
static bool tcp_connected = false;
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
    if (err == WOLFSSL_ERROR_WANT_READ || err == WOLFSSL_ERROR_WANT_WRITE)
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
    LOGE(TAG, "wolfSSL_read error: %d\n", err);

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
            tcp_connected = false;
            sz = WOLFSSL_ERROR_ZERO_RETURN;
        }
        else
        {
            sz = WOLFSSL_ERROR_WANT_WRITE;
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
                tcp_connected = false;
                close_conn();

                return -WOLFSSL_ERROR_ZERO_RETURN;
            }

            LOGE(TAG, "netconn_recv from network error: %d\n", err);
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

        if (current_available_data <= 0)
        {
            // EthernetAvailable negate the error code, so we need to negate it back to return the correct error code for wolfSSL
            return -current_available_data;
        }
        int copy_len = current_available_data > sz ? sz : current_available_data;

        memcpy(&reply[received], &((uint8_t *)data)[cur_data_pos], copy_len);
        received += copy_len;
        cur_data_pos += copy_len;
        sz -= copy_len;
    }
    return received;
}

void init_conn()
{
    conn = netconn_new(NETCONN_TCP);
    conn->recv_timeout = 5; // 10ms
}

bool tcp_conn()
{
    LOGD(TAG, "Connecting to AWS IoT Core...\n");

    ip_addr_t mqtt_server_address;

    bool success = true;
    if (success && netconn_gethostbyname(AWS_ENDPOINT, &mqtt_server_address) == ERR_OK)
    {
        success = true;
        LOGI(TAG, "Dns successfully\n");
    }
    else
    {

        success = false;
        LOGI(TAG, "Dns fail\n");
        PANIC("Dns fail");
    }
    if (success && netconn_connect(conn, &mqtt_server_address, AWS_PORT) == ERR_OK)
    {
        success = true;
        LOGI(TAG, "Connected successfully\n");
    }
    else
    {
        success = false;
        LOGI(TAG, "Connection fail\n");
    }
    return success;
}

void init_tls()
{
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
    return err == 0;
}

bool mqtt_connect()
{

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
        .pClientIdentifier = "client_id",
        .clientIdentifierLength = strlen("client_id")};

    status = MQTT_Connect(&mqttContext,
                          &connectInfo,
                          NULL,
                          1000, // timeout
                          &session_present);
    LOGD(TAG, "MQTT Publish status: %d", status);
    return status == MQTTSuccess;
}

bool mqtt_publish(const char *topic, const char *payload, MQTTQoS_t qos)
{

    if (!tcp_is_connected())
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

    if (mqtt_connected())
    {
        MQTT_Disconnect(&mqttContext);
    }
    if (ssl != NULL)
    {
        wolfSSL_shutdown(ssl);
        wolfSSL_free(ssl);
        wolfSSL_Cleanup();

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
}

bool tcp_is_connected()
{
    return conn != NULL && tcp_connected;
}

bool mqtt_connected()
{

    return tcp_is_connected() && mqttContext.connectStatus == MQTTConnected;
}

bool mqtt_loop()
{
    if (!mqtt_connected())
    {
        LOGD(TAG, "MQTT Not Connected");
        return false;
    }
    MQTTStatus_t status = MQTT_ProcessLoop(&mqttContext);
    LOGD(TAG, "MQTT Process Loop status: %d", status);
    return status == MQTTSuccess;
}
