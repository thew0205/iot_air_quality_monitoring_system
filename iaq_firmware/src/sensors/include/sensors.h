/**
 * @file sensors.h
 * @brief Header file for WiFi functionality.
 */

#pragma once


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

    //MQ sensors readings
    float h2s_voltage;  //MQ136
    float sno2_voltage; //MQ137

    bool valid;
} SensorData;

void sensors_init();
SensorData sensors_read_all();
