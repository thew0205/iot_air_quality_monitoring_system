#include "sensors.h"

#include <cstdio>

#include "platforms/adc_interface.h"
#include "pico/stdlib.h"
#include "hardware/uart.h"
#include "hardware/gpio.h"

// UART set-up for ZPHS01B
#define UART_ID uart0
#define UART_TX_PIN 16
#define UART_RX_PIN 17
#define BAUD_RATE 9600

static void send_request();
static size_t read_response(uint8_t *buffer, size_t max_len);

// ADC helpers
static const float conversion_factor = 3.3f / (1 << 12);

void sensors_init()
{
    // UART for the ZPHSO1B
    uart_init(UART_ID, BAUD_RATE);
    gpio_set_function(UART_TX_PIN, GPIO_FUNC_UART); // TX
    gpio_set_function(UART_RX_PIN, GPIO_FUNC_UART); // RX

    // ADC for the MQ sensors
    init_adc_sensors();

    std::printf("Sensors initialised.\n");
    sleep_ms(18000);
}

SensorData sensors_read_all()
{
    SensorData data = {};
    data.valid = false;

    // Read ZPHS01B
    uint8_t response[64] = {0};
    while (uart_is_readable(UART_ID))
        uart_getc(UART_ID);

    send_request();
    sleep_ms(200); // small delay before reading
    size_t len = read_response(response, sizeof(response));

    if (len >= 26 && response[1] == 0x86 && validate_checksum(response, len))
    {
        data.valid = true;
        data.pm1 = (response[2] << 8) | response[3];
        data.pm25 = (response[4] << 8) | response[5];
        data.pm10 = (response[6] << 8) | response[7];
        data.co2 = (response[8] << 8) | response[9];
        data.voc = response[10];
        data.temp = (((response[11] << 8) | response[12]) - 500) / 10.0f;
        data.hum = ((response[13] << 8) | response[14]) / 10.0f;
        data.ch2o = ((response[15] << 8) | response[16]) * 0.001f;
        data.co = ((response[17] << 8) | response[18]) * 0.1f;
        data.o3 = ((response[19] << 8) | response[20]) * 0.01f;
        data.no2 = ((response[21] << 8) | response[22]) * 0.01f;
    }

    // MQ sensors
    data.h2s_voltage = read_mq_adc(0) * conversion_factor;
    data.sno2_voltage = read_mq_adc(1) * conversion_factor;

    return data;
}

// Command packet to ZPHS01B sensor to fetch data
static const uint8_t REQUEST_CMD[9] = {
    0xFF, 0x01, 0x86, 0x00, 0x00, 0x00, 0x00, 0x00, 0x79};

// Helper function for checking whether the data received has a valid checksum
bool validate_checksum(const uint8_t *data, size_t length)
{
    uint16_t sum = 0;
    for (int i = 1; i < 25; i++)
        sum += data[i];
    uint16_t checksum = (uint8_t)((256 - (sum % 256)) & 0xFF); // as instructed by the datasheet
    return (checksum == data[25]);
}

// Helper function that reads bytes from UART into a buffer, up to max_len bytes, and returns how many bytes were actually read
static size_t read_response(uint8_t *buffer, size_t max_len)
{
    size_t index = 0;
    absolute_time_t timeout = make_timeout_time_ms(1500); // timeframe to run each reads

    // wait for start byte
    while (absolute_time_diff_us(get_absolute_time(), timeout) > 0)
    {
        if (uart_is_readable(UART_ID))
        {
            uint8_t b = uart_getc(UART_ID);
            // waits for start byte 0xFF
            if (b == 0xFF)
            {
                buffer[index++] = b;
                break;
            }
        }
    }
    if (index == 0)
        return 0; // returns 0 if no start byte is found

    // Read remaining bytes
    while (index < max_len && absolute_time_diff_us(get_absolute_time(), timeout) > 0)
    {
        if (uart_is_readable(UART_ID))
            buffer[index++] = uart_getc(UART_ID);
    }
    return index; // returns how many bytes were read and stored into the buffer
}

// Data request over UART (ZPHS01B)
static void send_request()
{
    for (int i = 0; i < 9; i++)
        uart_putc_raw(UART_ID, REQUEST_CMD[i]);
}

// static float read_mq_adc(uint adc_channel) {
//     adc_select_input(adc_channel);
//     uint16_t raw = adc_read();
//     return raw * conversion_factor;
// }

/**void printSensors()
{
    printf("This is a test function for the Sensor program.\n");
}*/
