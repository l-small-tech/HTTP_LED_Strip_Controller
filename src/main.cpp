#include <Arduino.h>
#include "RGBStripWebServer.cpp"

#include <ESP8266WiFi.h>
#include <ESPAsyncTCP.h>
#include <ESPAsyncWebServer.h>

#define SERVER_PORT 80

RGBStripWebServer* ws;

void setup() {

  // Safety Delay
  delay(750);

  // Serial port for debugging purposes
  Serial.begin(115200);
  Serial.println("\n------------------\n");

  // Initialized the web server
  ws = new RGBStripWebServer();

  Serial.println("Ready to roll!");

  // Turn the annoying blue Light off
  pinMode(D4, OUTPUT);
  digitalWrite(D4, HIGH);


}

void loop() {

}