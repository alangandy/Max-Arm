#include "Buzzer.h" // 引用蜂鸣器驱动库

// 按键控制例程

void setup(){
    Buzzer_init(); // 初始化蜂鸣器驱动库
}

bool start_en = true;
void loop(){
  if(start_en){
    setBuzzer(100); // 设置蜂鸣器响100毫秒
    delay(1000);  // 延时1000毫秒
    setBuzzer(300); // 设置蜂鸣器响300毫秒
    delay(1000);
    start_en = false;
  }
  else{
    delay(500); // 延时500毫秒
  }
}
