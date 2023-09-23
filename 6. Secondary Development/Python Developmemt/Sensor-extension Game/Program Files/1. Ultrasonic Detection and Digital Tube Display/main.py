import time
import TM1640
from machine import Pin, I2C
from Buzzer import Buzzer
from espmax import ESPMax
from PWMServo import PWMServo
from BusServo import BusServo
from Ultrasonic import ULTRASONIC
from RobotControl import RobotControl
from SuctionNozzle import SuctionNozzle

#超声波检测并数码管显示

pwm = PWMServo()
buzzer = Buzzer()
bus_servo = BusServo()
arm = ESPMax(bus_servo)
robot = RobotControl()
nozzle = SuctionNozzle()
tm = TM1640.TM1640(clk=Pin(33), dio=Pin(32))

# 初始化超声波
i2c = I2C(0, scl=Pin(16), sda=Pin(17), freq=100000)
hwsr06 = ULTRASONIC(i2c)

RGB_WORK_SIMPLE_MODE        = 0
RGB_WORK_BREATHING_MODE     = 1

if __name__ == '__main__':
  arm.go_home() #机械臂复位，回到初始位置
  tm.update_display() # 数码管清屏
  nozzle.set_angle(0,1000) #吸嘴角度置0
  time.sleep_ms(2000)
  hwsr06.setRGBMode(RGB_WORK_SIMPLE_MODE) # 超声波RGB设置RGB模式
  
  while True:
    Distance = hwsr06.getDistance() # 获取超声波检测距离
    print("Distance =", Distance, "mm") # 串口打印距离
    tm.tube_display_int(Distance) # 数码管上显示距离
    if Distance <= 50: # 距离小于等于50mm
      hwsr06.setRGBValue(bytes([0,255,0, 0,255,0])) # 超声波亮绿灯
    elif 50 < Distance and Distance <= 100: # 距离大于50mm,小于等于100mm
      hwsr06.setRGBValue(bytes([255,0,0, 255,0,0])) # 超声波亮红灯
    elif 100 < Distance: # 距离大于100mm
      hwsr06.setRGBValue(bytes([0,0,255, 0,0,255])) # 超声波亮蓝灯
    time.sleep(0.2) # 延时200毫秒
















