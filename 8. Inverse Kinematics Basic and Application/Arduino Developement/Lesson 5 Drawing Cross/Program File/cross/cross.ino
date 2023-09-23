#include "ESPMax.h"
#include "_espmax.h"

// 逆运动学画十字例程

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

    // 画竖边
    pos[0] = 0; pos[1] = -120; pos[2] = 80;
    set_position(pos,1500); 
    delay(1500);

    pos[0] = 0; pos[1] = -280; pos[2] = 75;
    set_position(pos,1000); 
    delay(1200);

    pos[0] = 0; pos[1] = -280; pos[2] = 150;
    set_position(pos,500); 
    delay(800);

    // 切换到横边的左端上方
    pos[0] = 100; pos[1] = -200; pos[2] = 150;
    set_position(pos,1000); 
    delay(1200);
    // 切换到横边的左端
    pos[0] = 100; pos[1] = -200; pos[2] = 80;
    set_position(pos,500); 
    delay(600);
    // 画横边
    for(int i=100; i > -100; i -= 2){
      pos[0] = i; pos[1] = -200; pos[2] = 80;
      set_position(pos,5); 
      delay(5);
    }
    delay(500);
    pos[0] = -100; pos[1] = -200; pos[2] = 80;
    set_position(pos,1000); 
    delay(1000);

    go_home(2000); // 机械臂回到初始位置
    start_en = false;
  }
  else{
    delay(500); // 延时500毫秒
  }
}
