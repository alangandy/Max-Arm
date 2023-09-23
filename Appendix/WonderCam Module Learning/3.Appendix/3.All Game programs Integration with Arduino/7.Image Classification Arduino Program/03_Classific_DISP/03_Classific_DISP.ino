#include <Arduino.h>
#include "WonderCam.h"
#include <U8g2lib.h>
#include <Wire.h>


WonderCam wc;
U8G2_SSD1306_128X32_UNIVISION_1_HW_I2C u8g2(U8G2_R0, /* reset=*/ U8X8_PIN_NONE);   // Adafruit ESP8266/32u4/ARM Boards + FeatherWing OLED


void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);
  wc.begin();
  Serial.println("START");
  wc.changeFunc(APPLICATION_CLASSIFICATION);
  wc.setLed(true);

  u8g2.begin();
  u8g2.enableUTF8Print();    // enable UTF8 support for the Arduino print() function
}



void loop() {
  wc.updateResult();
  int class_id = wc.classIdOfMaxProb();
  float prob = wc.classMaxProb();
  if (prob > 0.4) {
    if (class_id < 2) {
    u8g2.setFont(u8g2_font_unifont_t_chinese2);
    u8g2.setFontDirection(0);
    u8g2.firstPage();
    do {
      u8g2.setCursor(0, 15);
      u8g2.print("None");
    } while ( u8g2.nextPage() );
    } else if (class_id < 5) {
      u8g2.setFont(u8g2_font_unifont_t_chinese2);
      u8g2.setFontDirection(0);
      u8g2.firstPage();
      do {
        u8g2.setCursor(0, 15);
        u8g2.print("Harmful Waste");
      } while ( u8g2.nextPage() );
    } else if (class_id < 8) {
      u8g2.setFont(u8g2_font_unifont_t_chinese2);
      u8g2.setFontDirection(0);
      u8g2.firstPage();
      do {
        u8g2.setCursor(0, 15);
        u8g2.print("Recyclable waste");
      } while ( u8g2.nextPage() );
    } else if (class_id < 11) {
      u8g2.setFont(u8g2_font_unifont_t_chinese2);
      u8g2.setFontDirection(0);
      u8g2.firstPage();
      do {
        u8g2.setCursor(0, 15);
        u8g2.print("Wet garbage");
      } while ( u8g2.nextPage() );
    } else {
      u8g2.setFont(u8g2_font_unifont_t_chinese2);
      u8g2.setFontDirection(0);
      u8g2.firstPage();
      do {
        u8g2.setCursor(0, 15);
        u8g2.print("Dry garbage");
      } while ( u8g2.nextPage() );
    }
  }
  delay(100);
}
