#include <Arduino.h>
#include "WonderCam.h"
#include "WMMatrixLed.h"

WonderCam wc;
WMMatrixLed matrixLed(7, 6);  //点阵实例化

void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);
  wc.begin();
  Serial.println("START");
  wc.changeFunc(APPLICATION_FACEDETECT);
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

  if (wc.anyFaceDetected()) { //是否识别到了人脸
    for (int i = 1; i < 6; i++)
    {
      if (wc.faceOfIdDetected(i)) {
        Serial.println("识别到特定人脸");
        char str[20] = "ID:";
        str[4] = '\0';
        str[3] = 48 + i;
        int offset = 0;
        while (offset > (-(6 * 4))) {
          matrixLed.drawStr(offset, 8, str);
          offset -= 1;
          delay(200);
        }
      }
    }
  }
  delay(200);
}
