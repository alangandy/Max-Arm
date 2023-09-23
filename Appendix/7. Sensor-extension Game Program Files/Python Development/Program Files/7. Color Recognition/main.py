import time
from machine import Pin, I2C
from Ultrasonic import ULTRASONIC
from Color_sensor import COLOR
from Buzzer import Buzzer
from espmax import ESPMax
from PWMServo import PWMServo
from BusServo import BusServo
from RobotControl import RobotControl
from SuctionNozzle import SuctionNozzle

# 颜色识别

pwm = PWMServo()
buzzer = Buzzer()
bus_servo = BusServo()
arm = ESPMax(bus_servo)
robot = RobotControl()
nozzle = SuctionNozzle()

# 初始化超声波
i2c = I2C(0, scl=Pin(16), sda=Pin(17), freq=100000)
hwsr06 = ULTRASONIC(i2c)
apds = COLOR(i2c)
apds.enableLightSensor(False)


if __name__ == '__main__':
  # 传感器及机械臂初始化
  RED = const(1)
  GREEN = const(2)
  BLUE = const(3)
  WHITE = const(4)
  
  R_F = const(8500)
  G_F = const(13000)
  B_F = const(16600)
  r_f = const(140)
  g_f = const(150)
  b_f = const(140)
  
  RGB_WORK_SIMPLE_MODE    = 0
  RGB_WORK_BREATHING_MODE = 1
  hwsr06.setRGBMode(RGB_WORK_SIMPLE_MODE)
  hwsr06.setRGBValue(bytes([255,255,255, 255,255,255]))
  arm.go_home()
  nozzle.set_angle(0,1000)
  time.sleep_ms(2000)
  print('start...')
  while True:
    # 解析颜色传感器数据
    c = apds.readAmbientLight()
    r = apds.readRedLight()
    g = apds.readGreenLight()
    b = apds.readBlueLight()
    r = int(255 * (r - r_f) / (R_F - r_f))
    g = int(255 * (g - g_f) / (G_F - g_f))
    b = int(255 * (b - b_f) / (B_F - b_f))
    if r > 25 and r > g and r > b: # 输出颜色检测结果，根据检测颜色设置超声波RGB
      color = RED  # 检测到红色
      print('color: red')
      hwsr06.setRGBValue(bytes([255,0,0, 255,0,0]))
    elif g > 25 and g > r and g > b:
      color = GREEN # 检测到绿色
      print('color: green')
      hwsr06.setRGBValue(bytes([0,255,0, 0,255,0]))
    elif b > 25 and b > g and b > r:
      color = BLUE # 检测到蓝色
      print('color: blue')
      hwsr06.setRGBValue(bytes([0,0,255, 0,0,255]))
    else: # 没有检测到颜色
      color = 0
      print('')
      hwsr06.setRGBValue(bytes([255,255,255, 255,255,255]))
    
    time.sleep_ms(500)












