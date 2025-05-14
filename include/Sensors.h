#ifndef SENSORS_H
#define SENSORS_H

#include <Arduino.h>
#include <DHTesp.h>
#include "Display.h"

#define DHTPIN 12
#define DHTTYPE DHT22

class DHTSensor {
public:
    DHTSensor(); // constructor
    void initialize(); // initialize the sensor
    void checkTempHum(Display &oledDisplay); // read data from the sensor

private:
    DHTesp dhtSensor; // DHT sensor object
};

#endif