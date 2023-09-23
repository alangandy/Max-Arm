#include <Arduino.h>
#include "WonderCam.h"
#include <Adafruit_NeoPixel.h>

WonderCam wc;
Adafruit_NeoPixel pixels(2, 6, NEO_GRB + NEO_KHZ800);

void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);
  wc.begin();
  Serial.println("START");
  // Serial.println(a);
  wc.changeFunc(APPLICATION_COLORDETECT);
  pixels.begin();
  pixels.clear(); // Set all pixel colors to 'off'
  pixels.show();
}

void show_color(uint8_t r, uint8_t g, uint8_t b) {
  for (int i = 0; i < 2; i++) { // For each pixel...
    // pixels.Color() takes RGB values, from 0,0,0 up to 255,255,255
    // Here we're using a moderately bright green color:
    pixels.setPixelColor(i, pixels.Color(r, g, b));
  }
  pixels.show();   // Send the updated pixel colors to the hardware.
}

void loop() {
  wc.updateResult();
  if (wc.anyColorDetected()) {
    for (int i = 1; i < 4; ++i) {
      if (wc.colorIdDetected(i)) {
        switch (i) {
          case 1:
            show_color(255, 0, 0);
            return;
          case 2:
            show_color(0, 255, 0);
            return;
          case 3:
            show_color(0, 0, 255);
            return;
          default:
            return;
        }
      }
    }
  } else {
    pixels.clear(); // Set all pixel colors to 'off'
    pixels.show();
    Serial.println("未识别到颜色");
  }
  delay(100);
}
