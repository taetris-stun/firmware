#include <Arduino.h>
#include <ESP8266WiFi.h>
#include <ESP8266WebServer.h>
#include <WiFiManager.h>
#include <DNSServer.h>
#include <WebSocketsClient.h>
#include <ESP_DoubleResetDetector.h>

#define AP_PASSWORD "tinkertoe"
#define SERVER_ADDRESS "taetris-stun.tinkertoe.net"
#define SERVER_PORT 3001
#define SHOCKER_ID "4ta82s"
#define TRIGGER_PIN 14
#define RESET_TIMEOUT 1000
#define RESET_ADDRESS 0

WebSocketsClient webSocket;
int shockTime = 0;

void setupWifi() {
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
  wifiManager.autoConnect("dispo-shocker", AP_PASSWORD);

  Serial.println("- connected to network");
}

void webSocketEvent(WStype_t type, uint8_t * payload, size_t length) {
  switch (type)
  {
    case WStype_DISCONNECTED:
      Serial.println("- disconnected from server");
      break;

    case WStype_CONNECTED:
      Serial.println("- connected to server");
      webSocket.sendTXT(SHOCKER_ID);
      break;

    case WStype_TEXT:
      Serial.println("- received shock command");
      Serial.println(String((char *)payload));
      shockTime = String((char *)payload).toInt();
      break;
    
    default:
      break;
  }
}

void setup() {
  Serial.begin(9600);
  Serial.println();
  Serial.println();
  Serial.println("[ dispo-shocker // v0.0.1 ]");
  Serial.println();

  setupWifi();
  
  pinMode(TRIGGER_PIN, OUTPUT);

  webSocket.begin(SERVER_ADDRESS, SERVER_PORT, "/");
  webSocket.onEvent(webSocketEvent);
  webSocket.setReconnectInterval(5000);
}

void loop() {
  webSocket.loop();
  if (shockTime > 0) {
    digitalWrite(TRIGGER_PIN, HIGH);
    delay(shockTime);
    digitalWrite(TRIGGER_PIN, LOW);
    shockTime = 0;
  }
}