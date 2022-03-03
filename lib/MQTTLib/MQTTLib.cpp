#include <MQTTLib.h>

#include <ESP8266WiFi.h>
#include <PubSubClient.h>
#include <ArduinoJson.h>

WiFiClient espClient;

MQTTLib::MQTTLib() {}

void MQTTLib::init() {
    Serial.println("Connect WIFI");

    setup_wifi();
    MQTTClient = PubSubClient(espClient);
    

    Serial.println("Connect MQTT Server");
    setup_mqtt();
}

MQTTLib::~MQTTLib() {
    MQTTClient.disconnect();
    WiFi.disconnect();
}

void MQTTLib::setup_wifi() {
    
    WiFi.mode(WIFI_STA);
    WiFi.begin(WIFI_SSID, WIFI_PASSWORD);

    delay(10000);

    while (WiFi.status() != WL_CONNECTED)
    {
        Serial.print(".");
        delay(500);
    }

    Serial.println("WiFi connected");

}


void MQTTLib::setup_mqtt() {
    MQTTClient.setServer(MQTT_IP, MQTT_PORT);
}

void MQTTLib::publish_moisture(const uint16_t payload) const {
    StaticJsonDocument<32> doc;
    doc["moisture"] = payload;
    // MQTTClient.publish(MQTT_PUB_MOISTURE, (char*) serializeJson(doc, Serial));
}
