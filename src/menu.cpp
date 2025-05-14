#include "Menu.h"
#include "Alarms.h"

Menu::Menu(Display &oleddisplay, Alarm &alarmClock): oleddisplay(oleddisplay), alarmClock(alarmClock) {}

void Menu::initialize() {
    pinMode(MENU_OK, INPUT);
    pinMode(MENU_UP, INPUT);
    pinMode(MENU_DOWN, INPUT);
}

void Menu::checkMenuBtn() {
    if(digitalRead(MENU_OK)==LOW){
        displayMenu();
    }
}

int Menu::pressedButton() {
    while(true){
    if (digitalRead(MENU_UP)==LOW){
        delay(200); // `debounce`
        // oleddisplay.printline("UP", 1, 40, 0);
        return 1;
    } 
    else if (digitalRead(MENU_DOWN)==LOW){
        delay(200); // `debounce`
        // oleddisplay.printline("DOWN", 1, 50, 0);
        return 2;
    } 
    else if (digitalRead(MENU_OK)==LOW){
        delay(200); // `debounce`
        return 0;
    }
    else if (digitalRead(ALARM_STOP_BTN)==LOW){
        return -1;
    }
}
}

void Menu::displayMenu() {
    while(digitalRead(ALARM_STOP_BTN)==HIGH){
        oleddisplay.clear();
        oleddisplay.printfull("Menu");
        oleddisplay.printline(menuItems[currentMenu], 1, 10, 0);
        delay(200);

        int val = pressedButton();

        switch (val) {
        case 1: // up
            currentMenu = (currentMenu + 1) % menuSize;
            break;
        case 2: // down
            currentMenu = (currentMenu -1 + menuSize) % menuSize;
            break;
        case 0: // ok
            switch (currentMenu){
            case 0:
                setTimeZone();
                break;
            case 1:
                setAlarm(currentMenu-1);
                break;
            case 2:
                setAlarm(currentMenu-1);
                break;
            case 3:
                deleteAlarm();
                break;
            case 4:
                viewAlarms();
                break;
            }
        }
    }
}

void Menu::setTimeZone() {
    oleddisplay.clear();
    oleddisplay.printfull("Set UTC time zone");
    oleddisplay.printline("Current UTC Offset: ", 1, 10, 0);
    oleddisplay.printline(String(gmtOffset_sec), 1, 20, 0);
    oleddisplay.printline("Enter Hour", 1, 40, 0);
    delay(200);
    int hour = setValue(24); // set the hour
    if (hour == 100){
        return;
    }
    delay(200);

    oleddisplay.printfull("Set UTC Offset");
    oleddisplay.printline("Hour set:", 1, 10, 0);
    oleddisplay.printline(String(hour), 1, 20, 0);
    oleddisplay.printline("Enter Minute", 1, 30, 0);
    delay(200);
    int minute = setValue(60); // set the minute
    if (minute == 100){
        return;
    }
    delay(200);

    oleddisplay.printfull("Set UTC Offset");
    oleddisplay.printline("Hour set:", 1, 10, 0);
    oleddisplay.printline(String(hour), 1, 20, 0);
    oleddisplay.printline("Minute set:", 1, 30, 0);
    oleddisplay.printline(String(minute), 1, 40, 0);
    delay(2000);

    // update the utc offset time
    gmtOffset_sec = (hour * 3600) + (minute * 60);

    alarmClock.restartTime();

    oleddisplay.printfull("UTC Offset set to: " + String(hour) + ":" + String(minute));
    delay(3000);
}

void Menu::setAlarm(int alarm) {   
    oleddisplay.clear();
    oleddisplay.printfull("Set Alarm " + String(alarm+1));
    oleddisplay.printline("Enter Hour", 1, 10, 0);
    delay(200);
    int hour = setValue(24);
    if (hour == 100){
        return;
    }
    delay(200);

    oleddisplay.printfull("Set Alarm " + String(alarm+1));
    oleddisplay.printline("Hour set:", 1, 10, 0);
    oleddisplay.printline(String(hour), 1, 20, 0);
    oleddisplay.printline("Enter Minute", 1, 30, 0);
    delay(200);
    int minute = setValue(60); // set the minute
    if (minute == 100){
        return;
    }
    delay(200);

    oleddisplay.printfull("Set Alarm " + String(alarm+1));
    oleddisplay.printline("Hour set:", 1, 10, 0);
    oleddisplay.printline(String(hour), 1, 20, 0);
    oleddisplay.printline("Minute set:", 1, 30, 0);
    oleddisplay.printline(String(minute), 1, 40, 0);
    delay(2000);

    // set the alarm time
    alarmClock.alarmTime[alarm][0] = hour;
    alarmClock.alarmTime[alarm][1] = minute;

    // enable the alarm
    alarmClock.alarmEnabled = true;

    // display the alarm time
    oleddisplay.clear();
    oleddisplay.printfull("Alarm Set");
    oleddisplay.printline("Alarm " + String(alarm+1) + " at " + String(hour) + ":" + String(minute), 1, 10, 0);
    delay(3000);
}

int Menu::setValue (int timetype) {
    int temp = 0;
    int value = 0;
    int button = pressedButton();
    while(button != -1 || button != 0){
        int button = pressedButton();
        switch (button) {
        case 1: // up
            temp = (temp + 1) % timetype;
            break;
        case 2: // down
            temp = (temp - 1 + timetype) % timetype;
            break;
        case 0: // ok
            value = temp;
            oleddisplay.clear();
            oleddisplay.printline(String(temp), 2, 40, 0);
            return value;
            break;
        case -1:
            return 100;
            break;
        }
        oleddisplay.clear();
        oleddisplay.printline(String(temp), 2, 40, 0);
        delay(200);
    }
}

void Menu::viewAlarms() {
    while(digitalRead(ALARM_STOP_BTN)==HIGH){
        // oleddisplay.clear();
        oleddisplay.printfull("My Alarms");
        oleddisplay.printline("Alarm 1: " + String(alarmClock.alarmTime[0][0]) + ":" + String(alarmClock.alarmTime[0][1]), 1, 20, 0);
        oleddisplay.printline("Alarm 2: " + String(alarmClock.alarmTime[1][0]) + ":" + String(alarmClock.alarmTime[1][1]), 1, 30, 0);
        delay(200);
    }
    return;
}

void Menu::deleteAlarm() {
    int alarmNum = 0;
    while(digitalRead(ALARM_STOP_BTN)==HIGH){
        oleddisplay.clear();
        oleddisplay.printfull("Delete Alarm");
        oleddisplay.printline("Alarm 1: " + String(alarmClock.alarmTime[0][0]) + ":" + String(alarmClock.alarmTime[0][1]), 1, 20, 0);
        oleddisplay.printline("Alarm 2: " + String(alarmClock.alarmTime[1][0]) + ":" + String(alarmClock.alarmTime[1][1]), 1, 30, 0);
        oleddisplay.printline("Delete alarm "+ String(alarmNum+1), 1, 50, 0);
        delay(200);
        
        int val = pressedButton();
        switch (val) {
        case 1 or 2: // up
            alarmNum = (alarmNum + 1) % 2;
            break;
        case 0: // ok
            alarmClock.alarmTime[alarmNum][0] = 0;
            alarmClock.alarmTime[alarmNum][1] = 0;
            alarmClock.alarmEnabled = false;
            oleddisplay.printfull("Alarm " + String(alarmNum+1) + " deleted");
            delay(2000);
            return;            
        }
    }
}
