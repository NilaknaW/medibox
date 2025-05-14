#include "Sensors.h"

DHTSensor::DHTSensor(): dhtSensor() {} // constructor

void DHTSensor::initialize() {
    dhtSensor.setup(DHTPIN, DHTesp::DHT22);
}

void DHTSensor::checkTempHum(Display &oledDisplay) {
    TempAndHumidity data = dhtSensor.getTempAndHumidity();
    if (data.temperature > 32) {
        oledDisplay.printline("High Temperature", 1, 40, 0);
    } else if (data.temperature < 24) {
        oledDisplay.printline("Low Temperature", 1, 40, 0);
    } 
    if (data.humidity > 80) {
        oledDisplay.printline("High Humidity", 1, 50, 0);
    } else if (data.humidity < 65) {
        oledDisplay.printline("Low Humidity", 1, 50, 0);
    }
}