#include "Alarms.h"
#include "Menu.h"
#include <WiFi.h>
#include <time.h>

extern String dd, mm, yyyy, hh, minute, sec; // define external variables, so that global variables can be accessed
extern unsigned long currentMillis, prevMillis;
long gmtOffset_sec = 19800;  // Define it in one .cpp file`

volatile bool Alarm::snooze = false;
volatile unsigned long Alarm::snoozeStart = 0;

Alarm::Alarm(Display &oleddisplay): oleddisplay(oleddisplay) {}

void Alarm::initialize() {
    oleddisplay.printline("Connecting to WiFi", 1, 10, 0);

    WiFi.begin("Wokwi-GUEST", "", 6); // connect to wokwi guest wifi

    while (WiFi.status() != WL_CONNECTED){
        delay(1000);
        oleddisplay.printline("...", 1, 20, 0);
        yield();
    }
    oleddisplay.printline("Connected", 1, 30, 0);

    // Initialize LEDC for buzzer
    ledcSetup(0, 5000, 8);  // Channel 0, 5kHz frequency, 8-bit resolution
    ledcAttachPin(BUZZER, 0);


    // init time
    configTime(gmtOffset_sec, daylightOffset_sec, ntpServer);
    getcurrenttime();

    pinMode(LED_ALARM, OUTPUT);
    pinMode(BUZZER, OUTPUT); // check
    pinMode(ALARM_STOP_BTN, INPUT_PULLUP); // check
    
    digitalWrite(LED_ALARM, LOW);
    attachInterrupt(digitalPinToInterrupt(ALARM_STOP_BTN), stopAlarm, CHANGE);
    attachInterrupt(digitalPinToInterrupt(ALARM_SNOOZE_BTN), snoozeAlarm, CHANGE);

    snooze = false; // set snooze to false
    // snoozeStart = 0; // reset snooze start time
    snoozeStart = millis(); // store the time when snooze is pressed
}

void Alarm::disconnectwifi() {
    WiFi.disconnect(true);
    WiFi.mode(WIFI_OFF);
}

void Alarm::getcurrenttime() {
    // get the current time
    struct tm timeinfo;
    if (!getLocalTime(&timeinfo)){ // data is stored in timeinfo
        // oleddisplay.clear();
        oleddisplay.printline("Failed to obtain time", 1, 30, 0);
        return;
    };

    dd = String(timeinfo.tm_mday);
    mm = String(timeinfo.tm_mon + 1);
    yyyy = String(timeinfo.tm_year + 1900);
    hh = String(timeinfo.tm_hour);
    minute = String(timeinfo.tm_min);
    sec = String(timeinfo.tm_sec);
    prevMillis = millis();
 
    // oleddisplay.clear();
    oleddisplay.printtime(); // print on top (0,0)
}

void Alarm::checkAlarm() {
    while(!alarmEnabled) return; // if alarm is not enabled, return

    if (snooze) { // check if snooze is enabled and if the snooze duration is not over
        if ((millis() - snoozeStart) > SNOOZE_DURATION*60000) {
            snooze = false; // set snooze to false
            snoozeStart = 0; // reset snooze start time
            ringAlarm(); // ring the alarm again
        } else {
            oleddisplay.printline("Snoozing alarm", 1, 10, 0); // print on 2nd row
            oleddisplay.printline(String(SNOOZE_DURATION - (millis() - snoozeStart)/60000) + " min left", 1, 20, 0);
            return; // if snooze is enabled, return
        }
    }

    for (int i = 0; i < 2; i++){
        if (alarmTime[i][0] == hh.toInt() && alarmTime[i][1] == minute.toInt() && !alarmTriggered[i]){
            alarmTriggered[i] = true;
            ringAlarm();
        }
    } 
}


void Alarm::ringAlarm() {
    oleddisplay.printline("Medicine Time!", 1, 10, 0); // print on 2nd row
    digitalWrite(LED_ALARM, HIGH); // turn on the LED

    if (millis() % 1000 < 500) {  // Toggle buzzer every 1s
        tone(BUZZER, 440);
    } else {
        noTone(BUZZER);
    }
}

void IRAM_ATTR Alarm::stopAlarm() {
    digitalWrite(LED_ALARM, LOW);
    noTone(BUZZER);
    snooze = false; // set snooze to true
}

void IRAM_ATTR Alarm::snoozeAlarm() {
    // noInterrupts(); // disable interrupts to avoid multiple calls
    digitalWrite(LED_ALARM, LOW);
    noTone(BUZZER);
    snooze = true; // set snooze to true
    snoozeStart = millis(); // store the time when snooze is pressed
    // interrupts(); // enable interrupts   
}

void Alarm::restartTime() {
    configTime(gmtOffset_sec, daylightOffset_sec, ntpServer);
    getcurrenttime();
}

// getter and setter functions for snooze and snoozeStart
bool Alarm::getSnooze() {
    return snooze;
}
unsigned long Alarm::getSnoozeStart() {
    return snoozeStart;
}
void Alarm::setSnooze(bool state) {
    snooze = state;
}
void Alarm::setSnoozeStart(unsigned long time) {
    snoozeStart = time;
}

