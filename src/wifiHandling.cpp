#include <wifiHandling.h>

void initializeWifi() {

    displayCentered("Connecting to", SSID);

    WiFi.begin(SSID, PASSWORD);
    delay(500);

    while (WiFi.status() != WL_CONNECTED) {
        delay(500);
        DEBUG_PRINT(".");
    }

    DEBUG_PRINTLN("");
    DEBUG_PRINTLN("WiFi connected");
    DEBUG_PRINTLN("IP address: ");
    DEBUG_PRINTLN(WiFi.localIP());
    // /*setup MDNS for ESP32 */
    // if (!MDNS.begin("esp32")) {
    //     DEBUG_PRINTLN("Error setting up MDNS responder!");
    //     while (1) {
    //         delay(1000);
    //     }
    // }
    /* get the IP address of server by MDNS name */
    // DEBUG_PRINTLN("mDNS responder started");
    // IPAddress serverIp = MDNS.queryHost(serverHostname);
    // DEBUG_PRINT("IP address of server: ");
    // DEBUG_PRINTLN(serverIp.toString());
}
