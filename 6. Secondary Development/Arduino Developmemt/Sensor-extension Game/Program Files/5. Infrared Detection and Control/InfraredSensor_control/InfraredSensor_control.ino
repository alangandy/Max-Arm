#include "Buzzer.h"
#include "ESPMax.h"
#include "_espmax.h"
#include "ESP32PWMServo.h"
#include "SuctionNozzle.h"
#include "LobotSerialServoControl.h"

// 红外检测控制

#define sensor_pin  23 // 定义红外检测传感器引脚

void setup(){
    // 初始化驱动库
    Buzzer_init();
    ESPMax_init();
    Nozzle_init();
    PWMServo_init();
    pinMode(sensor_pin, INPUT_PULLUP); // 设置传感器引脚为内置上拉模式
    Serial.begin(115200);
    Serial.println("start...");
    setBuzzer(100); // 设置蜂鸣器响100毫秒
    go_home(2000); // 设置机械臂运行到初始位置
    SetPWMServo(1,1500,2000); // 设置吸嘴舵机运行到初始位置
}

void loop(){
    float pos[3];
    // 设置多次检测
    float sensor_state = 0.0;
    for(int i=0; i<5; i++){ 
      sensor_state += digitalRead(sensor_pin);
      delay(50); // 延时50毫秒
    }
    if(sensor_state == 0.0){ // 红外传感器检测到目标会把引脚设置为低电平
      setBuzzer(100); //设置蜂鸣器响100ms
      pos[0] = 0;pos[1] = -160;pos[2] = 100;
      set_position(pos,1500); //到色块上方
      delay(1500);
      pos[0] = 0;pos[1] = -160;pos[2] = 85;
      set_position(pos,800);  //吸取色块
      Pump_on();  //打开气泵
      delay(1000);
      pos[0] = 0;pos[1] = -160;pos[2] = 200;
      set_position(pos,1000);  //机械臂抬起来
      delay(1000);
      pos[0] = 70;pos[1] = -150;pos[2] = 200;
      set_position(pos,800);   //到放置区上方
      delay(800);
      SetPWMServo(1, 2200, 500);
      delay(200);
      pos[0] = 70;pos[1] = -150;pos[2] = 90;
      set_position(pos,800);   //到放置区
      delay(800);
      pos[0] = 130;pos[1] = -150;pos[2] = 88;
      set_position(pos,500);   //向左推一下
      delay(500);
      Valve_on();   //关闭气泵，打开电磁阀
      pos[0] = 130;pos[1] = -150;pos[2] = 200;
      set_position(pos,1000);  //机械臂抬起来
      delay(1000);
      Valve_off();   //关闭电磁阀
      go_home(1500); //机械臂复位
      delay(200);
      SetPWMServo(1, 1500, 500);
      delay(1500);
    }
    else{
      delay(100);
    }
}
