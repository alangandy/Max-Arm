#include "ESPMax.h"
#include "Buzzer.h"
#include "WonderCam.h"
#include "SuctionNozzle.h"
#include "ESP32PWMServo.h"

// 小幻熊垃圾分类

// 实例化小幻熊库
WonderCam cam;

void setup() {
  // 初始化
  cam.begin();
  Buzzer_init();
  ESPMax_init();
  Nozzle_init();
  PWMServo_init();
  Valve_on();  // 打开吸嘴
  SetPWMServo(1, 1500, 1000);
  Valve_off();     // 关闭吸嘴
  setBuzzer(100);  // 设置蜂鸣器响100ms
  Serial.begin(115200);
  Serial.println("start...");
  cam.changeFunc(APPLICATION_CLASSIFICATION);  // 设置图像分类功能
}

void loop() {
  float num = 0;          // 累计量
  float result_data = 0;  // 结果缓存量
  float pos[3];
  float place[3];
  int angle_pwm = 1500;
  int move_time = 1500;
  pos[0] = 0;
  pos[1] = -120;
  pos[2] = 150;
  set_position(pos, 1500);
  delay(1500);

  while (true) {
    num += 1;
    cam.updateResult();                     // 更新检测结果
    result_data += cam.classIdOfMaxProb();  // 获得当前置信度最大的id,并缓存起来
    if (num == 30) {                        // 多次检测
      float class_id = result_data / num;  // 结果取平均值
      result_data = 0;
      num = 0;

      if (class_id != int(class_id)) {    // 判断结果是不是整数，不是整数说明识别不稳定
        class_id = 0;
        continue; // 跳过这一次循环，重新识别
      }
      if ((2 <= class_id) & (class_id <= 4)) { // 有害垃圾
        Serial.println("Hazardous waste"); 
        angle_pwm = 1900; // 设置放置补偿角度
        move_time = 1000; // 设置运行时间
        place[0] = -120; place[1] = -170; place[2] = 60; // 设置放置坐标位置
      }
      else if ((5 <= class_id) & (class_id <= 7)) { // 可回收物
        Serial.println("Recyclable material");
        angle_pwm = 2100;
        move_time = 1200;
        place[0] = -120; place[1] = -120; place[2] = 60;
      }
      else if ((8 <= class_id) & (class_id <= 10)) { // 厨余垃圾
        Serial.println("Kitchen garbage");
        angle_pwm = 2300;
        move_time = 1400;
        place[0] = -120; place[1] = -70; place[2] = 60;
      }
      else if ((11 <= class_id) & (class_id <= 13)) { // 其他垃圾
        Serial.println("Other garbage");
        angle_pwm = 2500;
        move_time = 1600;
        place[0] = -120; place[1] = -20; place[2] = 60;
      }
      else { // 检测到其他id
        continue;  // 跳过这一次循环，重新识别
      }

      int d_y = 65;
      pos[1] -= d_y; pos[2] = 100;
      set_position(pos, 1000); //到色块上方
      delay(1000);
      pos[2] = 50;
      set_position(pos, 600); //吸取色块
      Pump_on();  //打开气泵
      delay(1000);
      pos[2] = 150;
      set_position(pos, 1000); //机械臂抬起来
      delay(1000);
      pos[0] = place[0]; pos[1] = place[1];
      set_position(pos, move_time);  //到放置区上方
      delay(move_time);
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
      set_position(pos, move_time);; //机械臂复位
      SetPWMServo(1, 1500, move_time); // 吸嘴角度置0
      delay(move_time);
    }
    delay(50);  // 延时50毫秒
  }
}
