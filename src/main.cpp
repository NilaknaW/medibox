#include <Arduino.h>
#include "display.h"
#include "alarms.h"
#include "sensors.h"
#include "menu.h"
#include "servomotor.h"
#include "mqtt.h"

// create objects
Display oleddisplay; 
Alarm alarmClock(oleddisplay);
DHTSensor dht; 
Menu menu(oleddisplay, alarmClock);
LDR ldr;
ServoMotor servo; 
MQTTManager mqtt(ldr, servo, dht);

// global variables
String dd, mm, yyyy, hh, minute, sec;
unsigned long prevMillis;
unsigned long currentMillis;

void setup() {
  // initialize sensors
  dht.initialize();
  ldr.initialize();
  oleddisplay.initialize();
  // initialize the user interface
  oleddisplay.printfull("Welcome to MediBox"); // prints welcome message
  alarmClock.initialize();
  menu.initialize();
  mqtt.initialize();

  // debugging serial monitor
  Serial.begin(9600);
}

void loop() {
  alarmClock.getcurrenttime(); // updates globla time data variables
  alarmClock.checkAlarm(); // checks for alarms
  
  menu.checkMenuBtn(); // checks for menu button presses
  ldr.updateLDR(); // updates the LDR value
  
  oleddisplay.clear(); // clears the display
  oleddisplay.printtime(); // prints the time on the display
  oleddisplay.printLDR(ldr.getCurrentAverage()); // prints the LDR value
  
  dht.checkTempHum(oleddisplay); // checks the temperature and humidity and print warning messages
  servo.update(dht.data.temperature, ldr); // update the servo position based on temperature
  
  mqtt.communicate();
  mqtt.sendData(); // send data to the MQTT broker
  delay(1000); // this speeds up the simulation
}
