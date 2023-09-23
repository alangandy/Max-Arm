#include "SuctionNozzle.h" // 引用吸嘴驱动库

// 控制气泵例程

void setup() {
  // put your setup code here, to run once:
  Nozzle_init(); // 初始化驱动库
  Serial.begin(9600); // 设置串口波特率
  Serial.println("start...");
}

bool start_en = true;
void loop() {
  // put your main code here, to run repeatedly:
  if(start_en){
    Pump_on(); // 打开气泵
    delay(2000); // 延时2000毫秒
    Valve_on(); // 打开电磁阀,关闭气泵
    delay(500);
    Valve_off(); // 关闭电磁阀
    delay(2000);
    start_en = false;
  }
  else{
    delay(500); // 延时500毫秒
  }
}
