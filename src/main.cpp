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
    
    int value = analogRead(GPIO_MOISTURE_SENSOR);
    Serial.print("Value");
    Serial.print(value);
    Serial.println("\n");
  
    mqttClient.publish_moisture(value);
    lastTime = currentTime;
  }

}