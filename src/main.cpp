#include <Arduino.h>
#include <ESP8266WiFi.h>
#include <PubSubClient.h>

// WIFI
#define WIFI_SSID "iPhone 11"
#define WIFI_PASSWORD "123456789"

// MQTT
#define MQTT_IP "172.20.10.10"
#define MQTT_PORT 1883

#define MQTT_PUB_PUMP ""
#define MQTT_PUB_MOISTURE "bonsai/moisture"

#define GPIO_MOISTURE_SENSOR A0

unsigned long lastTime;

WiFiClient espClient;
PubSubClient mqttClient(espClient);

void setup_wifi()
{
  WiFi.begin(WIFI_SSID, WIFI_PASSWORD);

  Serial.print("Connecting");
  while (WiFi.status() != WL_CONNECTED)
  {
    delay(500);
    Serial.print(".");
  }
  Serial.println();

  Serial.print("Connected, IP address: ");
  Serial.println(WiFi.localIP());
}

void callback(char *topic, byte *payload, unsigned int length)
{
  Serial.print("Message arrived [");
  Serial.print(topic);
  Serial.print("] ");
  for (int i = 0; i < length; i++)
  {
    Serial.print((char)payload[i]);
  }
  Serial.println();
}

void reconnect()
{
  // Loop until we're reconnected
  while (!mqttClient.connected())
  {
    Serial.print("Attempting MQTT connection...");
    // Attempt to connect
    if (mqttClient.connect("ESPCLient"))
    {
      Serial.println("connected");
      mqttClient.subscribe(MQTT_PUB_MOISTURE);
    }
    else
    {
      Serial.print("failed, rc=");
      Serial.print(mqttClient.state());
      Serial.println(" try again in 5 seconds");
      // Wait 5 seconds before retrying
      delay(5000);
    }
  }
}

void mqtt_publish(String topic, uint16_t t){
  char top[topic.length()+1];
  topic.toCharArray(top, topic.length()+1);
  char t_char[50];
  String t_str = String(t);
  t_str.toCharArray(t_char, t_str.length() + 1);
  mqttClient.publish(top, t_char);
}

void setup()
{
  Serial.begin(115200);
  lastTime = millis();

  pinMode(GPIO_MOISTURE_SENSOR, INPUT);

  setup_wifi();

  mqttClient.setServer(MQTT_IP, MQTT_PORT);
  mqttClient.setCallback(callback);
  reconnect();
}

void loop()
{
  const unsigned long currentTime = millis();

  if ((currentTime - lastTime) >= 30000)
  {
    reconnect();
    mqttClient.loop();

    int analog_value = analogRead(GPIO_MOISTURE_SENSOR);
    uint16_t moistVal = map(analog_value, 0, 1023, 100, 0);
    Serial.print("Moisture value : ");
    Serial.println(moistVal);

    mqtt_publish(MQTT_PUB_MOISTURE, moistVal);

    lastTime = currentTime;
  }
}