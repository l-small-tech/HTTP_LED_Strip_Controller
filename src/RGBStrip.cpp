#ifndef RGBSTRIP
#define RGBSTRIP
#endif

/*
    White High density Strip: BTF-LIGHTING RGBW RGBNW Natural White SK6812 (Similar WS2812B)
    Black Lower Density Strip: SK6812 RGBW RGB Warm RGBNW Led Strip

    On the Wemos D1 Mini, Use D7 as the Data Pin. D7 == GPIO 13 == MOSI
    Use 430-470 Ohm with the data pin
*/

#include <FastLED.h>
#include <Arduino.h>

// ---- Parameters ----
#define DATA_PIN    2
#define NUM_LEDS    50
#define BRIGHTNESS  255
#define FPS         48
#define LED_TYPE    WS2812B
#define COLOR_ORDER GRB


class RGBStrip {
    
private:
  CRGB* leds = new CRGB[NUM_LEDS];
  CRGB* leds_bkp = new CRGB[NUM_LEDS];
  int brightness = BRIGHTNESS;

  short mode;

  unsigned long thisTime;
  unsigned long lastTime = 0;


public:

    RGBStrip() {
        FastLED.addLeds<WS2812B, DATA_PIN, GRB>(leds, NUM_LEDS).setCorrection( TypicalLEDStrip );
        FastLED.setBrightness(brightness);
    }

    void push(int r, int g, int b) {
        // Set first led to color
        leds_bkp[0] = CRGB(r, g, b);

        // Shift all the leds one place down the strip
        for (int i = 0; i < NUM_LEDS - 1; i++) {
            leds_bkp[i + 1] = leds[i];
        }

        // Copy the new shifted data to the strip
        for (int i = 0; i < NUM_LEDS; i++) {
            leds[i] = leds_bkp[i];
        }

        showSync();
    }
    
    void setBrightness(int b) {

        this->brightness = b;
        FastLED.setBrightness(b);

        showSync();
    }

    void setStripColor(int r, int g, int b) {

        for (int i = 0; i < NUM_LEDS; i++) {
            leds[i] = CRGB(r, g, b);
        }

        showSync();
        Serial.printf("DEBUG:\tSet strip color to R: %d, G: %d, B: %d\n", r, g, b);
    }

    void on() {
        // Set all LEDs to the state they had before beign turned off        
        for (int i = 0; i < NUM_LEDS; i++) {
            leds[i] = leds_bkp[i];
        }

        showSync();
    }

    void off() {
        // Save the state of the strip
        for (int i = 0; i < NUM_LEDS; i++) {
            leds_bkp[i] = leds[i];
        }

        // Turn all the lights off
        for (int i = 0; i < NUM_LEDS; i++) {
            leds[i] = CRGB::Black;
        }

        showSync();
    }

    void showSync() {

        while (true) {
            thisTime = millis();
            if (thisTime - lastTime > 1000 / FPS) { 
                lastTime = thisTime;
                FastLED.show();
                break;
            }
            delay(1);
        }
    }

    void loopHook() {
    }
};

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
