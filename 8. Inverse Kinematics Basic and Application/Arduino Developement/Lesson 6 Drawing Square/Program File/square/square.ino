#include "ESPMax.h"
#include "_espmax.h"

// 逆运动学画正方形例程

void setup(){
    ESPMax_init();
    go_home(2000); // 机械臂回到初始位置
    Serial.begin(9600);
    Serial.println("start...");
}

bool start_en = true;
void loop(){
  if(start_en){
    float pos[3];
    // set_position(pos,t), pos[0]: x轴坐标, pos[1]: y轴坐标, pos[2]: z轴坐标, t: 移动的总时间（时间越长，速度越慢）

    // 机械臂运行到起始点
    pos[0] = 50; pos[1] = -260; pos[2] = 80;
    set_position(pos,2000); 
    delay(3000);

    // 画上横边
    for(int i=50; i > -50; i -= 5){
      pos[0] = i; pos[1] = -260; pos[2] = 80;
      set_position(pos,30); 
      delay(30);
    }
    delay(500);

    // 画右竖边
    for(int i=-260; i < -160; i += 5){
      pos[0] = -50; pos[1] = i; pos[2] = 80-(26+i/10);
      set_position(pos,30); 
      delay(30);
    }
    delay(500);

    // 画下横边
    for(int i=-50; i < 50; i += 5){
      pos[0] = i; pos[1] = -160; pos[2] = 70;
      set_position(pos,30); 
      delay(30);
    }
    delay(500);

    // 画左竖边
    for(int i=-160; i > -260; i -= 5){
      pos[0] = 50; pos[1] = i; pos[2] = 80-(26+i/10);
      set_position(pos,30); 
      delay(30);
    }
    delay(500);
    
    go_home(2000); // 机械臂回到初始位置
    start_en = false;
  }
  else{
    delay(500); // 延时500毫秒
  }
}
