/*
	HumiditySensorExample.ino

	This humidity accessry reads the value sends it to Homebridge mqtt.

	The circuit:
	* Input
        A0 - Soil moisture sensor

*/

#include <Arduino.h>
#include <WiFiManager.h>
#include "ArduinoHomebridgeMqtt.h"

const IPAddress MQTT_SERVER = IPAddress(192, 168, 1, 48);
const int INPUT_PIN = A0;
const char* HUMIDITY_SENSOR_NAME = "HumiditySensor";
const char* HUMIDITY_SENSOR_SERVICE = "HumiditySensor";
const char* CURRENT_RELATIVE_HUMIDITY = "CurrentRelativeHumidity";

long lastMsg = 0;
WiFiManager wifiManager;
ArduinoHomebridgeMqtt arduinoHomebridgeMqtt;

int readHumidity(int inputPin) {
  int value = analogRead(inputPin);
  value = constrain(value, 567, 959);
  return map(value, 567, 959, 100, 0);
}

void setup() {
  Serial.begin(9600);
  wifiManager.autoConnect();
  arduinoHomebridgeMqtt.connect(MQTT_SERVER);
  arduinoHomebridgeMqtt.addAccessory(HUMIDITY_SENSOR_NAME, HUMIDITY_SENSOR_SERVICE);
  arduinoHomebridgeMqtt.getAccessory();
}

void loop() {
  long now = millis();
  if (now - lastMsg > 5000) {
    lastMsg = now;
    int currentHumidity = readHumidity(INPUT_PIN);
    Serial.print("Current humidity ");
    Serial.print(currentHumidity);
    Serial.println("%");
    arduinoHomebridgeMqtt.setValueToHomebridge(HUMIDITY_SENSOR_NAME, CURRENT_RELATIVE_HUMIDITY, currentHumidity);
  }
  arduinoHomebridgeMqtt.loop();
}
