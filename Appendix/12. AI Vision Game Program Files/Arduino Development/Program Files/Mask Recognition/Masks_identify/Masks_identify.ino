#include "ESPMax.h"
#include "Buzzer.h"
#include "TM1640.h"
#include "WonderCam.h"
#include "SuctionNozzle.h"
#include "ESP32PWMServo.h"

// 小幻熊口罩识别并显示

// 实例化小幻熊库
WonderCam cam;
// 实例化点阵库
TM1640 Matrix(32, 33);
// 定义点阵显示数据
uint8_t empty_buf[16] = { 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0 };
uint8_t cross_buf[16] = { 0x0, 0x0, 0x0, 0x81, 0xc3, 0xe7, 0x7e, 0x3c, 0x3c, 0x7e, 0xe7, 0xc3, 0x81, 0x0, 0x0, 0x0 };
uint8_t smiling_buf[16] = { 0x0, 0xc, 0x2, 0x19, 0x21, 0x42, 0x80, 0x80, 0x80, 0x80, 0x42, 0x21, 0x19, 0x2, 0xc, 0x0 };

void setup() {
  // 初始化
  cam.begin();
  Buzzer_init();
  ESPMax_init();
  Nozzle_init();
  PWMServo_init();
  Valve_on();  // 打开吸嘴
  SetPWMServo(1, 1500, 1000);
  Valve_off();     // 关闭吸嘴
  setBuzzer(100);  // 设置蜂鸣器响100ms
  Serial.begin(115200);
  Serial.println("start...");
  Matrix.setDisplay(empty_buf, 16);            // 点阵清屏
  cam.changeFunc(APPLICATION_CLASSIFICATION);  // 设置图像分类功能
}

void loop() {
  float num = 0;          // 累计量
  float result_data = 0;  // 结果缓存量

  while (true) {
    num += 1;
    cam.updateResult();                     // 更新检测结果
    result_data += cam.classIdOfMaxProb();  // 获得当前置信度最大的id,并缓存起来

    if (num == 5) {                        // 多次检测
      float class_id = result_data / num;  // 结果取平均值
      result_data = 0; 
      num = 0;

      if (class_id == 2.0) {                 // id=2，则是戴口罩的
        Matrix.setDisplay(smiling_buf, 16);  // 点阵显示笑脸

      } else if (class_id == 3.0) {        // id=3，则是不戴口罩的
        Matrix.setDisplay(cross_buf, 16);  // 点阵显示叉

      } else {                             // 背景
        Matrix.setDisplay(empty_buf, 16);  // 点阵清屏
      }
    }
    delay(50);  // 延时50毫秒
  }
}