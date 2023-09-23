#include "PID.h"
#include "ESPMax.h"
#include "Buzzer.h"
#include "WonderCam.h"
#include "SuctionNozzle.h"
#include "ESP32PWMServo.h"

// 小幻熊颜色追踪并分拣

WonderCam cam;

arc::PID<double> x_pid(0.045, 0.0001, 0.0001);  // 设置PID参数
arc::PID<double> y_pid(0.045, 0.0001, 0.0001);

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
  cam.changeFunc(APPLICATION_COLORDETECT);  // 设置为颜色识别功能
}

void loop() {
  int i = 0;
  int angle_pwm = 0;
  float pos[3];
  float place[3];
  pos[0] = 0;
  pos[1] = -120;
  pos[2] = 150;
  set_position(pos, 1500);
  delay(1500);

  while (true) {
    int color_x = 160;
    int color_y = 120;
    cam.updateResult();            // 更新小幻熊结果数据
    if (cam.anyColorDetected()) {  // 识别到颜色
      WonderCamColorDetectResult p;
      if (cam.colorIdDetected(1)) {  // 判断是否识别id1颜色
        cam.colorId(1, &p);          // 获取id1颜色位置数据
        color_x = p.x; color_y = p.y;
        angle_pwm = 2100;
        place[0] = -120; place[1] = -140; place[2] = 85;

      } else if (cam.colorIdDetected(2)) {  // 判断是否识别id2颜色
        cam.colorId(2, &p);                 // 获取id2颜色位置数据
        color_x = p.x; color_y = p.y;
        angle_pwm = 2300;
        place[0] = -120; place[1] = -80; place[2] = 85;

      } else if (cam.colorIdDetected(3)) {  // 判断是否识别id3颜色
        cam.colorId(3, &p);                 // 获取id3颜色位置数据
        color_x = p.x; color_y = p.y;
        angle_pwm = 2500;
        place[0] = -120; place[1] = -20; place[2] = 85;
      }

      if (abs(color_x - 160) < 15) {  // X轴PID算法追踪
        color_x = 160;
      }
      x_pid.setTarget(160);
      x_pid.setInput(color_x);
      float dx = x_pid.getOutput();
      pos[0] -= dx;

      if (abs(color_y - 120) < 10) {  // Y轴PID算法追踪
        color_y = 120;
      }
      y_pid.setTarget(120);
      y_pid.setInput(color_y);
      float dy = y_pid.getOutput();
      pos[1] -= dy;

      if (pos[0] > 100) pos[0] = 100;  // 机械臂X轴范围限幅
      if (pos[0] < -100) pos[0] = -100;
      if (pos[1] > -60) pos[1] = -60;  // 机械臂Y轴范围限幅
      if (pos[1] < -240) pos[1] = -240;
      set_position(pos, 50);  // 驱动机械臂

      if ((abs(dx) < 0.1) & (abs(dy) < 0.1)) {
        i ++;
        if (i > 10) {
          i = 0;
          setBuzzer(100);  // 设置蜂鸣器响100ms
          float d_x = pos[0] / 2.3;
          float d_y = 68 - abs(d_x / 3);

          pos[0] += d_x; pos[1] -= d_y;
          set_position(pos, 1000); //到色块上方
          delay(1000);
          pos[2] = 85;
          set_position(pos, 600); //吸取色块
          Pump_on();  //打开气泵
          delay(1000);
          pos[2] = 150;
          set_position(pos, 1000); //机械臂抬起来
          delay(1000);
          pos[0] = place[0]; pos[1] = place[1];
          set_position(pos, 1200);  //到放置区上方
          delay(1200);
          SetPWMServo(1, angle_pwm, 800); //设置角度补偿
          delay(200);
          set_position(place, 1000);  //到放置区
          delay(1000);
          Valve_on();   //关闭气泵，打开电磁阀
          place[2] = 150;
          set_position(place, 1000); //机械臂抬起来
          delay(1000);
          Valve_off();   //关闭电磁阀
          pos[0] = 0; pos[1] = -120; pos[2] = 150;
          set_position(pos, 1500);; //机械臂复位
          SetPWMServo(1, 1500, 800); // 吸嘴角度置0
          delay(2000);
        }
      }
      delay(50);
    }
  }
}
