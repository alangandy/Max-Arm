#include "PID.h"
#include "ESPMax.h"
#include "Buzzer.h"
#include "TM1640.h"
#include "WonderCam.h"
#include "SuctionNozzle.h"
#include "ESP32PWMServo.h"

// 小幻熊颜色追踪并显示

WonderCam cam;

arc::PID<double> x_pid(0.030, 0.001, 0.0012);  // 设置PID参数
arc::PID<double> y_pid(0.030, 0.001, 0.0002);

TM1640 Matrix(32, 33);
uint8_t empty_buf[16] = { 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0 };
uint8_t red_buf[16] = { 0x0, 0x0, 0x0, 0x0, 0x0, 0xff, 0x19, 0x29, 0x49, 0x86, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0 };
uint8_t green_buf[16] = { 0x0, 0x0, 0x0, 0x0, 0x0, 0x3c, 0x42, 0x81, 0x81, 0xa1, 0x62, 0x0, 0x0, 0x0, 0x0, 0x0 };
uint8_t blue_buf[16] = { 0x0, 0x0, 0x0, 0x0, 0x0, 0xff, 0x89, 0x89, 0x89, 0x76, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0 };

void setup() {
  cam.begin();
  Buzzer_init();
  ESPMax_init();
  Nozzle_init();
  PWMServo_init();
  Valve_on();
  SetPWMServo(1, 1500, 1000);  // 吸嘴角度置0
  Valve_off();
  setBuzzer(100);  // 设置蜂鸣器响100ms
  Serial.begin(115200);
  Serial.println("start...");
  Matrix.setDisplay(empty_buf, 16);         // 点阵清屏
  cam.changeFunc(APPLICATION_COLORDETECT);  // 设置为颜色识别功能
}

void loop() {
  float pos[3];
  pos[0] = 0;
  pos[1] = -120;
  pos[2] = 150;
  set_position(pos, 1500);
  delay(1500);
  int display_st = 0;

  while (true) {
    int color_x = 160;
    int color_y = 120;
    cam.updateResult();            // 更新小幻熊结果数据
    if (cam.anyColorDetected()) {  // 识别到颜色
      WonderCamColorDetectResult p;
      if (cam.colorIdDetected(1)) {  // 判断是否识别id1颜色
        cam.colorId(1, &p);          // 获取id1颜色位置数据
        color_x = p.x;
        color_y = p.y;
        if (display_st != 1) {
          display_st = 1;
          Matrix.setDisplay(red_buf, 16);  // 点阵显示‘R’
        }
      } else if (cam.colorIdDetected(2)) {  // 判断是否识别id2颜色
        cam.colorId(2, &p);                 // 获取id2颜色位置数据
        color_x = p.x;
        color_y = p.y;
        if (display_st != 2) {
          display_st = 2;
          Matrix.setDisplay(green_buf, 16);  // 点阵显示‘G’
        }
      } else if (cam.colorIdDetected(3)) {  // 判断是否识别id3颜色
        cam.colorId(3, &p);                 // 获取id3颜色位置数据
        color_x = p.x;
        color_y = p.y;
        if (display_st != 3) {
          display_st = 3;
          Matrix.setDisplay(blue_buf, 16);  // 点阵显示‘B’
        }
      }

      if (abs(color_x - 160) < 15) {  // X轴PID算法追踪
        color_x = 160;
      }
      x_pid.setTarget(160);
      x_pid.setInput(color_x);
      pos[0] -= x_pid.getOutput();

      if (abs(color_y - 120) < 10) {  // Y轴PID算法追踪
        color_y = 120;
      }
      y_pid.setTarget(120);
      y_pid.setInput(color_y);
      pos[2] += y_pid.getOutput();

      if (pos[0] > 100) pos[0] = 100;  // 机械臂X轴范围限幅
      if (pos[0] < -100) pos[0] = -100;
      if (pos[2] > 180) pos[2] = 180;  // 机械臂Z轴范围限幅
      if (pos[2] < 100) pos[2] = 100;
      set_position(pos, 50);  // 驱动机械臂
      delay(50);

    } else {
      if (display_st != 0) {
        display_st = 0;
        Matrix.setDisplay(empty_buf, 16);  // 点阵清屏
      }
    }
  }
}