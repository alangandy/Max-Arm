#include "LobotSerialServoControl.h" // 导入库文件

// 控制总线舵机转动例程

#define SERVO_SERIAL_RX   35
#define SERVO_SERIAL_TX   12
#define receiveEnablePin  13
#define transmitEnablePin 14
HardwareSerial HardwareSerial(2);
LobotSerialServoControl BusServo(HardwareSerial,receiveEnablePin,transmitEnablePin);

void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600); // 设置串口波特率
  Serial.println("start...");  // 串口打印"start..."
  BusServo.OnInit();
  HardwareSerial.begin(115200,SERIAL_8N1,SERVO_SERIAL_RX,SERVO_SERIAL_TX);
  delay(500); // 延时500毫秒
}

bool start_en = true;
void loop() {
  // put your main code here, to run repeatedly:
  if(start_en){
    BusServo.LobotSerialServoMove(1,500,1000); // 设置1号舵机运行到500脉宽位置，运行时间为1000毫秒
    delay(1000); // 延时1000毫秒
  
    BusServo.LobotSerialServoMove(1,700,1000); // 设置1号舵机运行到700脉宽位置，运行时间为1000毫秒
    delay(1000); // 延时1000毫秒
  
    BusServo.LobotSerialServoMove(1,300,2000); // 设置1号舵机运行到300脉宽位置，运行时间为2000毫秒
    delay(2000); // 延时2000毫秒
  
    BusServo.LobotSerialServoMove(1,500,1000); // 设置1号舵机运行到500脉宽位置，运行时间为1000毫秒
    delay(1000); // 延时1000毫秒
    start_en = false;
  }
  else{
    delay(500); // 延时500毫秒
  }
}
