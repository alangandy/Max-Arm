// 控制ESP32板载LED闪烁例程

#define LED_BUILTIN 2 // 定义LED控制引脚

void setup() {
  // 初始化引脚LED_BUILTIN输出模式
  pinMode(LED_BUILTIN, OUTPUT); 
}

void loop() {
  digitalWrite(LED_BUILTIN, HIGH);   // 打开LED (HIGH为电压等级,设置为高电平)
  delay(1000);                       // 延时1000毫秒
  digitalWrite(LED_BUILTIN, LOW);    // 关闭LED (LOW为电压等级,设置为低电平)
  delay(1000);                       // 延时1000毫秒
}
