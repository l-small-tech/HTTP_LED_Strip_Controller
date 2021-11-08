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

        server->on("/posttest", HTTP_POST, [] (AsyncWebServerRequest *request) {
            Serial.print("Param count: ");
            Serial.println(request->params());

            Serial.println(request->getParam("ThatKey")->value());

            request->send(200, "text/plain", "OK");
        });

        server->on("/setRGB", HTTP_ANY, [&](AsyncWebServerRequest *request) {
            int r = request->getParam("r")->value().toInt();
            int g = request->getParam("g")->value().toInt();
            int b = request->getParam("b")->value().toInt();
            strip->setStripColor(r, g, b);
            request->send(200, "text/plain", "OK");
        });

        // Turn off Route
        server->on("/off", HTTP_ANY, [&](AsyncWebServerRequest *request) {
            strip->off();
            request->send(200, "text/plain", "OK");
        });

        // Turn on Route
        server->on("/on", HTTP_ANY, [&](AsyncWebServerRequest *request) {
            strip->on();
            request->send(200, "text/plain", "OK");
        });

        // Set Brightness Route
        server->on("/setBrightness", HTTP_ANY, [&](AsyncWebServerRequest *request) {

            if (request->hasParam("brightness")) {
                int brightness = request->getParam("brightness")->value().toInt();
                strip->setBrightness(brightness);
                Serial.printf("Set brightness to %d\n", brightness);
                request->send(200, "text/plain", "OK");
            } else {
                Serial.println("Bad call to /setBrightness");
                request->send(400, "text/plain", "Bad Request");
            }
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
};