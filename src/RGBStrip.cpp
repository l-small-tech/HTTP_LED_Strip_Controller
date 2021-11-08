#ifndef RGBSTRIP
#define RGBSTRIP
#endif

#include <FastLED.h>
#include <Arduino.h>

// ---- Parameters ----
#define DATA_PIN    2
#define NUM_LEDS    50
#define BRIGHTNESS  50
#define FPS         60
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
    
    void setBrightness(int b) {
        this->brightness = b;
        FastLED.setBrightness(b);
        FastLED.show();
    }

    void setStripColor(int r, int g, int b) {

        for (int i = 0; i < NUM_LEDS; i++) {
            leds[i] = CRGB(r, g, b);
        }
        FastLED.show();
    }

    void on() {
        // Set all LEDs to the state they had before beign turned off        
        for (int i = 0; i < NUM_LEDS; i++) {
            leds[i] = leds_bkp[i];
        }

        FastLED.show();
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

        FastLED.show();
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
};