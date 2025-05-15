#ifndef MENU_H
#define MENU_H

#include <Arduino.h>
#include "Display.h"
#include "Alarms.h"

#define MENU_OK 32 // GREEN
#define MENU_UP 33 // BLUE
#define MENU_DOWN 25 // YELLOW

class Menu {
public:
    Menu(Display &oleddisplay, Alarm &alarmClock);
    void initialize();
    void displayMenu();
    void checkMenuBtn();
    int pressedButton();
    void setTimeZone();
    void viewAlarms();
    void deleteAlarm();
    void setAlarm(int alarm);
    int setValue(int timetype);

    int currentMenu = 0;
    int menuSize = 5;
    String menuItems[5] = {"1 - Set UTC Time", "2 - Set Alarm 1", "3 - Set Alarm 2", "4 - Disable Alarm", "5 - View Alarms"};

private:
    Display &oleddisplay;
    Alarm &alarmClock;
};

#endif