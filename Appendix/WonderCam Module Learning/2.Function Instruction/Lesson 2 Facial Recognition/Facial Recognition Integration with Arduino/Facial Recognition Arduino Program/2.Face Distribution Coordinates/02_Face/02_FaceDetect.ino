#include <Arduino.h>
#include "WonderCam.h"

WonderCam wc;

void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);
  wc.begin();
  Serial.println("START");
  // Serial.println(a);
  wc.changeFunc(APPLICATION_FACEDETECT);
  wc.setLed(true);
}

void loop() {
  wc.updateResult();
  if (wc.anyFaceDetected()) {
    int num = wc.numOfTotalFaceDetected();
    Serial.print("共检测到");
    Serial.print(num);
    Serial.println("张人脸");
    if (wc.faceOfIdDetected(1)) {
      Serial.print("检测到ID1人脸在");
      WonderCamFaceDetectResult p;
      wc.getFaceOfId(1, &p);
      Serial.print("X:");
      Serial.print(p.x);
      Serial.print(" Y:");
      Serial.println(p.y);
    }
    if(wc.anyUnlearnedFaceDetected()) {
      WonderCamFaceDetectResult p;
      wc.getFaceOfIndex(1, &p);
      Serial.print("检测到第一张未学习人脸,在X:");
      Serial.print(p.x);
      Serial.print(" Y:");
      Serial.println(p.y);
    }
  } else {
    Serial.println("未检测到人脸");
  }
  delay(500);
}
