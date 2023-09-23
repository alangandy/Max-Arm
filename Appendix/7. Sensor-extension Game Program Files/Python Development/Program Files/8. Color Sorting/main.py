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

# 颜色分拣

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
  color = 0
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
    if r > 25 and r > g and r > b:t = RED  # 输出颜色检测结果
    elif g > 25 and g > r and g > b:t = GREEN
    elif b > 25 and b > g and b > r:t = BLUE
    else:t = 0
    
    if t > 0: 
      buzzer.setBuzzer(100)
      color = t # 得到颜色检测的结果
      print('color:',color)
      if color == 1: # 根据相应颜色设置超声波rgb灯的颜色
        angle = -45
        (x,y,z) = (120,-140,85)
        hwsr06.setRGBValue(bytes([255,0,0, 255,0,0]))
      elif color == 2: 
        angle = -25
        (x,y,z) = (120,-80,85)
        hwsr06.setRGBValue(bytes([0,255,0, 0,255,0]))
      elif color == 3: 
        angle = 0
        (x,y,z) = (120,-20,82)
        hwsr06.setRGBValue(bytes([0,0,255, 0,0,255]))
      
    if color > 0: # 检测到颜色
      Distance = hwsr06.getDistance() # 获取超声波检测距离
      print('distance:', Distance)
      if 70 < Distance < 80: # 色块距离满足吸取的条件，进行分拣
        buzzer.setBuzzer(100)  #设置蜂鸣器响100ms
        time.sleep_ms(1000)
        arm.set_position((0,-160,85),1500) #吸取色块
        nozzle.on() #打开气泵
        time.sleep_ms(1600)
        arm.set_position((0,-160,180),1000) #抬起来
        time.sleep_ms(1000)
        arm.set_position((x,y,180),1000) #到达相应颜色的放置区上方
        time.sleep_ms(1000)
        nozzle.set_angle(angle,800) # 设置角度补偿
        arm.set_position((x,y,z),800) # 放置色块
        time.sleep_ms(1000)
        nozzle.off() #关闭气泵
        arm.set_position((x,y,200),1000) #抬起来
        time.sleep_ms(1000)
        arm.go_home() #机械臂复位，回到初始位置
        nozzle.set_angle(0,800)
        time.sleep_ms(2000)
        (x,y,z) = arm.ORIGIN
        angle = 0
        color = 0
        hwsr06.setRGBValue(bytes([255,255,255, 255,255,255])) # 设置超声波RGB亮白光
        
    time.sleep_ms(500)











