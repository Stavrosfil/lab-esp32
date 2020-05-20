#include "esp_system.h"
#include <ArduinoJson.h>
#include <LiquidCrystal.h>
#include <MFRC522.h>
#include <PubSubClient.h>
#include <SPI.h>
#include <WiFi.h>
#include <WiFiClientSecure.h>
#include <Wire.h>
#include <definitions.h>
#include <json.h>
#include <lcd.h>
#include <mqtt.h>
#include <rfidSensor.h>
#include <wifiHandling.h>

LiquidCrystal lcd(LCD_RS, LCD_EN, LCD_D4, LCD_D5, LCD_D6, LCD_D7);

// Create MFRC522 instance
MFRC522 mfrc522(SS_PIN, RST_PIN);

String tag_uuid    = "";
String msg         = "";
String device_uuid = "";

// create an instance of WiFiClientSecure
WiFiClientSecure espClient;
PubSubClient client(espClient);

long rfid_timer = 0;

//!-------------------------------WATCHDOG-------------------------------

// time in ms to trigger the watchdog
const int wdtTimeout = 10000;
hw_timer_t *timer    = NULL;

void IRAM_ATTR resetModule() {
    ets_printf("reboot\n");
    esp_restart();
}

//!-------------------------------SETUP-------------------------------

void setup() {

    timer = timerBegin(0, 80, true);                  // timer 0, div 80
    timerAttachInterrupt(timer, &resetModule, true);  // attach callback
    timerAlarmWrite(timer, wdtTimeout * 1000, false); // set time in us
    timerAlarmEnable(timer);                          // enable interrupt

    initializeLCD(&lcd);
    displayBootScreen();

#ifdef DEBUG
    Serial.begin(115200);
#endif

    DEBUG_PRINTLN();
    DEBUG_PRINT("Connecting to ");
    DEBUG_PRINTLN(SSID);

    pinMode(G_LED, OUTPUT);
    pinMode(R_LED, OUTPUT);
    digitalWrite(R_LED, HIGH);
    digitalWrite(G_LED, HIGH);

    initializeRFIDSensor(mfrc522);

    if (mfrc522.PCD_PerformSelfTest())
        DEBUG_PRINTLN("-------Passed the self test!--------");

    // Intialize the WiFi module
    initializeWifi();

    // Create the MQTT connection object
    initializeMqttConnection(&espClient, &client);

    // Intialize the RFID sensor
    initializeRFIDSensor(mfrc522);

    // Enable RFID sensor interrupts
    enableRFIDInterrupts(mfrc522);

    rfid_timer = millis();

    ledcSetup(BUZZER_PWM_CHANNEL, 1000, BUZZER_PWM_RESOLUTION);

    ledcWrite(0, 255);

    byte mac[ 6 ];
    WiFi.macAddress(mac);
    device_uuid = String(mac[ 0 ], HEX) + String(mac[ 1 ], HEX) + String(mac[ 2 ], HEX) + String(mac[ 3 ], HEX) +
                  String(mac[ 4 ], HEX) + String(mac[ 5 ], HEX);

    // Delay 100ms to set the things up
    delay(100);

    // Display the idle screen
    displayIdleScreen();
}

//!-------------------------------LOOP-------------------------------

void loop() {

    timerWrite(timer, 0); // reset timer (feed watchdog)

    if (millis() - rfid_timer > RFID_READ_TIME) {

        tag_uuid = readCard(mfrc522);

        if (tag_uuid != "") {

            displayAuthScreen();

            cardIdToJSON(tag_uuid, device_uuid, msg);

            client.publish(RFID_TOPIC, msg.c_str());

            DEBUG_PRINTLN();
            DEBUG_PRINT("Message : ");
            DEBUG_PRINTLN(msg);

            digitalWrite(G_LED, HIGH);

            // Clear the message contents to receive the next
            msg      = "";
            tag_uuid = "";

            rfid_timer = millis();
        }
    }

    // if client was disconnected then try to reconnect again
    if (!client.connected()) {
        mqttconnect(&client);
        delay(100);
    }

    // This function will listen for incomming
    // subscribed topic-process-invoke receivedCallback
    // Start the event loop
    client.loop();

    delay(100);
}