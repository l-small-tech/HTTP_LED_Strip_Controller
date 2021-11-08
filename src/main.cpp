#include <Arduino.h>
#include "RGBStripWebServer.cpp"

#include <ESP8266WiFi.h>
#include <ESPAsyncTCP.h>
#include <ESPAsyncWebServer.h>

#define SERVER_PORT 80

RGBStripWebServer* ws;

void setup() {

  // Safety Delay
  delay(1000);

  // Serial port for debugging purposes
  Serial.begin(115200);
  Serial.println("\n------------------\n");

  // Initialized the web server
  ws = new RGBStripWebServer();

  Serial.println("Ready to roll!");

    // // Function Pointer Array Demo
    // typedef void (*farray)();
    // farray modes[5] = {NULL};

    // // modes[0] = &test;
    // modes[0] = []() {
    //     cout << "Test anon" << endl;
    // };

    // modes[1] = []() {
    //     cout << "Anon #2" << endl;
    // };

    // modes[1]();
}

void loop() {

}