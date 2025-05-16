#include "Display.h"

extern unsigned long currentMillis, prevMillis;
extern String dd, mm, yyyy, hh, minute, sec;

Display::Display() : display(OLED_W, OLED_H, &Wire, OLED_RST) {}

void Display::initialize() {
    Serial.begin(9600);
    if(!display.begin(SSD1306_SWITCHCAPVCC, OLED_ADDR)){
        Serial.println(F("SSD1306 allocation failed"));
        for(;;); // donot proceed, loop forevver
    }

    display.display();
    delay(2000);
}

void Display::printfull(const String &msg) {
    display.clearDisplay();
    display.setTextSize(1);
    display.setTextColor(SSD1306_WHITE);
    display.setCursor(0,0);
    display.println(msg);
    display.display();
}

void Display::clear() {
    display.clearDisplay();
    display.display();
}

void Display::printline(String msg, int txtSize, int row, int colm) {
    display.setTextSize(txtSize);
    display.setTextColor(SSD1306_WHITE);
    display.setCursor(colm,row);
    display.println(msg);
    display.display();
}

void Display::printtime() {
    // Define the position and size of the time area
    int x = 0, y = 0, width = 128, height = 10;  // Adjust as needed

    // Overwrite only the time area with a black rectangle
    display.fillRect(x, y, width, height, BLACK);
    
    display.setTextSize(1);
    display.setTextColor(SSD1306_WHITE);
    display.setCursor(0,0);
    display.println(dd + "/" + mm + "/" + yyyy + " " + hh + ":" + minute + ":" + sec);
    display.display();
}

void Display::updateTime(){
    currentMillis = millis();
    unsigned long seconds = (currentMillis - prevMillis) / 1000;

    sec = String((sec.toInt() + seconds) % 60);

    if (seconds >= 60){
        prevMillis += 60 * 1000;
        minute = String((minute.toInt() + 1) % 60);
    }

    if (minute.toInt() == 60){
        minute = "00";
        hh = String((hh.toInt() + 1) % 24);
    }

    printtime();
}

void Display::printLDR(int ldrValue) {
    printline("LDR: " + String(ldrValue), 1, 30, 0); // prints the LDR value
}