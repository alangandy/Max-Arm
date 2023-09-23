import time
from machine import Pin, I2C
from Ultrasonic import ULTRASONIC
from Buzzer import Buzzer
from espmax import ESPMax
from PWMServo import PWMServo
from BusServo import BusServo
from RobotControl import RobotControl
from SuctionNozzle import SuctionNozzle

#超声波检测吸取

pwm = PWMServo()
buzzer = Buzzer()
bus_servo = BusServo()
arm = ESPMax(bus_servo)
robot = RobotControl()
nozzle = SuctionNozzle()

# 初始化超声波
i2c = I2C(0, scl=Pin(16), sda=Pin(17), freq=100000)
hwsr06 = ULTRASONIC(i2c)

RGB_WORK_SIMPLE_MODE        = 0
RGB_WORK_BREATHING_MODE     = 1

if __name__ == '__main__':
  arm.go_home()
  nozzle.set_angle(0,1000)
  time.sleep_ms(2000)
  time_ = time.time()
  while True:
    if time.time() - time_ >= 5:
      time_ = time.time()
      hwsr06.setRGBMode(RGB_WORK_BREATHING_MODE)
      hwsr06.setRGBBreathingValue(bytes([20,35,50,  50,20,35,]))
    
    Distance = hwsr06.getDistance() # 获取超声波检测距离
    print("distance = ", Distance)
    if 70 < Distance < 80:
      buzzer.setBuzzer(100) #设置蜂鸣器响100ms
      arm.set_position((0,-160,100),1500)
      time.sleep_ms(1000) #等待1000ma
      arm.set_position((0,-160,85),800) #吸取色块
      nozzle.on()  #打开气泵
      time.sleep_ms(1000)
      arm.set_position((0,-160,200),1000) #抬起来
      time.sleep_ms(1000)
      arm.set_position((70,-150,200),800) #到达放置区上方
      nozzle.set_angle(30,600)
      time.sleep_ms(1000)
      nozzle.set_angle(35,300)
      arm.set_position((70,-150,90),800) #到放置区
      time.sleep_ms(800)
      arm.set_position((130,-150,88),500) #向左移动一下进行放置
      time.sleep_ms(500)
      nozzle.off()  #关闭气泵
      arm.set_position((130,-150,200),1000) #抬起来
      time.sleep_ms(1000)
      arm.go_home() #机械臂复位，回到初始位置
      nozzle.set_angle(0,2000)
      time.sleep_ms(2000)
      
    else:
      time.sleep_ms(500) #延时











