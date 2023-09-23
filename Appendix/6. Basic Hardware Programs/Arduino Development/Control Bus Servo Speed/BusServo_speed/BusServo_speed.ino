#include "LobotSerialServoControl.h" // 导入库文件

// 控制总线舵机速度例程

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
  BusServo.OnInit(); // 初始化总线舵机库
  HardwareSerial.begin(115200,SERIAL_8N1,SERVO_SERIAL_RX,SERVO_SERIAL_TX);
  delay(500); // 延时500毫秒
  BusServo.LobotSerialServoMove(1,500,1500); // 设置1号舵机运行到500脉宽位置，运行时间为1500毫秒
  delay(1500); // 延时1500毫秒
}

bool start_en = true;
void loop() {
  // put your main code here, to run repeatedly:
  if(start_en){
    int t[2] = {500, 2000};
    for(int i=0; i < 2; i++){ // 分别以不同时间运行一轮，时间越长速度越慢
      BusServo.LobotSerialServoMove(1,600,t[i]); // 设置1号舵机运行到600脉宽位置
      delay(t[i]); 
    
      BusServo.LobotSerialServoMove(1,500,t[i]); // 设置1号舵机运行到500脉宽位置
      delay(t[i]);
  
      BusServo.LobotSerialServoMove(1,400,t[i]); // 设置1号舵机运行到400脉宽位置
      delay(t[i]);
    
      BusServo.LobotSerialServoMove(1,500,t[i]); // 设置1号舵机运行到500脉宽位置
      delay(t[i]); 
    }
    start_en = false;
  }
  else{
    delay(500); // 延时500毫秒
  }
}
