#ifndef DISPLAY_H
#define DISPLAY_H

#include <Arduino.h>
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>

#define OLED_W 128
#define OLED_H 64
#define OLED_RST -1
#define OLED_ADDR 0x3C

class Display {
public:
    Display();
    void initialize();
    void printfull(const String &msg);
    void printline(String msg, int txtSize, int row, int colm);
    void printtime();
    void clear();
    void updateTime();
    void printLDR(int ldrValue);
private:
    Adafruit_SSD1306 display;
};

#endif