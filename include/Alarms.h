#ifndef ALARMS_H
#define ALARMS_H

#include <Arduino.h>
#include "Display.h"

#define LED_ALARM 17
#define BUZZER 5

#define ALARM_STOP_BTN 35 // RED
#define ALARM_SNOOZE_BTN 26 // BLACK

#define SNOOZE_DURATION 5 // in minutes
extern long gmtOffset_sec;  // Declare as extern

class Alarm {
public:
    Alarm(Display &oleddisplay);

    void getcurrenttime();
    void initialize();
    void disconnectwifi();
    void checkAlarm();
    void ringAlarm();
    static void IRAM_ATTR stopAlarm();
    static void IRAM_ATTR snoozeAlarm();
    void restartTime();
    // void snoozeSet(); // snooze start time
    // static Alarm* getInstance();

    bool alarmEnabled = false;
    bool alarmTriggered[2] = {false, false}; // since we use only 2 alarms
    int alarmTime[2][2] = {{0, 0}, {0, 0}}; // alarm1(hour, minute), alarm2(hour, minute)

    // Getters and Setters for static volatile variables
    static bool getSnooze();
    static void setSnooze(bool state);
    static unsigned long getSnoozeStart();
    static void setSnoozeStart(unsigned long time);

private:
    Display &oleddisplay;
    const char* ntpServer = "pool.ntp.org";
    
    const int daylightOffset_sec = 0;
    const char* ssid = "SLT FIBER";
    const char* password = "hon2701962";

    static volatile bool snooze; // snooze is enabled or not
    static volatile unsigned long snoozeStart; // snooze start time
};

#endif