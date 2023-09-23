#include "PID.h"
#include "ESPMax.h"
#include "Buzzer.h"
#include "TM1640.h"
#include "WonderCam.h"
#include "FanModule.h"
#include "SuctionNozzle.h"
#include "ESP32PWMServo.h"

// 小幻熊人脸追踪风扇

WonderCam cam;

arc::PID<double> x_pid(0.030, 0.001, 0.0012);  // 设置PID参数
arc::PID<double> y_pid(0.030, 0.001, 0.0002);

TM1640 Matrix(32, 33);
uint8_t empty_buf[16] = { 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0 };
uint8_t smiling_buf[16] = { 0x0, 0xc, 0x2, 0x19, 0x21, 0x42, 0x80, 0x80, 0x80, 0x80, 0x42, 0x21, 0x19, 0x2, 0xc, 0x0 };


void setup() {
  cam.begin();
  Buzzer_init();
  ESPMax_init();
  Nozzle_init();
  PWMServo_init();
  FanModule_init();
  Valve_on();
  SetPWMServo(1, 1500, 1000); // 吸嘴角度置0 
  Valve_off();
  setBuzzer(100);  // 设置蜂鸣器响100ms
  Serial.begin(115200);
  Serial.println("start...");
  Matrix.setDisplay(empty_buf, 16);        // 点阵清屏
  cam.changeFunc(APPLICATION_FACEDETECT);  // 设置为人脸识别功能
}

void loop() {
  float pos[3];
  pos[0] = 0;
  pos[1] = -120;
  pos[2] = 150;
  set_position(pos, 1500);
  delay(1500);
  bool display_st = false;

  while (true) {
    cam.updateResult();           // 更新小幻熊结果数据
    if (cam.anyFaceDetected()) {  // 识别到人脸
      if (!display_st) {          // 判断风扇之前是否是关闭状态，避免频繁发送开启指令
        display_st = true;
        FanModule_on();                      // 开启风扇模块
        Matrix.setDisplay(smiling_buf, 16);  // 点阵显示笑脸
      }
      WonderCamFaceDetectResult p;  // 获取人脸坐标数据
      cam.getFaceOfIndex(1, &p);
      int face_x = p.x;
      int face_y = p.y;

      if (abs(face_x - 160) < 15) {  // X轴PID算法追踪
        face_x = 160;
      }
      x_pid.setTarget(160);
      x_pid.setInput(face_x);
      pos[0] -= x_pid.getOutput();

      if (abs(face_y - 120) < 10) {  // Y轴PID算法追踪
        face_y = 120;
      }
      y_pid.setTarget(120);
      y_pid.setInput(face_y);
      pos[2] += y_pid.getOutput();

      if (pos[0] > 100) pos[0] = 100;  // 机械臂X轴范围限幅
      if (pos[0] < -100) pos[0] = -100;
      if (pos[2] > 180) pos[2] = 180;  // 机械臂Z轴范围限幅
      if (pos[2] < 100) pos[2] = 100;
      set_position(pos, 50);  // 驱动机械臂
    } else {                  // 未识别到人脸
      if (display_st) {
        display_st = false;
        FanModule_off();                   // 关闭风扇模块
        Matrix.setDisplay(empty_buf, 16);  // 点阵清屏
      }
    }
    delay(50);
  }
}