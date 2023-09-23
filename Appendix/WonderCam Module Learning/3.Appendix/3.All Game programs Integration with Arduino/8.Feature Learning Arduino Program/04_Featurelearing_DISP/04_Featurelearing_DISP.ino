#include <Arduino.h>
#include "WonderCam.h"
#include <string.h>
#include <stdlib.h>
#include <U8g2lib.h>
#include <Wire.h>

WonderCam wc;
U8G2_SSD1306_128X32_UNIVISION_1_HW_I2C u8g2(U8G2_R0, /* reset=*/ U8X8_PIN_NONE);   // Adafruit ESP8266/32u4/ARM Boards + FeatherWing OLED


void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);
  wc.begin();
  wc.changeFunc(APPLICATION_FEATURELEARNING);
  u8g2.begin();
//  u8g2.enableUTF8Print();    // enable UTF8 support for the Arduino print() function

}



void loop() {
  wc.updateResult();
  int class_id = wc.featureIdOfMaxProb();
  float prob = wc.featureMaxProb();
  char str[20];
  if (prob > 0.4 && class_id != 0) {
    sprintf(str, "ID:%d", class_id);
    int offset = 14;
    int len = strlen(str);
//    u8g2.setFont(u8g2_font_unifont_t_chinese2);
  u8g2.setFont(u8g2_font_ncenB14_tr);
    u8g2.setFontDirection(0);
    u8g2.firstPage();
    do {
      u8g2.setCursor(0, 15);
      u8g2.print(str);
    } while ( u8g2.nextPage() );

  }
  delay(100);
}
