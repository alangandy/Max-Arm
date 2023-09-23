#include "Buzzer.h"
#include "ESPMax.h"
#include "_espmax.h"
#include "ESP32PWMServo.h"
#include "SuctionNozzle.h"
#include "LobotSerialServoControl.h"

// 触控摆放

#define sensor_pin  23 // 定义触摸传感器引脚

void setup(){
    // 初始化驱动库
    Buzzer_init();
    ESPMax_init();
    Nozzle_init();
    PWMServo_init();
    pinMode(sensor_pin, INPUT_PULLUP); // 设置传感器引脚为内置上拉模式
    Serial.begin(9600);
    Serial.println("start...");
    setBuzzer(100); // 设置蜂鸣器响100毫秒
    go_home(2000); // 设置机械臂运行到初始位置
    SetPWMServo(1,1500,2000); // 设置吸嘴舵机运行到初始位置
}

int num = 0; // 木块计数变量
int angle_pul[3] = {1800,2000,2200};
void loop(){
    float pos[3];
    // 设置多次检测
    float sensor_state = 0.0;
    for(int i=0; i<3; i++){ 
      sensor_state += digitalRead(sensor_pin);
      delay(20); // 延时50毫秒
    }
    if(sensor_state == 0.0){ // 触摸传感器检测到目标会把引脚设置为低电平
      Serial.print("num: ");
      Serial.println(num+1);
      setBuzzer(100); //设置蜂鸣器响100ms
      pos[0] = 0;pos[1] = -160;pos[2] = 100;
      set_position(pos,1500); //到色块上方
      delay(1500);
      pos[0] = 0;pos[1] = -160;pos[2] = 85;
      set_position(pos,800);  //吸取色块
      Pump_on();  //打开气泵
      delay(1000);
      pos[0] = 0;pos[1] = -160;pos[2] = 180;
      set_position(pos,1000);  //机械臂抬起来
      delay(1000);
      pos[0] = 120;pos[1] = (-20-60*num);pos[2] = 180;
      set_position(pos,1500);   //到放置区上方
      Serial.println(angle_pul[num]);
      delay(100);
      SetPWMServo(1,angle_pul[num],1000); // 设置角度补偿，使木块放正
      delay(500);
      pos[0] = 120;pos[1] = (-20-60*num);pos[2] = (83+num);
      set_position(pos,1000);   //到放置区
      delay(1200);
      Valve_on();   //关闭气泵，打开电磁阀
      pos[0] = 120;pos[1] = (-20-60*num);pos[2] = 200;
      set_position(pos,1000);  //机械臂抬起来
      delay(1000);
      Valve_off();   //关闭电磁阀
      go_home(1500); //机械臂复位
      delay(100);
      SetPWMServo(1,1500,1500); // 吸嘴角度复位
      num += 1;
      if(num >= 3){
        num = 0;
        setBuzzer(100); //设置蜂鸣器响100ms
        delay(100);
        setBuzzer(100); //设置蜂鸣器响100ms
      }
    }
    else{
      delay(100);
    }
}
