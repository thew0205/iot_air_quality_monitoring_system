#include <cstdio>
#include "pico/stdlib.h"

#include "wifi/wifi.h"
#include "platforms/include/sensors.h"


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
        printf("Loop tick\n");
        SensorData data = sensors_read_all();

        //Print readings to serial
        if (data.valid) {
            printf("ZPHS01B OK\n");
            std::printf("......SENSOR READINGS......\n");
            std::printf("PM1.0: %.3f  µg/m³ | PM2.5: %.3f  µg/m³ | PM10:  %.3f  µg/m³ \n", data.pm1, data.pm25, data.pm10);
            std::printf("CO₂: %.3f  ppm | VOC Grade: %.3f \n", data.co2, data.voc);
            std::printf("Temp: %.1f °C | Humidity: %.1f %%\n", data.temp, data.hum);
            std::printf("CH₂O: %.3f µg/m³ | CO: %.1f µg/m³ | O₃: %.2f µg/m³ | NO₂: %.2f µg/m³\n\n", data.ch2o_ugm3, data.co_ugm3, data.o3_ugm3, data.no2_ugm3);
            
        } else {
            printf("ZPHS01B not detected.\n");
            std::printf("ZPHS01B not detected/Invalid response\n\n");
        }
        std::printf(".....MQ ANALOG SENSORS..........\n");
        std::printf("MQ-H2S (GPIO26): %.3f µg/m³\n", data.h2s_ugm3);
        std::printf("MQ-NH3 (GPIO27): %.3f µg/m³\n", data.nh3_ugm3);
        std::printf("........................\n\n");


        //Wait for the next cycle
        sleep_ms(3000);
    }

    /*while (1)
        printTest();
    return 0;*/
}