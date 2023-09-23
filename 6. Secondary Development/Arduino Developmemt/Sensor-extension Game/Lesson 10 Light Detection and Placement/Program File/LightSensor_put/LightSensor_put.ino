#include "Buzzer.h"
#include "ESPMax.h"
#include "_espmax.h"
#include "ESP32PWMServo.h"
#include "SuctionNozzle.h"
#include "LobotSerialServoControl.h"

// 光线感应摆放

#define sensor_pin 32  // 定义光线传感器引脚

void setup() {
  // 初始化驱动库
  Buzzer_init();
  ESPMax_init();
  Nozzle_init();
  PWMServo_init();
  analogReadResolution(10);
  analogSetClockDiv(ADC_11db);
  Serial.begin(115200);
  Serial.println("start...");
  setBuzzer(100);              // 设置蜂鸣器响100毫秒
  go_home(2000);               // 设置机械臂运行到初始位置
  SetPWMServo(1, 1500, 2000);  // 设置吸嘴舵机运行到初始位置
}


void loop() {
  int num = 0;
  float pos[3];
  int angle_pul[3] = { 1600, 1800, 2000 };

  while (true) {
    float lightValue = analogRead(sensor_pin); // 读取光线传感器的数值，光线越强数值越小
    Serial.println(lightValue); // 串口打印传感器数值

    if (lightValue > 950) { // 传感器被挡
      Serial.print("num: ");
      Serial.println(num + 1);
      setBuzzer(100);  //设置蜂鸣器响100ms
      pos[0] = 0;
      pos[1] = -160;
      pos[2] = 100;
      set_position(pos, 1500);  //到色块上方
      delay(1500);
      pos[0] = 0;
      pos[1] = -160;
      pos[2] = 86;
      set_position(pos, 800);  //吸取色块
      Pump_on();               //打开气泵
      delay(1000);
      pos[0] = 0;
      pos[1] = -160;
      pos[2] = 180;
      set_position(pos, 1000);  //机械臂抬起来
      delay(1000);
      pos[0] = 120;
      pos[1] = (-20 - 60 * num);
      pos[2] = 180;
      set_position(pos, 1500);  //到放置区上方
      Serial.println(angle_pul[num]);
      delay(100);
      SetPWMServo(1, angle_pul[num], 1000);  // 设置角度补偿，使木块放正
      delay(500);
      pos[0] = 120;
      pos[1] = (-20 - 60 * num);
      pos[2] = 88;
      set_position(pos, 1000);  //到放置区
      delay(1200);
      Valve_on();  //关闭气泵，打开电磁阀
      pos[0] = 120;
      pos[1] = (-20 - 60 * num);
      pos[2] = 200;
      set_position(pos, 1000);  //机械臂抬起来
      delay(1000);
      Valve_off();    //关闭电磁阀
      go_home(1500);  //机械臂复位
      delay(100);
      SetPWMServo(1, 1500, 1500);  // 吸嘴角度复位
      num += 1;
      if (num >= 3) {
        num = 0;
        setBuzzer(100);  //设置蜂鸣器响100ms
        delay(100);
        setBuzzer(100);  //设置蜂鸣器响100ms
      }
    } else {
      delay(300);
    }
  }
}
