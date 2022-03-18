#include <Arduino.h>
#include <AutonomousBonsai.h>
#include <PubSubClient.h>
#include <ESP8266WiFi.h>
#include <string>
unsigned long lastTime;

WiFiClient espClient;
PubSubClient MQTTClient(espClient);

void setup_wifi()
{

  WiFi.mode(WIFI_STA);
  WiFi.begin("iPhone 11", "123456789");

  delay(10000);

  while (WiFi.status() != WL_CONNECTED)
  {
    delay(500);
    Serial.print(WiFi.status());
  }
}

void setup()
{
  Serial.begin(115200);
  lastTime = millis();

  Serial.println("Setup WIFI");
  setup_wifi();

  Serial.println("Setup MQTT Server");
  MQTTClient.setServer("172.20.10.10", 1883);

  Serial.println("Setup PinMode");
  pinMode(GPIO_MOISTURE_SENSOR, INPUT);
}

void loop()
{
  const unsigned long currentTime = millis();

  if ((currentTime - lastTime) >= 3000)
  {
    int attempts = 0;

    while (!MQTTClient.connected() && attempts < 5)
    {
      Serial.println("Trying to connect MQTT");

      if (MQTTClient.connect("ESP8266Client"))
      {

        uint16_t const moistVal = map(analogRead(GPIO_MOISTURE_SENSOR), 0, 1023, 100, 0);
        std::string s = std::to_string(moistVal);

        if (MQTTClient.publish("bonsai/moisture", s.c_str()))
        {
          Serial.println("Published on bonsai/moisture");
        }
        else
        {
          Serial.println("Error during publish");
        }
      }
      delay(500);
      attempts++;
    }

    lastTime = currentTime;
  }
}
