#include "ESPMax.h"
#include "Buzzer.h"
#include "Ultrasound.h"
#include "SuctionNozzle.h"
#include "ESP32PWMServo.h"

// 超声波检测吸取

Ultrasound ultrasound;  //实例化超声波类

void setup(){
    Buzzer_init();
    ESPMax_init();
    Nozzle_init();
    PWMServo_init();
    Valve_on();
    go_home(2000);
    delay(2000);
    SetPWMServo(1, 1500, 1000);
    Valve_off();
    Serial.begin(115200);
    Serial.println("start...");
    ultrasound.Breathing(30, 50, 60, 20, 30, 50); // 发光超声波模块幻彩模式
}

void loop(){
    float pos[3];
    int distance = 0;
    for(int i=0; i<5; i++){
        distance += ultrasound.GetDistance(); //读取超声波测值
        delay(200);
    }
    int dis = int(distance/5); //取平均值
    Serial.println(dis);
    if(60 < dis & dis < 80){  //判断超声波检测距离在60~80mm范围内
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
}
