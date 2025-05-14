#include <Arduino.h>
#include "display.h"
#include "alarms.h"
#include "sensors.h"
#include "menu.h"

// create objects
Display oleddisplay; 
Alarm alarmClock(oleddisplay);
DHTSensor dht; 
Menu menu(oleddisplay, alarmClock);

// global variables
String dd, mm, yyyy, hh, minute, sec;
unsigned long prevMillis;
unsigned long currentMillis;

void setup() {
  oleddisplay.initialize();
  oleddisplay.printfull("Welcome to MediBox");
  alarmClock.initialize();
  dht.initialize();
  menu.initialize();
}

void loop() {
  oleddisplay.clear(); // clears the display
  alarmClock.getcurrenttime(); // updates globla time data variables
  alarmClock.checkAlarm(); // checks for alarms
  oleddisplay.printtime(); // prints the time on the display
  menu.checkMenuBtn(); // checks for menu button presses
  dht.checkTempHum(oleddisplay); // checks the temperature and humidity
  delay(500); // this speeds up the simulation
}
