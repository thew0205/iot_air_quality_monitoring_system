#include "lwip/api.h"

#include <string.h>
#include <time.h>

#include "pico/stdlib.h"
#include "pico/cyw43_arch.h"

#include "lwip/pbuf.h"
#include "lwip/tcp.h"

#include "FreeRTOS.h"
#include "task.h"

// #include <wolfssl.h>
/* Important: make sure settings.h appears before any other wolfSSL headers */
#include <wolfssl/wolfcrypt/settings.h>
/* Reminder: settings.h includes user_settings.h
 * For ALL project wolfSSL settings, see:
 * [your path]/Arduino\libraries\wolfSSL\src\user_settings.h   */
#include <wolfssl/ssl.h>
#include <wolfssl/certs_test.h>
#include <wolfssl/wolfcrypt/error-crypt.h>

#define AWS_ENDPOINT "aj8uipcillvb-ats.iot.eu-west-2.amazonaws.com"
#define AWS_PORT 8883

#define CLIENT_ID "TestPCClient01"
#define PUBLISH_TOPIC "test/pcclient"
#define PUBLISH_MSG "Hello from wolfSSL MQTT on PC"

// File paths
#define AWS_ROOT_CA "dart_test_keys/AmazonRootCA1.pem"
#define DEVICE_CERT "dart_test_keys/certificate.crt"
#define PRIVATE_KEY "dart_test_keys/private.key"

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
struct netconn *conn;

// int32_t transport_send(NetworkContext_t *pNetworkContext,
//                        const void *pBuffer,
//                        size_t bytesToSend);

// int32_t transport_recv(NetworkContext_t *pNetworkContext,
//                        void *pBuffer,
//                        size_t bytesToRecv);

// typedef struct NetworkContext
// {
//     struct netconn *conn;
// } NetworkContext_t;

// int32_t transport_send(NetworkContext_t *ctx,
//                        const void *buffer,
//                        size_t bytesToSend)
// {
//     err_t err = netconn_write(ctx->conn, buffer, bytesToSend, NETCONN_COPY);
//     return (err == ERR_OK) ? bytesToSend : -1;
// }
// int32_t transport_recv(NetworkContext_t *ctx,
//                        void *buffer,
//                        size_t bytesToRecv)
// {
//     struct netbuf *buf;
//     void *data;
//     u16_t len;
//     size_t received = 0;

//     // Check if data is available without blocking
//     // Use timeout on the socket

//     err_t err = netconn_recv(ctx->conn, &buf);
//     if (err != ERR_OK)
//     {
//         // No data = return 0 (CoreMQTT behavior)
//         return 0;
//     }

//     do
//     {
//         netbuf_data(buf, &data, &len);
//         size_t copy_len = (len > (bytesToRecv - received)) ? (bytesToRecv - received) : len;

//         memcpy((uint8_t *)buffer + received, data, copy_len);
//         received += copy_len;

//         if (received >= bytesToRecv)
//             break;

//     } while (netbuf_next(buf) >= 0);

//     netbuf_delete(buf);
//     return received;
// }

/*****************************************************************************/
/* EthernetSend() to send a message string.                                  */
/*****************************************************************************/
int EthernetSend(WOLFSSL *ssl, char *message, int sz, void *ctx)
{
    err_t err = netconn_write(conn, message, sz, NETCONN_COPY);
    return (err == ERR_OK) ? sz : -1;
}

/*****************************************************************************/
/* EthernetReceive() to receive a reply string.                              */
/*****************************************************************************/

struct ReceiveBuffer
{
    uint16_t buffer_size;
    uint16_t pos;
    uint8_t buffer[20000];
};
struct ReceiveBuffer recv_buffer;
int EthernetReceive(WOLFSSL *ssl, char *reply, int sz, void *ctx)
{
    int ret = 0;
    struct netbuf *buf;
    void *data;
    u16_t len;
    size_t received = 0;

    // printf("MEMP TCP_SEG free: %d\n", lwip_stats.memp[MEMP_TCP_SEG].avail);
    // printf("PBUF free: %d\n", lwip_stats.tcp.[MEMP_PBUF].avail);

    err_t err = netconn_recv(conn, &buf);
    if (err == ERR_OK)
    {

        do
        {
            netbuf_data(buf, &data, &len);
            memcpy(&recv_buffer.buffer[recv_buffer.buffer_size], data, len);
            recv_buffer.buffer_size += len;
        } while (netbuf_next(buf) >= 0);
    }
    int available = recv_buffer.buffer_size - recv_buffer.pos;
    sz = sz > available ? available : sz;
    memcpy(reply, &recv_buffer.buffer[recv_buffer.pos], sz);
    recv_buffer.pos += sz;
    // while (wifiClient.available() > 0 && ret < sz)
    // {
    //     reply[ret++] = wifiClient.read();
    // }
    netbuf_delete(buf);

    return sz;
}

// uint32_t get_time_ms()
// {
//     return (time_us_64() / 1000);
// }
// void MQTTEventCallbackFun(struct MQTTContext *pContext,
//                           struct MQTTPacketInfo *pPacketInfo,
//                           struct MQTTDeserializedInfo *pDeserializedInfo)
// {
//     printf("MQTTEventCallbackFun: %s\n", pDeserializedInfo->pPublishInfo->pPayload);
// }
static uint8_t mqtt_buffer[1024 * 10];
ip_addr_t mqtt_server_address;

void start_tcp_client()
{
    recv_buffer.buffer_size = 0;
    recv_buffer.pos = 0;
    struct netbuf *buf;
    void *data;
    u16_t len;
    static WOLFSSL_CTX *ctx = NULL;
    static WOLFSSL *ssl = NULL;
    // wolfSSL_Debugging_ON();

    int err = 0;

    int ret = wolfSSL_Init();
    if (ret == WOLFSSL_SUCCESS)
    {
        printf("Successfully called wolfSSL_Init\n");
    }
    else
    {
        printf("ERROR: wolfSSL_Init failed\n");
    }
    WOLFSSL_METHOD *method;

    method = wolfSSLv23_client_method();
    if (method == NULL)
    {
        printf("unable to get wolfssl client method\n");
        panic("");
    }
    ctx = wolfSSL_CTX_new(method);
    if (ctx == NULL)
    {
        printf("unable to get ctx\n");
        panic("");
    }

    /* Use built-in validation, No verification callback function: */
    wolfSSL_CTX_set_verify(ctx, SSL_VERIFY_PEER, 0);

    /* Certificate */
    printf("Initializing certificates...\n");
    ret = wolfSSL_CTX_use_certificate_buffer(ctx,
                                             (const unsigned char *)DEVICE_CERT_STRING,
                                             strlen(DEVICE_CERT_STRING),
                                             WOLFSSL_FILETYPE_PEM);
    if (ret == WOLFSSL_SUCCESS)
    {
        printf("Success: use certificate: ");
        printf(CTX_SERVER_CERT);
    }
    else
    {
        printf("Error: wolfSSL_CTX_use_certificate_buffer failed: ");
        // wc_ErrorString(ret, wc_error_message);
        // Serial.println(wc_error_message);
        panic("");
    }

    /* Setup private client key */
    ret = wolfSSL_CTX_use_PrivateKey_buffer(ctx,
                                            (const unsigned char *)DEVICE_KEY_STRING,
                                            strlen(DEVICE_KEY_STRING),
                                            WOLFSSL_FILETYPE_PEM);
    if (ret == WOLFSSL_SUCCESS)
    {
        printf("Success: use private key buffer: ");
        // printf(CTX_SERVER_KEY));
    }
    else
    {
        printf("Error: wolfSSL_CTX_use_PrivateKey_buffer failed: \n");
        // wc_ErrorString(ret, wc_error_message);
        // Serial.println(wc_error_message);
        panic("");
    }

    ret = wolfSSL_CTX_load_verify_buffer(ctx,
                                         (const unsigned char *)AWS_ROOT_CA_STRING,
                                         strlen(AWS_ROOT_CA_STRING),
                                         WOLFSSL_FILETYPE_PEM);
    if (ret == WOLFSSL_SUCCESS)
    {
        printf("Success: load_verify CTX_CA_CERT\n");
    }
    else
    {
        printf("Error: wolfSSL_CTX_load_verify_buffer failed: \n");
        // wc_ErrorString(ret, wc_error_message);
        // Serial.println(wc_error_message);
        panic("");
    }

    /* Initialize wolfSSL using callback functions. */
    wolfSSL_SetIOSend(ctx, EthernetSend);
    wolfSSL_SetIORecv(ctx, EthernetReceive);

    ssl = wolfSSL_new(ctx);
    bool session_present = false;
    conn = netconn_new(NETCONN_TCP);
    conn->recv_timeout = 5; // 10ms
    // conn->send_timeout = 5000;

    if (netconn_gethostbyname(AWS_ENDPOINT, &mqtt_server_address) == ERR_OK)
    {
        printf("Dns successfully\n");
    }
    else
    {

        printf("Dns fail\n");
        panic("");
    }
    if (netconn_connect(conn, &mqtt_server_address, AWS_PORT) == ERR_OK)
    {
        printf("Connected successfully\n");
    }
    else
    {
        printf("Connection fail\n");
    }
    // const char *msg = "GET / HTTP/1.0\r\n\r\n";
    // netconn_write(conn, msg, strlen(msg), NETCONN_COPY);
    printf("Connecting to wolfSSL TLS Secure Server...\n");
    do
    {
        err = 0; /* reset error */
        printf("wolfSSL_connect ...\n");
        ret = wolfSSL_connect(ssl);
        printf("wolfSSL_connect return result =%d\n", ret);
        if ((ret != WOLFSSL_SUCCESS) && (ret != WC_PENDING_E))
        {
            printf("Failed connection, checking error.\n");
            // err = error_check_ssl(ssl, ret, true,
            // F("Create WOLFSSL object from ctx"));
        }
        else
        {
            printf(".");
        }
    } while (err == WC_PENDING_E);

    printf("Connected!\n");
    printf("SSL version is %d\n", wolfSSL_get_version(ssl));

    const char *cipherName = wolfSSL_get_cipher(ssl);
    printf("SSL cipher suite is %s\n", cipherName);

    static const unsigned char mqtt_connect_packet[] = {
        0x10, 0x13,
        0x00, 0x04, 'M', 'Q', 'T', 'T',
        0x04, 0x02,
        0x00, 0x3C,
        0x00, 0x07, 'p', 'y', '_', 't', 'e', 's', 't'};

    const unsigned char mqtt_publish_p[] = {
        0x30,                                                           // PUBLISH, QoS0
        0x1B,                                                           // Remaining length (26)
        0x00, 0x0C,                                                     // Topic length = 12
        '/', 't', 'e', 's', 't', '/', 't', 'o', 'p', 'i', 'c', '/',     // topic
        'H', 'e', 'l', 'l', 'o', ' ', 'A', 'W', 'S', 'u', '+', '+', 'T' // payload
    };

    ret = wolfSSL_write(ssl, mqtt_connect_packet, sizeof(mqtt_connect_packet));
    if (ret <= 0)
    {
        printf("MQTT CONNECT send failed\n");
        // goto exit;
    }

    printf("Sent MQTT CONNECT (%d bytes)\n", ret);

    /* Optionally read MQTT CONNACK */
    unsigned char buffer[320];
    ret = wolfSSL_read(ssl, buffer, sizeof(buffer));
    if (ret > 0)
    {
        printf("Received %d bytes: ", ret);
        for (int i = 0; i < ret; i++)
            printf("%02X ", buffer[i]);
        printf("\n");
    }

    if (wolfSSL_write(ssl, mqtt_publish_p, sizeof(mqtt_publish_p)) <= 0)
    {
        printf("SSL_write (MQTT PUBLISH) failed");
        // goto exit;
    }
    printf("Sent MQTT PUBLISH to /test/topic/\n");
    int bytes;
    do
    {
        bytes = wolfSSL_read(ssl, buffer, sizeof(buffer));
        printf("Received %d bytes (MQTT CONNACK):\n", bytes);
        for (int i = 0; i < bytes; ++i)
            printf("%02X ", buffer[i]);
        printf("\n");
    } while (bytes > 0);
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
                10000, // stack words
                NULL,
                tskIDLE_PRIORITY + 1,
                NULL);

    // Start FreeRTOS scheduler
    vTaskStartScheduler();

    // We should never get here
    for (;;)
        tight_loop_contents();
}

// static WOLFSSL_CTX *ctx = NULL;
// static WOLFSSL *ssl = NULL;

// /*****************************************************************************/
// /* EthernetSend() to send a message string.                                  */
// /*****************************************************************************/
// int EthernetSend(WOLFSSL *ssl, char *message, int sz, void *ctx)
// {
//     int sent = 0;
//     (void)ssl;
//     (void)ctx;

//     sent = wifiClient.write((byte *)message, sz);
//     return sent;
// }

// /*****************************************************************************/
// /* EthernetReceive() to receive a reply string.                              */
// /*****************************************************************************/
// int EthernetReceive(WOLFSSL *ssl, char *reply, int sz, void *ctx)
// {
//     int ret = 0;
//     (void)ssl;
//     (void)ctx;

//     while (wifiClient.available() > 0 && ret < sz)
//     {
//         reply[ret++] = wifiClient.read();
//     }
//     return ret;
// }
// void reconnect()
// {
//     // Loop until we're reconnected
//     while (!pubClient.connected())
//     {
//         Serial.print("Attempting MQTT connection...");
//         // Attempt to connect
//         if (pubClient.connect("arent"))
//         {
//             Serial.println("connected");
//             // Once connected, publish an announcement...
//             pubClient.publish("test/topic", "hello worldUnable to connect to network, rebooting in 10 seconds...Unable to connect to network, rebooting in 10 seconds...Unable to connect to network, rebooting in 10 seconds...");
//             // ... and resubscribe
//             pubClient.subscribe("inTopic");
//         }
//         else
//         {
//             Serial.print("failed, rc=");
//             Serial.print(pubClient.state());
//             Serial.println(" try again in 5 seconds");
//             // Wait 5 seconds before retrying
//             delay(5000);
//         }
//     }
// }

// void setup()
// {
//     Serial.begin(57600);

//     // pubClient.setServer(serve, 1883);
//     // pubClient.setCallback(callback);

//     // Ethernet.begin(mac, ip);

//     multi.addAP(ssid, password);

//     if (multi.run() != WL_CONNECTED)
//     {
//         Serial.println("Unable to connect to network, rebooting in 10 seconds...");
//         delay(10000);
//         rp2040.reboot();
//     }
//     Serial.println("");
//     Serial.println("WiFi connected");
//     Serial.println("IP address: ");
//     Serial.println(WiFi.localIP());
//     // Allow the hardware to sort itself out
//     delay(150);

//     wolfSSL_Debugging_ON();
//     int err = 0;

//     int ret = wolfSSL_Init();
//     if (ret == WOLFSSL_SUCCESS)
//     {
//         Serial.println("Successfully called wolfSSL_Init");
//     }
//     else
//     {
//         Serial.println("ERROR: wolfSSL_Init failed");
//     }
//     WOLFSSL_METHOD *method;

//     method = wolfSSLv23_client_method();
//     if (method == NULL)
//     {
//         Serial.println(F("unable to get wolfssl client method"));
//         fail_wait();
//     }
//     ctx = wolfSSL_CTX_new(method);
//     if (ctx == NULL)
//     {
//         Serial.println(F("unable to get ctx"));
//         fail_wait();
//     }

//     /* Use built-in validation, No verification callback function: */
//     wolfSSL_CTX_set_verify(ctx, SSL_VERIFY_PEER, 0);

//     /* Certificate */
//     Serial.println("Initializing certificates...");
//     ret = wolfSSL_CTX_use_certificate_buffer(ctx,
//                                              (const unsigned char *)DEVICE_CERT_STRING,
//                                              strlen(DEVICE_CERT_STRING),
//                                              WOLFSSL_FILETYPE_PEM);
//     if (ret == WOLFSSL_SUCCESS)
//     {
//         Serial.print("Success: use certificate: ");
//         Serial.println(xstr(CTX_SERVER_CERT));
//     }
//     else
//     {
//         Serial.println(F("Error: wolfSSL_CTX_use_certificate_buffer failed: "));
//         // wc_ErrorString(ret, wc_error_message);
//         // Serial.println(wc_error_message);
//         fail_wait();
//     }

//     /* Setup private client key */
//     ret = wolfSSL_CTX_use_PrivateKey_buffer(ctx,
//                                             (const unsigned char *)DEVICE_KEY_STRING,
//                                             strlen(DEVICE_KEY_STRING),
//                                             WOLFSSL_FILETYPE_PEM);
//     if (ret == WOLFSSL_SUCCESS)
//     {
//         Serial.print("Success: use private key buffer: ");
//         Serial.println(xstr(CTX_SERVER_KEY));
//     }
//     else
//     {
//         Serial.println(F("Error: wolfSSL_CTX_use_PrivateKey_buffer failed: "));
//         // wc_ErrorString(ret, wc_error_message);
//         // Serial.println(wc_error_message);
//         fail_wait();
//     }

//     ret = wolfSSL_CTX_load_verify_buffer(ctx,
//                                          (const unsigned char *)AWS_ROOT_CA_STRING,
//                                          strlen(AWS_ROOT_CA_STRING),
//                                          WOLFSSL_FILETYPE_PEM);
//     if (ret == WOLFSSL_SUCCESS)
//     {
//         Serial.println(F("Success: load_verify CTX_CA_CERT"));
//     }
//     else
//     {
//         Serial.println(F("Error: wolfSSL_CTX_load_verify_buffer failed: "));
//         // wc_ErrorString(ret, wc_error_message);
//         // Serial.println(wc_error_message);
//         fail_wait();
//     }

//     /* Initialize wolfSSL using callback functions. */
//     wolfSSL_SetIOSend(ctx, EthernetSend);
//     wolfSSL_SetIORecv(ctx, EthernetReceive);

//     ssl = wolfSSL_new(ctx);

//     if (wifiClient.connect(AWS_ENDPOINT, AWS_PORT))
//     {
//         Serial.println("Connected successfully");
//     }
//     else
//     {
//         Serial.println("Connection fail");
//     }

//     Serial.print(F("Connecting to wolfSSL TLS Secure Server..."));
//     do
//     {
//         err = 0; /* reset error */
//         Serial.println(F("wolfSSL_connect ..."));
//         ret = wolfSSL_connect(ssl);
//         Serial.print("wolfSSL_connect return result =");
//         Serial.println(ret);
//         if ((ret != WOLFSSL_SUCCESS) && (ret != WC_PENDING_E))
//         {
//             Serial.println(F("Failed connection, checking error."));
//             // err = error_check_ssl(ssl, ret, true,
//             // F("Create WOLFSSL object from ctx"));
//             Serial.print("err =");
//             Serial.println(err);
//         }
//         else
//         {
//             Serial.print(".");
//         }
//     } while (err == WC_PENDING_E);

//     Serial.println();
//     Serial.println(F("Connected!"));
//     Serial.print(F("SSL version is "));
//     Serial.println(wolfSSL_get_version(ssl));

//     auto cipherName = wolfSSL_get_cipher(ssl);
//     Serial.print(F("SSL cipher suite is "));
//     Serial.println(cipherName);

//     static const unsigned char mqtt_connect_packet[] = {
//         0x10, 0x13,
//         0x00, 0x04, 'M', 'Q', 'T', 'T',
//         0x04, 0x02,
//         0x00, 0x3C,
//         0x00, 0x07, 'p', 'y', '_', 't', 'e', 's', 't'};

//     const unsigned char mqtt_publish_p[] = {
//         0x30,                                                           // PUBLISH, QoS0
//         0x1B,                                                           // Remaining length (26)
//         0x00, 0x0C,                                                     // Topic length = 12
//         '/', 't', 'e', 's', 't', '/', 't', 'o', 'p', 'i', 'c', '/',     // topic
//         'H', 'e', 'l', 'l', 'o', ' ', 'A', 'W', 'S', ' ', '_', '_', 'T' // payload
//     };

//     ret = wolfSSL_write(ssl, mqtt_connect_packet, sizeof(mqtt_connect_packet));
//     if (ret <= 0)
//     {
//         Serial.printf("MQTT CONNECT send failed");
//         // goto exit;
//     }

//     Serial.printf("Sent MQTT CONNECT (%d bytes)\n", ret);

//     /* Optionally read MQTT CONNACK */
//     unsigned char buf[320];
//     ret = wolfSSL_read(ssl, buf, sizeof(buf));
//     if (ret > 0)
//     {
//         Serial.printf("Received %d bytes: ", ret);
//         for (int i = 0; i < ret; i++)
//             Serial.printf("%02X ", buf[i]);
//         Serial.printf("\n");
//     }

//     if (wolfSSL_write(ssl, mqtt_publish_p, sizeof(mqtt_publish_p)) <= 0)
//     {
//         Serial.printf("SSL_write (MQTT PUBLISH) failed");
//         // goto exit;
//     }
//     Serial.printf("Sent MQTT PUBLISH to /test/topic/\n");
//     int bytes;
//     do
//     {
//         bytes = wolfSSL_read(ssl, buf, sizeof(buf));
//         Serial.printf("Received %d bytes (MQTT CONNACK):\n", bytes);
//         for (int i = 0; i < bytes; ++i)
//             Serial.printf("%02X ", buf[i]);
//         Serial.printf("\n");
//     } while (bytes > 0);
// }

// void loop()
// {
//     if (!pubClient.connected())
//     {
//         reconnect();
//     }
//     else
//     {
//         // pubClient.publish("test/topic", "hello worldUnable to connect to network, rebooting in 10 seconds...Unable to connect to network, rebooting in 10 seconds...Unable to connect to network, rebooting in 10 seconds...");
//     }
//     pubClient.loop();
// }
