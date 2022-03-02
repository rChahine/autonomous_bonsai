#ifndef MQTT_LIB_H
#define MQTT_LIB_H

#include <ESP8266WiFi.h>
#include <PubSubClient.h>
#include <ArduinoJson.h>

// WIFI
#define WIFI_SSID 
#define WIFI_PASSWORD "richarda"

// MQTT
#define MQTT_IP "192.168.138.19k1"
#define MQTT_PORT 1883

#define MQTT_PUB_PUMP ""
#define MQTT_PUB_MOISTURE "/bonsai/moisture"

class MQTTLib {
    public:
        MQTTLib();
        ~MQTTLib();
        void init();
        void publish_moisture(const uint16_t payload) const;
    private:
        WiFiClient espClient;
        PubSubClient MQTTClient;
        void setup_wifi();
        void setup_mqtt();
};

#endif
