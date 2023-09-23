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
  Serial.begin(115200);        // 设置串口波特率
  Serial.println("start...");  // 串口打印"start..."
  BusServo.OnInit();           // 初始化总线舵机库
  HardwareSerial.begin(115200, SERIAL_8N1, SERVO_SERIAL_RX, SERVO_SERIAL_TX);
  delay(500);                  // 延时500毫秒
}

bool start_en = true;
void loop() {
  if (start_en) {
    Serial.print("Position: ");
    Serial.println(BusServo.LobotSerialServoReadPosition(1)); // 获取1号舵机位置并通过串口打印
    delay(200); // 延时
    Serial.print("Vin: ");
    Serial.print(BusServo.LobotSerialServoReadVin(1)/1000.0); // 获取1号舵机电压并通过串口打印
    Serial.println(" V");
    start_en = false;
  }
  else {
    delay(500); // 延时500毫秒
  }
}
