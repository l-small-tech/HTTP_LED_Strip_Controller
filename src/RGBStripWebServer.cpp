#ifndef RGBSTRIPWEBSERVER
#define RGBSTRIPWEBSERVER
#endif

#include <ESP8266WiFi.h>
#include <ESPAsyncTCP.h>
#include <ESPAsyncWebServer.h>
#include "RGBStrip.cpp"
#include "secrets.h"

#define SERVER_PORT 80

class RGBStripWebServer {
private:
    RGBStrip* strip;
    AsyncWebServer* server;

    inline void initWiFi() {
        WiFi.begin(SSID, PASSWORD);
    
        // Wait for a connection
        while (WiFi.status() != WL_CONNECTED) {
            delay(1000);
            Serial.println(".");
        }
    
        // Print ESP Local IP Address
        Serial.println(WiFi.localIP());
    }

    inline void setupRoutes() {

        server->on("/off", HTTP_ANY, [&](AsyncWebServerRequest *request) {

            strip->off();
            request->send(200, "text/plain", "OK");
        });

        server->on("/on", HTTP_ANY, [&](AsyncWebServerRequest *request) {

            strip->on();
            request->send(200, "text/plain", "OK");
        });

        // Push a color to the strip
        server->on("/push", HTTP_ANY, [&](AsyncWebServerRequest *request) {

            Serial.println("/push called.");

            int r = request->getParam("r")->value().toInt();
            int g = request->getParam("g")->value().toInt();
            int b = request->getParam("b")->value().toInt();

            strip->push(r, g, b);
            Serial.printf("Pushed color: %d, %d, %d\n\n", r, g, b);
            request->send(200, "text/plain", "OK");
        });

        // Set Brightness Route
        server->on("/setBrightness", HTTP_ANY, [&](AsyncWebServerRequest *request) {

            if (request->hasParam("brightness")) {
                int brightness = request->getParam("brightness")->value().toInt();
                strip->setBrightness(brightness);
                Serial.printf("DEBUG: Setting brightness to %d\n", brightness);
                request->send(200, "text/plain", "OK");
            } else {
                Serial.print("Missing brightness parameter");
                request->send(400, "text/plain", "Bad Request");
            }
        });

        server->on("/setRGB", HTTP_ANY, [&](AsyncWebServerRequest *request) {

            Serial.println("DEBUG:\t/setRGB Called");

            int r = request->getParam("r")->value().toInt();
            int g = request->getParam("g")->value().toInt();
            int b = request->getParam("b")->value().toInt();

            Serial.printf("DEBUG:\tSetting RGB to R: %d, G: %d, B: %d\n", r, g, b);

            strip->setStripColor(r, g, b);
            request->send(200, "text/plain", "OK");
        });
    }

public:
    RGBStripWebServer() {
        // Initialize classes
        strip = new RGBStrip();
        server = new AsyncWebServer(SERVER_PORT);
                    
        // Setup the web server
        initWiFi();

        // Setup the endpoint routes
        setupRoutes();

        // Start server
        server->begin();
    }

    void loopHook() {
        strip->loopHook();
    }
};