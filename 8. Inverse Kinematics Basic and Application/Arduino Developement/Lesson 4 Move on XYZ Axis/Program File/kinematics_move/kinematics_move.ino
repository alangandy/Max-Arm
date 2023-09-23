#include "ESPMax.h"
#include "_espmax.h"

// 逆运动学三轴移动例程

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
    pos[0] = x; pos[1] = y; pos[2] = z-50;
    set_position(pos,1000); // Z轴相对初始位置下移50毫米
    delay(1000);

    pos[0] = x-50; pos[1] = y; pos[2] = z-50;
    set_position(pos,1000); // X轴相对初始位置左移50毫米
    delay(1000);
    pos[0] = x+50; pos[1] = y; pos[2] = z-50;
    set_position(pos,2000); // X轴相对初始位置右移50毫米
    delay(2000);
    pos[0] = x; pos[1] = y; pos[2] = z-50;
    set_position(pos,1000); // X轴回到初始位置
    delay(1000);

    pos[0] = x; pos[1] = y-50; pos[2] = z-50;
    set_position(pos,1000); // Y轴相对初始位置后移50毫米
    delay(1000);
    pos[0] = x; pos[1] = y+50; pos[2] = z-50;
    set_position(pos,2000); // Y轴相对初始位置前移50毫米
    delay(2000);
    pos[0] = x; pos[1] = y; pos[2] = z-50;
    set_position(pos,1000); // Y轴回到初始位置
    delay(1000);
    
    start_en = false;
  }
  else{
    delay(500); // 延时500毫秒
  }
}
