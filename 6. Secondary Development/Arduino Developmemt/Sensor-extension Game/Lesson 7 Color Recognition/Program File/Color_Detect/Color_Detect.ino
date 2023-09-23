#include "ESPMax.h"
#include "Buzzer.h"
#include "Ultrasound.h"
#include "ESP32PWMServo.h"
#include "SuctionNozzle.h"
#include "Arduino_APDS9960.h"

// 颜色识别

Ultrasound ultrasound; // 实例化超声波类

#define RED   1
#define GREEN 2
#define BLUE  3

int r_f = 30;
int g_f = 50;
int b_f = 50;
int R_F = 3000;
int G_F = 2600;
int B_F = 3500;

// 颜色检测函数
int ColorDetect() {
  // 颜色检测初始化延时
  while (!APDS.colorAvailable()) delay(5);
  // 定义变量
  int r, g, b, c;
  // 获取rgb三个颜色读数
  APDS.readColor(r, g, b);
  // 取值范围缩放
  r = map(r, r_f, R_F, 0, 255);
  g = map(g, g_f, G_F, 0, 255);
  b = map(b, b_f, B_F, 0, 255);

  //根据三个颜色读数的值，确定读数最大值为当前测量颜色，例如r值最大，则当前检测为红色
  if (r > g) c = RED;
  else c = GREEN;
  if (c == GREEN && g < b) c = BLUE;
  if (c == RED && r < b) c = BLUE;

  //当颜色读数大于50，返回当前颜色读数，否则返回0
  if (c == BLUE && b > 60) return c;
  else if (c == GREEN && g > 60) return c;
  else if (c == RED && r > 60) return c;
  else return 0;
}

void setup() {
  // 初始化
  Buzzer_init();
  ESPMax_init();
  Nozzle_init();
  Valve_on();
  go_home(2000);
  Valve_off();
  Serial.begin(115200);
  Serial.println("start...");
  if (!APDS.begin()) {
    Serial.println("Error initializing APDS-9960 sensor.");
  }
  ultrasound.Color(255, 255, 255, 255, 255, 255); // 发光超声波模块RGB灯光
}


void loop() {
  if (ColorDetect()) { // 判断颜色检测传感器有没有检测到颜色
    float color_num = 0.0;
    for (int i = 0; i < 5; i++) { 
      color_num += ColorDetect(); // 多次检测，避免误识别
      delay(80);
    }
    color_num = color_num / 5.0; // 检测结果取平均值，如果不是整数，说明检测结果不稳定
    if (color_num == 1.0) {
      Serial.println("Red"); // 检测到红色，打印‘Red’
      ultrasound.Color(255, 0, 0, 255, 0, 0); // 超声波亮红色
    }
    else if (color_num == 2.0) {
      Serial.println("Green"); // 检测到绿色，打印‘Green’
      ultrasound.Color(0, 255, 0, 0, 255, 0); // 超声波亮绿色
    }
    else if (color_num == 3.0) {
      Serial.println("Blue"); // 检测到蓝色，打印‘Blue’
      ultrasound.Color(0, 0, 255, 0, 0, 255); // 超声波亮蓝色
    }
    else { // 检测结果不是整数，不做其他操作
      ultrasound.Color(255, 255, 255, 255, 255, 255); 
    }
  }
  else { // 没有检测到颜色
    ultrasound.Color(255, 255, 255, 255, 255, 255);
    delay(500);
  }
}
