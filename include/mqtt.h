#ifndef MQTT_H
#define MQTT_H

#include <ArduinoJson.h>
#include <PubSubClient.h>
#include <WiFi.h>
#include <WiFiClientSecure.h>
#include <wifiHandling.h>
#include <definitions.h>
#include <lcd.h>

void initializeMqttConnection(WiFiClientSecure *espClient, PubSubClient *client);

void mqttconnect(PubSubClient *client);

void receivedCallback(char *topic, byte *payload, unsigned int length);

#endif
