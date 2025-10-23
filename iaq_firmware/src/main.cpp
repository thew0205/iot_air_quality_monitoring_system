#include <cstdio>
#include "pico/stdlib.h"

#include "wifi/wifi.h"
#include "sensors.h"


int main()
{
     //Initialise standard I/O
    stdio_init_all();
    std::printf("\n=== IoT Air Quality Board ===\n");

    //Initialize hardware modules
    std::printf("Initialising sensors..\n");
    sensors_init();

    std::printf("Initialising WiFi module...\n");

    sleep_ms(2000);
    std::printf("\nSystem Ready.\n\n");

    while (true) {
        //Read all sensor data
        SensorData data = sensors_read_all();

        //Print readings to serial
        if (data.valid) {
            std::printf("......SENSOR READINGS......\n");
            std::printf("PM1.0: %.3f  µg/m³ | PM2.5: %.3f  µg/m³ | PM10:  %.3f  µg/m³ \n", data.pm1, data.pm25, data.pm10);
            std::printf("CO₂: %.3f  ppm | VOC Grade: %.3f \n", data.co2, data.voc);
            std::printf("Temp: %.1f °C | Humidity: %.1f %%\n", data.temp, data.hum);
            std::printf("CH₂O: %.3f mg/m³ | CO: %.1f ppm | O₃: %.2f ppm | NO₂: %.2f ppm\n\n", data.ch2o, data.co, data.o3, data.no2);
            
        } else {
            std::printf("ZPHS01B not detected/Invalid response\n\n");
        }
        std::printf(".....MQ ANALOG SENSORS..........\n");
        std::printf("MQ-H2S (ADC0, GPIO26): %.3f V\n", data.h2s_voltage);
        std::printf("MQ-SNO2 (ADC1, GPIO27): %.3f V\n", data.sno2_voltage);
        std::printf("........................\n\n");


        //Wait for the next cycle
        sleep_ms(3000);
    }

    /*while (1)
        printTest();
    return 0;*/
}