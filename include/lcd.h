#ifndef LCD_H
#define LCD_H

#include <LiquidCrystal.h>
#include <Wire.h>
#include <definitions.h>

void displayCentered(String s1, String s2);

void displayConnectingScreen();

void displayAuthScreen();

int centerCursor(String s);

void displayIdleScreen();

void displayWelcomeScreen(String user_name, int status);

void displayBootScreen();

void initializeLCD(LiquidCrystal *lcd);

#endif
