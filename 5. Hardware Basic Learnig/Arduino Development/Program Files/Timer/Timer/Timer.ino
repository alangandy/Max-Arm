
// 定时器例程

#define led1_pin  26   // 定义LED1的引脚
#define led2_pin  2    // 定义LED2的引脚
hw_timer_t * timer = NULL;    //声明一个定时器

void IRAM_ATTR onTimer() {    //中断函数
  digitalWrite(led1_pin, !digitalRead(led1_pin));  // 设置LED1状态反转(如:由HIGH变成LOW)
  digitalWrite(led2_pin, !digitalRead(led2_pin));  // 设置LED2状态反转(如:由HIGH变成LOW)
}
 
void setup() {
  Serial.begin(115200);                         // 设置串口波特率
  Serial.println("start...");                   // 串口打印"start..."    
  pinMode(led1_pin, OUTPUT);                    // 设置LED1引脚输出模式
  pinMode(led2_pin, OUTPUT);                    // 设置LED2引脚输出模式
  digitalWrite(led1_pin, LOW);                  // 设置设置LED1引脚低电平
  digitalWrite(led2_pin, LOW);                  // 设置设置LED2引脚低电平
  timer = timerBegin(0, 80, true);              // 初始化,使用第一个定时器,总共4个定时器从0开始计数,预分频器设置80分频         
  timerAttachInterrupt(timer, &onTimer, true);  // 调用中断函数
  timerAlarmWrite(timer, 1000000, true);        // 设置触发时间(单位:微秒,1000000微秒=1秒)
  timerAlarmEnable(timer);                      // 定时器使能
}
 
void loop() {
 
}
