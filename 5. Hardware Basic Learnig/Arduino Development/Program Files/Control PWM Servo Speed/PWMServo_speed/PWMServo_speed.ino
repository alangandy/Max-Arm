#include "ESP32PWMServo.h" // 导入库文件

// 控制PWM舵机速度例程

void setup() {
  PWMServo_init(); // 初始化PWM舵机库
  Serial.begin(9600); // 设置串口波特率
  Serial.println("start...");  // 串口打印"start..."
  delay(200);               // 延时200毫秒
  
}

bool start_en = true;
void loop() {
  // put your main code here, to run repeatedly:
  if(start_en){
    SetPWMServo(1,500,2000);  // 设置1号PWM舵机脉宽500，运行时间2000毫秒
    delay(200);               // 延时200毫秒
    int t[2]= {500, 2000};
    for(int i=0; i<2; i++){ // 分别以不同时间运行一轮，时间越长速度越慢
      SetPWMServo(1,500,t[i]); // 设置1号PWM舵机脉宽500
      delay(200);               // 延时200毫秒
      SetPWMServo(1,2500,t[i]);  // 设置1号PWM舵机脉宽2500
      delay(200);               // 延时200毫秒
      SetPWMServo(1,500,t[i]); // 设置1号PWM舵机脉宽500
      delay(200);               // 延时200毫秒
    }   
    start_en = false;
  }
  else{
    delay(500); // 延时500毫秒
  }
}
