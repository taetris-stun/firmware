#include <Arduino.h>
#include <ESP8266WiFi.h>
#include <DNSServer.h>
#include <ESP8266WebServer.h>
#include <WiFiManager.h>

#include <ESP_DoubleResetDetector.h>
#define RESET_TIMEOUT 2000
#define RESET_ADDRESS 0


void setup() {
  Serial.begin(9600);
  Serial.println();
  Serial.println();
  Serial.println("[ dispo-shocker // v0.0.1 ]");
  Serial.println();

  WiFiManager wifiManager;
  wifiManager.setDebugOutput(false);

  DoubleResetDetector* resetDetector = new DoubleResetDetector(RESET_TIMEOUT / 1000, RESET_ADDRESS);

  Serial.println("- checking for double reset");
  if (resetDetector->detectDoubleReset()) {
    Serial.println();
    Serial.println();
    Serial.println("[ RESET ]");
    Serial.println();
    wifiManager.resetSettings();
  }

  delay(RESET_TIMEOUT);
  Serial.println("- check over");
  resetDetector->stop();

  Serial.println("- starting WiFiManager");
  wifiManager.autoConnect("dispo-shocker", "tinkertoe");

  Serial.println("- connected to network");
}

void loop() {

}