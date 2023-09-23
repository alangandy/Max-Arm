// 按键检测例程

int pushButton = 25; // 定义按键引脚

void setup() {
  Serial.begin(9600); // 设置串口波特率
  Serial.println("start...");
  pinMode(pushButton, INPUT); // 设置按键引脚模式为输入
}


void loop() {
  int buttonState = digitalRead(pushButton); // 读取按键值
  if(buttonState == 0){ // 当按键按下时，为低电平
    delay(10); // 延时10ms消除抖动
    if(buttonState == 0){ // 再次判断按键是否按下
      Serial.println("hello world"); // 串口输出hello world
      delay(500);
    } 
  }
  delay(10);   
}
