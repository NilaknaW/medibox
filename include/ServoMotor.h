#ifndef SERVOMOTOR_H
#define SERVOMOTOR_H

#include <Arduino.h>
#include <ServoMotor.h>
#include <ESP32Servo.h> // Include the ESP32 Servo library
#include <Sensors.h> // Include the Sensors header file

#define SERVO_PIN 16 // Pin for the servo motor
#define SERVO_MAX_ANGLE 180 // Maximum angle for the servo

class ServoMotor {
public:
    ServoMotor(); // Constructor

    float idealTemp = 30; // Ideal temperature for the servo
    float servoMinAngle = 30; // Minimum angle for the servo

    void initialize(); // Initialize the servo motor
    void update(float temperature, LDR &ldr); // Update the servo position based on temperature  
    void setIdealTemp(float temp); // Set the ideal temperature for the servo
    void setServoMinAngle(float angle); // Set the minimum angle for the servo
    float getServoAngle();
    
private:
    Servo servo; // Servo object
    void setServoAngle(int angle); // Set the servo angle
    int currentAngle; // Current angle of the servo
};

#endif
