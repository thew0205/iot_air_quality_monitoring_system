/**
 * @file sensors.h
 * @brief Header file for WiFi functionality.
 */

#pragma once

#include <stdint.h>
#include <stddef.h>

/**
 * @brief Data structure to store readings from all sensors.
 */
typedef struct {
    //ZPHS01B readings
    float pm1;
    float pm25;
    float pm10;
    float co2;
    float voc;
    float temp;
    float hum;
    float ch2o;
    float co;
    float o3;
    float no2;

    float ch2o_ugm3;    //ugm3 equivalent
    float co_ugm3;      //ugm3 equivalent
    float o3_ugm3;      //ugm3 equivalent
    float no2_ugm3;     //ugm3 equivalent


    //MQ sensors readings
    float h2s;      //MQ136
    float nh3;      //MQ137

    float h2s_ugm3;     //ugm3 equivalent
    float nh3_ugm3;     //ugm3 equivalent
    bool valid;
} SensorData;

void sensors_init();
SensorData sensors_read_all();
bool validate_checksum(const uint8_t *data, size_t length);


/**
 * @brief Prints a test message to the console.
 */
// void printTest();
