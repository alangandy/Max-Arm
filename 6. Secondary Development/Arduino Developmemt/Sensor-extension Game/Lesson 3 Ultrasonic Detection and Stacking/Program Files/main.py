import time
from machine import Pin, I2C
from Ultrasonic import ULTRASONIC
from Buzzer import Buzzer
from espmax import ESPMax
from PWMServo import PWMServo
from BusServo import BusServo
from RobotControl import RobotControl
from SuctionNozzle import SuctionNozzle

# 超声波检测码垛

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
  # 机械臂初始设置
  arm.go_home()
  nozzle.set_angle(0,1000)
  time.sleep_ms(2000)
  time_ = time.time()
  overlay = 0
  while True:
    if time.time() - time_ >= 5:
      time_ = time.time()
      hwsr06.setRGBMode(RGB_WORK_BREATHING_MODE)
      hwsr06.setRGBBreathingValue(bytes([20,35,50,  50,20,35,]))
    
    Distance = hwsr06.getDistance()  # 获取超声波检测距离
    print("distance = ", Distance)
    if 70 < Distance < 80:
      buzzer.setBuzzer(100) #设置蜂鸣器响100ms
      time.sleep_ms(1000) #等待1000ma
      arm.set_position((0,-160,85),1500) #吸取色块
      nozzle.on() #打开气泵
      time.sleep_ms(1600)
      arm.set_position((0,-160,200),1000) #抬起来
      time.sleep_ms(1000)
      arm.set_position((160,0,200),1500) #到达放置区上方
      time.sleep_ms(1500)
      arm.set_position((160,0,(88+overlay*40)),1000) #根据已经放置的数量调整高度放置
      time.sleep_ms(1200)
      nozzle.off() #关闭气泵
      arm.set_position((160,0,200),1000) #抬起来
      time.sleep_ms(1000)
      arm.go_home() #机械臂复位，回到初始位置
      time.sleep_ms(2000)
      overlay += 1 # 放置数量加1
      if overlay >= 3: overlay = 0
      
    else:
      time.sleep_ms(500)












