#include "../platforms/include/sensors.h"
#include "adc_interface.h"
#include "pico/stdlib.h"
#include "hardware/uart.h"
#include "hardware/gpio.h"

#include <cstdio>
#include <math.h>

#include "iaq_utils/iaq_utils.h"


// UART set-up for ZPHS01B
#define UART_ID uart1
#define UART_TX_PIN 8
#define UART_RX_PIN 9
#define BAUD_RATE 9600

// ADC set-up for MQ
#define MQ136_ADC_PIN 26
#define MQ137_ADC_PIN 27

static void send_request();
static size_t read_response(uint8_t *buffer, size_t max_len);

// Sensor initialization
void sensors_init()
{
    // UART for the ZPHSO1B
    uart_init(UART_ID, BAUD_RATE);
    gpio_set_function(UART_TX_PIN, GPIO_FUNC_UART); // TX
    gpio_set_function(UART_RX_PIN, GPIO_FUNC_UART); // RX
    std::printf("ZPHSO1B sensor initialised.\n");

    //ADC for the MQ sensors
    adc_init();
    init_adc_pin(MQ136_ADC_PIN);
    init_adc_pin (MQ137_ADC_PIN);
    std::printf("MQ sensors initialised.\n");

    CUSTOM_SLEEP_MS(10000);
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
    if (index == 0) return 0; // returns 0 if no start byte is found

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


//======================== ADC CONFIG BEGINS ===============================
//ppm = 10 ^ ((Vrl - 0.7 ) / 0.65) is the formular derived from the graph

#define A 0.7f
#define B 0.65f

//Helper to convert measured voltage(Vrl) to ppm 
static float vrl_to_ppm(float vrl) {
    float exponent = ((vrl - A ) / B); 
    return powf(10.0f, exponent);
}

// -----------------------------------------------------------
float read_mq136_ppm() {
    float vrl = read_adc_voltage(MQ136_ADC_PIN);
    float ppm = vrl_to_ppm(vrl);
    return ppm;
}

// -----------------------------------------------------------
float read_mq137_ppm() {
    float vrl = read_adc_voltage(MQ137_ADC_PIN);
    float ppm = vrl_to_ppm(vrl);
    return ppm;
}

//======================== ADC CONFIG ENDS ===============================



// Temperature-aware ppm → µg/m³ conversion 
static float ppm_to_ugm3(float ppm, float MW, float tempC)
{
    // Convert temperature to Kelvin
    float T = tempC + 273.15f;

    // Gas constant for atm·L / mol·K
    const float R = 0.082057f;

    // molar volume at this temperature (L/mol)
    float molar_volume = R * T;  

    // Convert ppm to µg/m³ using ideal gas law: µg/m³ = ppm * (MW * 1e3) / (R * T)
    return ppm * (MW / molar_volume) * 1000.0f;
}



//========================= Data read =====================================
SensorData sensors_read_all()
{
    SensorData data = {};
    data.valid = false;

    // Read ZPHS01B
    uint8_t response[64] = {0}; //buffer for responding data

    while (uart_is_readable(UART_ID))
        uart_getc(UART_ID);

    send_request();
    CUSTOM_SLEEP_MS(200); // small delay before reading
    size_t len = read_response(response, sizeof(response));

    // Initialize tempC at room temp so it's available for MQ sensor conversions (incase the ZPHS01B read fails)
    float tempC = 25.0f;

    if (len >= 26 && response[1] == 0x86 && validate_checksum(response, len))
    {
        data.valid = true;
        data.pm1 = (response[2] << 8) | response[3];
        data.pm25 = (response[4] << 8) | response[5];
        data.pm10 = (response[6] << 8) | response[7];
        data.co2 = (response[8] << 8) | response[9];
        data.voc = response[10];
        
        data.temp = ((((response[11] << 8) | response[12]) - 500) / 10.0f) + 5.6f; //5.6 is an offset for the temperature reading
        tempC = data.temp; //for the temp-aware convertions to ug/m3
        
        data.hum = ((response[13] << 8) | response[14]);

        data.ch2o = ((response[15] << 8) | response[16]) * 0.001f;
        data.ch2o_ugm3 = (float) data.ch2o * 1000.0f; //convertion to ugm3

        data.co = ((uint16_t)(response[17] << 8) | response[18]) * 0.1f;
        data.co_ugm3 = ppm_to_ugm3(data.co, 28.0f, tempC); //convertion to ugm3
        
        data.o3 = ((uint16_t)(response[19] << 8) | response[20]) * 0.01f;
        data.o3_ugm3 = ppm_to_ugm3(data.o3, 48.0f, tempC); //convertion to ugm3

        data.no2 = ((uint16_t)(response[21] << 8) | response[22]) * 0.01f;
        data.no2_ugm3 = ppm_to_ugm3(data.no2, 46.0f, tempC); //convertion to ugm3
    }

    // MQ sensors
    data.h2s = read_mq136_ppm();
    data.nh3 = read_mq137_ppm();

    // CONVERSION TO ug/m3
    data.h2s_ugm3 = ppm_to_ugm3(data.h2s, 34.08f, tempC); 
    data.nh3_ugm3 = ppm_to_ugm3(data.nh3, 17.03f, tempC);

    return data;
}


/**void printSensors()
{
    printf("This is a test function for the Sensor program.\n");
}*/
