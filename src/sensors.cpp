#include "Sensors.h"

DHTSensor::DHTSensor(): dhtSensor() {} // constructor

void DHTSensor::initialize() {
    dhtSensor.setup(DHTPIN, DHTesp::DHT22);
}

void DHTSensor::checkTempHum(Display &oledDisplay) {
    data = dhtSensor.getTempAndHumidity();
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

LDR::LDR() {
    avgTime = 2.0f; // default average time in minutes
    interval = 5.0f; // default interval in seconds
    recalculateAvgWindow(); // recalculate the average window size
    resetReadings(); // reset the readings array
} // constructor

void LDR::initialize() {
    pinMode(LDRPIN, INPUT);
    analogReadResolution(12); // Set ADC resolution to 12 bits (0-4095)
}

float LDR::readLDR() {
    // // Convert the analog value into lux value:
    // int analogValue = analogRead(A0);
    // float voltage = analogValue / 1024. * 5;
    // float resistance = 2000 * voltage / (1 - voltage / 5);
    // float lux = pow(RL10 * 1e3 * pow(10, GAMMA) / resistance, (1 / GAMMA));
    // // Print the lux value on the display
    // return lux;     

    // Read raw analog value (0-4095 for ESP32)
    int analogValue = analogRead(LDRPIN); // Use your actual pin

    // Convert to voltage (0-3.3V)
    float voltage = analogValue * (VREF / MAX_ADC);

    // Calculate resistance (assuming 2kΩ series resistor)
    float resistance = 2000.0f * voltage / (VREF - voltage);
    
    // Calculate lux using the photoconductive cell formula
    float lux = pow(RL10 * 1e3f * pow(10.0f, GAMMA) / resistance, (1.0f / GAMMA));
    
    // Normalize to 0-1 range based on expected maximum lux
    float normalized = constrain(lux / MAX_EXPECTED_LUX, 0.0f, 1.0f);
    
    return normalized;
}

float LDR::updateLDR() {
    // Read the LDR value at regular intervals
    unsigned long currentTime = millis();

    if (currentTime - lastReadingTime >= static_cast<unsigned long>(interval * 1000)) { // check if the interval has passed
        // Read the LDR value and store it in the array
        ldrReadings[readingIndex] = readLDR();
        // Print the lux value to serial monitor for debugging
        // Serial.print(ldrReadings[readingIndex]); // print the lux value
        // Serial.print(" ");
        // Update the reading index and reset the last reading time
        readingIndex = (readingIndex + 1) % avgWindow;
        lastReadingTime = currentTime;

        // Calculate the average
        float sum = 0.0f;
        for (int i = 0; i < avgWindow; i++) {
            sum += ldrReadings[i];
        }
        currentAverage = sum / avgWindow;
    }
    return currentAverage; // return the current average lux value
}

void LDR::setInterval(float interval) {
    this->interval = interval;
    recalculateAvgWindow(); // recalculate the average window size
    resetReadings(); // reset the readings array
}

void LDR::setAvgTime(float avgTime) {
    this->avgTime = avgTime; // set the average time
    recalculateAvgWindow(); // recalculate the average window size
    resetReadings(); // reset the readings array
}

void LDR::recalculateAvgWindow() {
    avgWindow = static_cast<int>(avgTime * 60.0f / interval); // recalculate the average window size
    if (avgWindow < 1) {
        avgWindow = 1; // ensure the average window size is at least 1
    }
}

float LDR::getCurrentAverage() {
    return currentAverage; // return the current average lux value
}

void LDR::resetReadings() {
    delete[] ldrReadings; // free the old memory
    ldrReadings = new float[avgWindow]; // allocate memory for the new readings array
    readingIndex = 0; // reset the reading index
}

void LDR::setGamma(float gamma) {
    this->GAMMA = gamma; // set the gamma value
}