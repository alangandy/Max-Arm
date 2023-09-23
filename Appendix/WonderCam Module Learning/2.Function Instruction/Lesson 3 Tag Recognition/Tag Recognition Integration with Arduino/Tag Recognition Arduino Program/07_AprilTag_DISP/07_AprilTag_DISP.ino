#include <Arduino.h>
#include "WonderCam.h"
#include "WMMatrixLed.h"
#include <string.h>
#include <stdlib.h>

WonderCam wc;
WMMatrixLed matrixLed(7, 6);  //点阵实例化

void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);
  wc.begin();
  Serial.println("START");
  wc.changeFunc(APPLICATION_APRILTAG);
  wc.setLed(true);
  matrixLed.setBrightness(3);
  matrixLed.clearScreen();

  uint8_t drawBuffer[16] = {
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00
    , 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00
  };
  for (int i = 0; i < 16; i++) {
    drawBuffer[i] = 0xff;
    matrixLed.drawBitmap(0, 0, 16, drawBuffer);
    delay(20);
  }
  delay(500);
  matrixLed.clearScreen();
}



void loop() {
  wc.updateResult();
char str[20];
  if (wc.anyTagDetected()) { //是否识别到了条形码
    for (int i = 0; i < 200; i++) {
      if (wc.tagIdDetected(i)) {
        sprintf(str, "ID:%d", i); 
        int offset = 16;
        int len = strlen(str);
        while (offset > (-(6 * (len)))) {
          matrixLed.drawStr(offset, 8, str);
          offset -= 1;
          delay(150);
        }
      }
    }
  } else {
    Serial.println("未扫描到条形码");
  }
  delay(200);
}
