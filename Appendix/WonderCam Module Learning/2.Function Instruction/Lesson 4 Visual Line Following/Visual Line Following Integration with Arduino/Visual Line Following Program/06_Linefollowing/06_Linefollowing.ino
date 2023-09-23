#include <Arduino.h>
#include "WonderCam.h"
#include "WMMatrixLed.h"
#include <U8g2lib.h>
#include <Wire.h>

WonderCam wc;
WMMatrixLed matrixLed(7, 6);  //点阵实例化
U8G2_SSD1306_128X32_UNIVISION_1_HW_I2C u8g2(U8G2_R0, /* reset=*/ U8X8_PIN_NONE);   // Adafruit ESP8266/32u4/ARM Boards + FeatherWing OLED

void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);
  wc.begin();
  Serial.println("START");
  wc.changeFunc(APPLICATION_LINEFOLLOW);
  wc.setLed(true);
  u8g2.begin();
  u8g2.enableUTF8Print();    // enable UTF8 support for the Arduino print() function
}


uint8_t no_counter = 0;
void loop() {
  wc.updateResult();

  char buf[100];
  if (wc.lineIdDetected(1)) { //是否识别到了线条
    WonderCamLineResult p;
    if (wc.lineId(1, &p)) {
      u8g2.setFont(u8g2_font_unifont_t_chinese2);
      u8g2.setFontDirection(0);
      u8g2.setDrawColor(1);
      u8g2.firstPage();
      do {
        u8g2.drawLine(map(p.start_x, 0, 319, 0, 127), map(p.start_y, 0, 239, 0, 31), map(p.end_x, 0, 319, 0, 127), map(p.end_y, 0, 239, 0, 31));
      } while ( u8g2.nextPage() );
    }  else {
      no_counter++;
      if (no_counter > 5) {
        u8g2.clearDisplay();
      }
    }
  }
  delay(50);
}
