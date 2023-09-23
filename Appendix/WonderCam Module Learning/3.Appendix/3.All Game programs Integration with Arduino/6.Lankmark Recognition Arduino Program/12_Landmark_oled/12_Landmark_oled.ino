#include <Arduino.h>
#include "WonderCam.h"
#include "WMMatrixLed.h"
#include <U8g2lib.h>
#include <Wire.h>

WonderCam wc;
U8G2_SSD1306_128X32_UNIVISION_1_HW_I2C u8g2(U8G2_R0, /* reset=*/ U8X8_PIN_NONE);   // Adafruit ESP8266/32u4/ARM Boards + FeatherWing OLED

void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);
  wc.begin();
  Serial.println("START");
  wc.changeFunc(APPLICATION_LANDMARK_REC);
  wc.setLed(true);
  u8g2.begin();
  u8g2.enableUTF8Print();    // enable UTF8 support for the Arduino print() function
}


uint8_t no_counter = 0;
void loop() {
  wc.updateResult();

  char buf[100];
  int id = wc.landmarkIdWithMaxProb();
  float max_prob = wc.landmarkMaxProb();
  if (max_prob > 0.5 && id > 0) {
    switch (id) {
      case 1: {
          sprintf(buf, "Go");
          break;
        }
      case 2: {
          sprintf(buf, "Turn left");
          break;
        }
      case 3: {
          sprintf(buf, "Turn right");
          break;
        }
      case 4: {
          sprintf(buf, "Back");
          break;
        }
      case 5: {
          sprintf(buf, "Stop");
          break;
        }
      default: {
          sprintf(buf, "");
          break;
        }
    }
//    u8g2.setFont(u8g2_font_unifont_t_chinese2);
//    u8g2.setFontDirection(0);
    u8g2.firstPage();
    do {
      u8g2.setCursor(0, 15);
      u8g2.setFont(u8g2_font_ncenB10_tr);
      u8g2.print(buf);
    } while ( u8g2.nextPage() );
  }  else {
    no_counter++;
    if (no_counter > 2) {
      u8g2.clearDisplay();
    }
  }
  delay(100);
}
