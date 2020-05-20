#include <Arduino.h>
#include <lcd.h>

byte customChar[] = {B01100, B00010, B01100, B00010, B01100, B00000, B00000, B00000};

LiquidCrystal *local_lcd;

int centerCursor(String s) {
    // unusedLength represents the characters that remain empty on the display
    // This particular lcd has a resolution of 16x2 characters, so 16 on each
    // line.
    int unusedLength = 16 - s.length();
    if (unusedLength >= 0) {
        return unusedLength / 2;
    } else {
        return 0;
    }
}

void displayCentered(String s1, String s2) {
    local_lcd->clear();
    local_lcd->home();
    local_lcd->setCursor(centerCursor(s1), 0);
    local_lcd->print(s1);
    local_lcd->setCursor(centerCursor(s2), 1);
    local_lcd->print(s2);
}

void displayConnectingScreen() { displayCentered("Connecting to", SERVER_HOSTNAME); }

void displayAuthScreen() { displayCentered("Please wait", "......"); }

void displayIdleScreen() {
    local_lcd->clear();
    local_lcd->home();
    local_lcd->setCursor(centerCursor("A3SAT"), 0);
    local_lcd->print("A");
    local_lcd->write(byte(0));
    local_lcd->print("SAT");
    local_lcd->setCursor(centerCursor("--- Scan ID ---"), 1);
    local_lcd->print("--- Scan ID ---");
}

void displayWelcomeScreen(String user_name, int status) {
    local_lcd->clear();
    local_lcd->home();
    if (status) {
        local_lcd->setCursor(centerCursor("Welcome"), 0);
        local_lcd->print("Welcome");
    } else {
        local_lcd->setCursor(centerCursor("Goodbye"), 0);
        local_lcd->print("Goodbye");
    }
    local_lcd->setCursor(centerCursor(user_name + "."), 1);
    local_lcd->print(user_name + ".");
}

void displayBootScreen() {
    local_lcd->setCursor(0, 0);
    local_lcd->print("   A");
    local_lcd->write(byte(0));
    local_lcd->print("SAT Lab.   ");
    local_lcd->setCursor(0, 1);
    local_lcd->print("   Booting...   ");
}

void initializeLCD(LiquidCrystal *lcd) {
    // Print the booting status of the MCU
    local_lcd = lcd;
    local_lcd->begin(16, 2);
    local_lcd->createChar(0, customChar);
    local_lcd->clear();
}
