#ifndef SENSORS_H
#define SENSORS_H

#include <Arduino.h>
#include <DHTesp.h>
#include "Display.h"

#define DHTPIN 12
#define DHTTYPE DHT22

#define LDRPIN 34

class DHTSensor {
public:
    DHTSensor(); // constructor
    void initialize(); // initialize the sensor
    void checkTempHum(Display &oledDisplay); // read data from the sensor
    TempAndHumidity data;

private:
    DHTesp dhtSensor; // DHT sensor object
};

class LDR {

public:
    LDR(); // constructor
    
    float GAMMA = 0.75; // should update from dashboard
    float servoAngle; // angle of the servo motor
    float avgTime; // minutes to average
    float interval; // time in sec between readings

    void initialize(); // initialize the LDR
    float readLDR(); // read data from the LDR
    float updateLDR(); // calculate lux value
    void setInterval(float interval); // set the interval for reading LDR
    void setAvgTime(float avgTime); // set the time to average
    void setGamma(float gamma); // set the gamma value
    float getCurrentAverage(); // get the current average lux value

private:
    float* ldrReadings; // Dynamic array for readings
    int avgWindow; // number of readings to average
    float currentAverage; // current average lux value
    int readingIndex;
    unsigned long lastReadingTime;
    static constexpr  float RL10 = 50;

    // Constants for ESP32 (12-bit ADC, 3.3V reference)
    const float MAX_ADC = 4095.0f;
    const float VREF = 3.3f;
    const float MAX_EXPECTED_LUX = 1000.0f;

    void recalculateAvgWindow(); // recalculate the average window size
    void resetReadings(); // reset the readings array
};

#endif