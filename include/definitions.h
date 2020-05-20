#ifndef _DEFINTIONS_H_
#define _DEFINTIONS_H_

// enable/disable serial debug output
#define DEBUG

#ifdef DEBUG
#define DEBUG_PRINT(x)    Serial.print(x)
#define DEBUG_PRINTHEX(x) Serial.print(x, HEX)
#define DEBUG_PRINTDEC(x) Serial.print(x, DEC)
#define DEBUG_PRINTLN(x)  Serial.println(x)
#else
#define DEBUG_PRINT(x)
#define DEBUG_PRINTHEX(x)
#define DEBUG_PRINTDEC(x)
#define DEBUG_PRINTLN(x)
#endif

//----------------------------------TIMERS--------------------------------------

#define RFID_READ_TIME 3000

//---------------------------------INTERNET-------------------------------------

#include <credentials.h>

//-----------------------------------PINS---------------------------------------

// setting PWM properties
#define BUZZER_PIN                13
#define BUZZER_TONE_ACCEPTED_HIGH 880
#define BUZZER_TONE_ACCEPTED_LOW  440
#define BUZZER_TONE_DENIED_HIGH   220
#define BUZZER_TONE_DENIED_LOW    110
#define BUZZER_PWM_CHANNEL        0
#define BUZZER_PWM_RESOLUTION     8

// LED pins
#define G_LED 16
#define R_LED 4

// RFID sensor pins
#define RST_PIN 22 // Reset pin
#define SS_PIN  21 // Slave select pin
#define IRQ_PIN 17 // Interrupt pin

// LCD pins
#define LCD_RS 15
#define LCD_EN 2
#define LCD_D4 26
#define LCD_D5 25
#define LCD_D6 33
#define LCD_D7 32

#endif // _DEFINTIONS_H_
