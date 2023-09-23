#include "ESPMax.h"
#include "Buzzer.h"
#include "TM1640.h"
#include "Ultrasound.h"
#include "SuctionNozzle.h"

// 超声波检测并数码管显示

TM1640 module(32, 33);
Ultrasound ultrasound;  //实例化超声波类

float distance;
int i;
uint16_t r;
uint16_t g;
uint16_t b;

void setup() {
  // 初始化
  Buzzer_init();
  ESPMax_init();
  Nozzle_init();
  Valve_on();
  go_home(2000);
  delay(2000);
  Valve_off();
  Serial.begin(115200);
  Serial.println("start...");
  //  ultrasound.Breathing(30, 50, 60, 20, 30, 50);  // 发光超声波模块幻彩模式
}

void loop() {
  char text[6];
  int distance = ultrasound.GetDistance();  // 读取超声波测值
  Serial.println(distance);  // 串口打印距离值单位mm
  sprintf(text, "%4d", distance);  // 把距离值转换成字符串
  module.setDisplayToString(text);  // 在数码管显示
  if (distance > 0 && distance <= 50)
    ultrasound.Color(0, 255, 0, 0, 255, 0); //Green
  else if (distance > 50 && distance <= 100)
    ultrasound.Color(255, 0, 0, 255, 0, 0); //Red
  else if (distance > 100)
    ultrasound.Color(0, 0, 255, 0, 0, 255); //Blue
  delay(300);  // 延时300毫秒

  // if(distance<5||(distance>7&&distance<12)||(distance>14&&distance<19)||distance>21)
  //    if ((distance > 0 && distance <= 50)||(distance > 50 && distance <= 100)||distance > 100)
  //    run();
  //rainbow_color();
}

void run()
{
  static uint32_t timer;
  //  if (timer < millis())
  //  {
  if (distance > 50 && distance <= 100)
    ultrasound.Color(0, 255, 0, 0, 255, 0); //Green
  else if (distance > 0 && distance <= 50)
    ultrasound.Color(255, 0, 0, 255, 0, 0); //Red
  else if (distance > 100)
    ultrasound.Color(0, 0, 255, 0, 0, 255); //Blue

  //      timer = millis() + 250;
  //    }
}

/*
  void rainbow_color()          //Gradient rainbow light
  {
  static uint32_t color_timer;
  if(color_timer < millis())
  {
      color_timer = millis()+120;
      if(i<33&&i>=0){
          r=255;
          g=7.65*i;
          b=0;
      }else if(i<50&&i>=33){
          r=750-15*i;
          g=255;
          b=0;
      }else if(i<=66&&i>=50){
          r=0;
          g=255;
          b=15*i-750;
      }else if(i<=83&&i>66){
          r=0;
          g=1250-15*i;
          b=255;
      }else if(i<=100&&i>83){
          r=9*i-750;
          g=0;
          b=255;
      }else{
          r=5*i-350;
          g=0;
          b=1500-12.5*i;
        }
      i++;
      if(i>120)
      i=0;
      ultrasound.Color(r, g,b, r, g, b);
     }
  }
*/
