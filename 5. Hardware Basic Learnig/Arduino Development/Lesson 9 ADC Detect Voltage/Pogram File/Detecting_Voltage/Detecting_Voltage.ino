// ADC检测电压例程

#define DetectingPin 39 // 检测引脚

void setup() {
  // put your setup code here, to run once:
  Serial.begin(115200);        // 设置串口波特率
  Serial.println("start...");  // 串口打印"start..."
  delay(500); // 延时500毫秒
}

void loop() {
  // put your main code here, to run repeatedly:
  // 读取检测电压引脚的值，采用12位ADC转换，范围是0~4095
  float ReadValue = analogRead(DetectingPin); 
  // 引脚检测的满量程是3.3V，实际测得是3.2V，所以检测电路采用了分压检测，分压系数是0.25
  // 总电压 = 分压电压 / 分压系数
  float VoltageValue = ((ReadValue / 4095) * 3.2) / 0.25; // 按照公式求总电压
  Serial.println(VoltageValue); // 串口打印总电压
  delay(2000); // 延时2000毫秒
}
