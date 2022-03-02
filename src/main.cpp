#include <Arduino.h>
#include <AutonomousBonsai.h>
#include <MQTTLib.h>
 
unsigned long lastTime;

MQTTLib mqttClient;

void setup() {
  Serial.begin(115200);
  lastTime = millis();

  pinMode(GPIO_MOISTURE_SENSOR, INPUT);

  mqttClient.init();

}

void loop() {
  const unsigned long currentTime = millis();

  if((currentTime - lastTime) >= 30000) {
    
    int analog_value = analogRead(GPIO_MOISTURE_SENSOR);
    uint16_t moistVal = map(analog_value, 0, 1023, 100, 0);

    mqttClient.publish_moisture(moistVal);
    lastTime = currentTime;
  }

}
