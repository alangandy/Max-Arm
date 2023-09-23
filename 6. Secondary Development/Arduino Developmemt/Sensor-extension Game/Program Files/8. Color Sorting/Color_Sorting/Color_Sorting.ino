#include "ESPMax.h"
#include "Buzzer.h"
#include "Ultrasound.h"
#include "ESP32PWMServo.h"
#include "SuctionNozzle.h"
#include "Arduino_APDS9960.h"

// 颜色分拣

Ultrasound ultrasound;  //实例化超声波类

#define RED   1
#define GREEN 2
#define BLUE  3

int r_f = 30;
int g_f = 50;
int b_f = 50;
int R_F = 3000;
int G_F = 2600;
int B_F = 3500;

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
  PWMServo_init();
  Valve_on();
  go_home(2000);
  Valve_off();
  delay(100);
  SetPWMServo(1, 1500, 1000);
  Serial.begin(115200);
  Serial.println("start...");
  if (!APDS.begin()) {
    Serial.println("Error initializing APDS-9960 sensor.");
  }
  ultrasound.Color(255, 255, 255, 255, 255, 255); // 关闭发光超声波模块RGB灯光
}

// 定义变量
int x, y, z;
int angle_pul = 1500;
int detect_color = 0;
bool color_detect = true;

void loop() {
  float pos[3];
  int distance = 0;

  if (color_detect) { // 检测颜色阶段
    if (ColorDetect()) { // 判断颜色检测传感器有没有检测到颜色
      float color_num = 0.0;
      for (int i = 0; i < 5; i++) {
        color_num += ColorDetect(); // 多次检测，避免误识别
        delay(80);
      }
      color_num = color_num / 5.0; // 检测结果取平均值，如果不是整数，说明检测结果不稳定
      color_detect = false;
      if (color_num == 1.0) {
        // 设置红色放置区坐标
        x = 120;
        y = -140;
        z = 85;
        angle_pul = 2200; // 设置角度补偿脉宽
        detect_color = RED;
        Serial.println("Red"); // 检测到红色，打印‘Red’
        ultrasound.Color(255, 0, 0, 255, 0, 0); // 超声波亮红色
      }
      else if (color_num == 2.0) {
        // 设置绿色放置区坐标
        x = 120;
        y = -80;
        z = 85;
        angle_pul = 2000; // 设置角度补偿脉宽
        detect_color = GREEN;
        Serial.println("Green"); // 检测到绿色，打印‘Green’
        ultrasound.Color(0, 255, 0, 0, 255, 0); // 超声波亮绿色
      }
      else if (color_num == 3.0) {
        // 设置蓝色放置区坐标
        x = 120;
        y = -20;
        z = 82;
        angle_pul = 1800; // 设置角度补偿脉宽
        detect_color = BLUE;
        Serial.println("Blue"); // 检测到蓝色，打印‘Blue’
        ultrasound.Color(0, 0, 255, 0, 0, 255); // 超声波亮蓝色
      }
      else { // 检测结果不是整数，不做其他操作
        detect_color = 0;
        color_detect = true;
        ultrasound.Color(255, 255, 255, 255, 255, 255);
      }
    }
    else { // 没有检测到颜色
      if (color_detect) detect_color = 0;
      ultrasound.Color(255, 255, 255, 255, 255, 255);
      delay(200);
    }
  }
  else { // 超声波检测距离阶段
    for (int i = 0; i < 5; i++) {
      distance += ultrasound.GetDistance(); //读取超声波测值
      delay(100);
    }
    int dis = int(distance / 5); //取平均值
    Serial.print("Distance:");
    Serial.println(dis);
    if (60 < dis & dis < 80) { //判断超声波检测距离在60~80mm范围内
      if (detect_color) {
        setBuzzer(100); //设置蜂鸣器响100ms
        delay(1000);
        pos[0] = 0; pos[1] = -160; pos[2] = 100;
        set_position(pos, 1500); //到色块上方
        delay(1500);
        pos[0] = 0; pos[1] = -160; pos[2] = 85;
        set_position(pos, 800); //吸取色块
        Pump_on();  //打开气泵
        delay(1000);
        pos[0] = 0; pos[1] = -160; pos[2] = 180;
        set_position(pos, 1000); //机械臂抬起来
        delay(1000);
        pos[0] = x; pos[1] = y; pos[2] = 180;
        set_position(pos, 1500);  //到放置区上方
        delay(1500);
        SetPWMServo(1, angle_pul, 800); //设置角度补偿
        delay(200);
        pos[0] = x; pos[1] = y; pos[2] = z;
        set_position(pos, 1000);  //到放置区
        delay(1000);
        Valve_on();   //关闭气泵，打开电磁阀
        pos[0] = x; pos[1] = y; pos[2] = 200;
        set_position(pos, 1000); //机械臂抬起来
        delay(1000);
        Valve_off();   //关闭电磁阀
        go_home(1500); //机械臂复位
        SetPWMServo(1, 1500, 800);
        delay(200);
        detect_color = 0;
        color_detect = true;
        ultrasound.Color(255, 255, 255, 255, 255, 255);
        delay(1500);
      }
    }
  }
}
