#include "ESPMax.h"
#include "_espmax.h"

// 逆运动基础例程

void setup(){
    ESPMax_init();
    go_home(2000); // 机械臂回到初始位置
    Serial.begin(9600);
    Serial.println("start...");
}

bool start_en = true;
void loop(){
  if(start_en){
    float x,y,z;
    float pos[3];
    // 机械臂初始位置的XYZ位置
    x = 0;
    y = -(L1 + L3 + L4);
    z = (L0 + L2);
    // 串口打印XYZ位置，单位毫米
    Serial.print(x);
    Serial.print("; ");
    Serial.print(y);
    Serial.print("; ");
    Serial.println(z);

    // 机械臂初始位置已经是处于机械臂可移动空间的边缘了，所以要先下移，否则机械臂是无法在X、Y轴上移动的
    // set_position(pos,t), pos={x,y,z}; x: x轴坐标, y: y轴坐标, z: z轴坐标, t: 移动的总时间（时间越长，速度越慢）
    
    pos[0] = x; pos[1] = y; pos[2] = z-100;
    set_position(pos,2000); // Z轴相对初始位置下移100毫米
    delay(2000);
    pos[0] = x; pos[1] = y; pos[2] = z;
    set_position(pos,2000); // 机械臂恢复初始姿态
    delay(1000);

    start_en = false;
  }
  else{
    delay(500); // 延时500毫秒
  }
}
