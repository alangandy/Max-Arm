#include "Buzzer.h"
#include "ESPMax.h"
#include "_espmax.h"
#include "ESP32PWMServo.h"
#include "SuctionNozzle.h"
#include "LobotSerialServoControl.h"

// 双红外检测分拣

#define infrared_left 23   // 定义左红外检测传感器引脚
#define infrared_right 32  // 定义右红外检测传感器引脚

void setup() {
  // 初始化驱动库
  Buzzer_init();
  ESPMax_init();
  Nozzle_init();
  PWMServo_init();
  pinMode(infrared_left, INPUT_PULLUP);  // 设置传感器引脚为内置上拉模式
  pinMode(infrared_right, INPUT_PULLUP);
  Serial.begin(115200);
  Serial.println("start...");
  setBuzzer(100);              // 设置蜂鸣器响100毫秒
  go_home(2000);               // 设置机械臂运行到初始位置
  SetPWMServo(1, 1500, 2000);  // 设置吸嘴舵机运行到初始位置
}

void loop() {
  float pos[3];
  // 设置多次检测
  float sensor_left = 0.0;
  float sensor_right = 0.0;

  for (int i = 0; i < 5; i++) {
    sensor_left += digitalRead(infrared_left); //读取传感器数值
    sensor_right += digitalRead(infrared_right);
    delay(50);  // 延时50毫秒
  }

  if (sensor_left == 0.0) {  // 红外传感器检测到目标会把引脚设置为低电平
    Serial.println("infrared_left");
    setBuzzer(100);  //设置蜂鸣器响100ms
    pos[0] = 70;
    pos[1] = -165;
    pos[2] = 120;
    set_position(pos, 1500);  //到色块上方
    delay(1500);
    pos[0] = 70;
    pos[1] = -165;
    pos[2] = 86;
    set_position(pos, 800);  //吸取色块
    Pump_on();               //打开气泵
    delay(1000);
    pos[0] = 70;
    pos[1] = -165;
    pos[2] = 200;
    set_position(pos, 1000);  //机械臂抬起来
    delay(1000);
    pos[0] = 150;
    pos[1] = -35;
    pos[2] = 200;
    set_position(pos, 800);  //到放置区上方
    delay(800);
    SetPWMServo(1, 1800, 500);
    delay(200);
    pos[0] = 150;
    pos[1] = -35;
    pos[2] = 90;
    set_position(pos, 800);  //到放置区
    delay(800);
    pos[0] = 150;
    pos[1] = 10;
    pos[2] = 88;
    set_position(pos, 500);  //移动一下进行放置
    delay(500);
    Valve_on();  //关闭气泵，打开电磁阀
    pos[0] = 150;
    pos[1] = 10;
    pos[2] = 200;
    set_position(pos, 1000);  //机械臂抬起来
    delay(1000);
    Valve_off();    //关闭电磁阀
    go_home(1500);  //机械臂复位
    delay(200);
    SetPWMServo(1, 1500, 500);
    delay(1500);

  } else if (sensor_right == 0.0) {  // 红外传感器检测到目标会把引脚设置为低电平
    Serial.println("infrared_right");
    setBuzzer(100);  //设置蜂鸣器响100ms
    pos[0] = -70;
    pos[1] = -165;
    pos[2] = 120;
    set_position(pos, 1500);  //到色块上方
    delay(1500);
    pos[0] = -70;
    pos[1] = -165;
    pos[2] = 86;
    set_position(pos, 800);  //吸取色块
    Pump_on();               //打开气泵
    delay(1000);
    pos[0] = -70;
    pos[1] = -165;
    pos[2] = 200;
    set_position(pos, 1000);  //机械臂抬起来
    delay(1000);
    pos[0] = -150;
    pos[1] = -35;
    pos[2] = 200;
    set_position(pos, 800);  //到放置区上方
    delay(800);
    SetPWMServo(1, 1200, 500);
    delay(200);
    pos[0] = -150;
    pos[1] = -35;
    pos[2] = 90;
    set_position(pos, 800);  //到放置区
    delay(800);
    pos[0] = -150;
    pos[1] = 10;
    pos[2] = 88;
    set_position(pos, 500);  //移动一下进行放置
    delay(500);
    Valve_on();  //关闭气泵，打开电磁阀
    pos[0] = -150;
    pos[1] = 10;
    pos[2] = 200;
    set_position(pos, 1000);  //机械臂抬起来
    delay(1000);
    Valve_off();    //关闭电磁阀
    go_home(1500);  //机械臂复位
    delay(200);
    SetPWMServo(1, 1500, 500);
    delay(1500);
  } else {
    delay(100);
  }
}
