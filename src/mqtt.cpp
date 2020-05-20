#include <mqtt.h>

const size_t CAPACITY = JSON_OBJECT_SIZE(3);
StaticJsonDocument<CAPACITY> doc;

void initializeMqttConnection(WiFiClientSecure *espClient, PubSubClient *client) {

    // Set SSL/TLS certificate
    espClient->setCACert(CA_CERT);

    // Configure the MQTT server with IPaddress and port
    client->setServer(SERVER_HOSTNAME, SERVER_PORT);

    // This receivedCallback function will be invoked
    // when client received subscribed topic
    client->setCallback(receivedCallback);
}

void mqttconnect(PubSubClient *client) {

    // Loop until reconnected
    while (!client->connected()) {

        displayConnectingScreen();
        DEBUG_PRINT("MQTT connecting ... ");

        // client ID for the MQTT Broker
        String clientId = "lab_1";

        // connect now
        if (client->connect(clientId.c_str(), MQTT_USERNAME, MQTT_PASSWORD)) {

            displayCentered("Connected!", "");
            DEBUG_PRINTLN("connected!");

            // subscribe topic
            // ? mabye put this in the main loop, since the mqttName does not
            // ? suggest subscibing
            client->subscribe(RESPONSE_TOPIC);
            displayIdleScreen();
        } else {
            displayCentered("Connection fail", "Code: " + client->state());
            DEBUG_PRINT("failed, status code = ");
            DEBUG_PRINT(client->state());
            DEBUG_PRINTLN(" trying again in 5 seconds");
            // Wait 5 seconds before retrying
            delay(5000);
        }
    }
}

void receivedCallback(char *topic, byte *payload, unsigned int length) {

    DEBUG_PRINT("Message received on: ");
    DEBUG_PRINTLN(topic);
    DEBUG_PRINT("payload: ");

    // TODO: test this with a string for dynamic size in future usage.
    // TODO: Also maybe put this in the separate json file
    char json[ 300 ];

    for (int i = 0; i < length; i++) {
        json[ i ] = (char)payload[ i ];
    }

    json[ length ] = '\0';
    DEBUG_PRINTLN(String(json));

    // Deserialize the JSON document
    DeserializationError error = deserializeJson(doc, json);

    // Test if parsing succeeds.
    if (error) {
        DEBUG_PRINT(F("deserializeJson() failed: "));
        DEBUG_PRINTLN(error.c_str());
        return;
    }

    // Fetch values.
    //
    // Most of the time, you can rely on the implicit casts.
    // In other case, you can do doc["time"].as<long>();
    // String user        = doc[ "user_name" ];
    String lab_uuid    = doc[ "lab_uuid" ];
    String top_line    = doc[ "top_line" ];
    String bottom_line = doc[ "bottom_line" ];

    displayCentered(top_line, bottom_line);

    if (lab_uuid != "") {

        // Print values.
        // DEBUG_PRINTLN("Received user: " + user);
        // DEBUG_PRINTLN("With status: " + String(user_status));
        ledcAttachPin(BUZZER_PIN, BUZZER_PWM_CHANNEL);

        if (lab_uuid != "0") {

            digitalWrite(G_LED, LOW);

            // Welcome tone! ( F5, A5, G6 )
            // Play this tone when someone enters the lab!
            // ledcWriteTone(BUZZER_PWM_CHANNEL, 698);
            // delay(80);
            // ledcWriteTone(BUZZER_PWM_CHANNEL, 880);
            // delay(80);
            ledcWriteTone(BUZZER_PWM_CHANNEL, 1567);
            delay(160);

            DEBUG_PRINTLN("User entered lab!");

        } else if (lab_uuid == "0") {

            digitalWrite(G_LED, LOW);

            // Goodbye tone ( A5, F5, C5, A4 )
            // Play this tone when someone exits the lab!
            // ledcWriteTone(BUZZER_PWM_CHANNEL, 880);
            // delay(80);
            // ledcWriteTone(BUZZER_PWM_CHANNEL, 698);
            // delay(80);
            // ledcWriteTone(BUZZER_PWM_CHANNEL, 523);
            // delay(80);
            ledcWriteTone(BUZZER_PWM_CHANNEL, 440);
            delay(160);

            DEBUG_PRINTLN("User exited lab!");
        }

    } else {

        DEBUG_PRINTLN("Unknown user");
        displayCentered("Unknown user", "Try again");
        digitalWrite(R_LED, LOW);
        ledcAttachPin(BUZZER_PIN, BUZZER_PWM_CHANNEL);

        // Play the error tone if something goes wrong
        ledcWriteTone(BUZZER_PWM_CHANNEL, BUZZER_TONE_DENIED_HIGH);
        delay(250);
        ledcWriteTone(BUZZER_PWM_CHANNEL, BUZZER_TONE_DENIED_LOW);
        delay(250);
    }

    ledcDetachPin(BUZZER_PIN);
    delay(1500);

    digitalWrite(R_LED, HIGH);
    digitalWrite(G_LED, HIGH);

    displayIdleScreen();
}
