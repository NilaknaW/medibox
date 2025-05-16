#include "ServoMotor.h"
#include "Sensors.h"
#include <cmath>

ServoMotor::ServoMotor() {
    servo.attach(SERVO_PIN); // Attach the servo to the pin
    servo.write(servoMinAngle); // Set initial position to minimum angle
} // constructor

void ServoMotor::initialize() {
    servo.attach(SERVO_PIN); // Attach the servo to the pin
    servo.write(servoMinAngle); // Set initial position to minimum angle
} // initialize the servo motor

void ServoMotor::update(float temperature, LDR &ldr) {
    int angle = servoMinAngle + (180 - servoMinAngle) * ldr.getCurrentAverage() * ldr.GAMMA * log(ldr.interval / ldr.avgTime) * temperature / idealTemp; // Map temperature to angle
    setServoAngle(angle); // Set the servo angle
} // update the servo position based on temperature

void ServoMotor::setServoAngle(int angle) {
    if (angle < servoMinAngle) {
        angle = servoMinAngle; // Clamp to minimum angle
    } else if (angle > SERVO_MAX_ANGLE) {
        angle = SERVO_MAX_ANGLE; // Clamp to maximum angle
    }
    currentAngle = angle; // Update the current angle
    servo.write(angle); // Set the servo position
    Serial.print("Servo angle: ");
    Serial.println(angle); // Print the angle for debugging
} // set the servo angle

void ServoMotor::setIdealTemp(float temp) {
    this->idealTemp = temp; // Set the ideal temperature for the servo
} // set the ideal temperature for the servo

void ServoMotor::setServoMinAngle(float angle) {
    this->servoMinAngle = angle; // Set the minimum angle for the servo
} // set the minimum angle for the servo

float ServoMotor::getServoAngle() {
    return currentAngle; // Get the current angle of the servo
} // get the current angle of the servo