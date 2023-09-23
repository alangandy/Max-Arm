import time
import TM1640
from machine import Pin, I2C
from PID import PID
from WonderCam import *
from Buzzer import Buzzer
from espmax import ESPMax
from PWMServo import PWMServo
from BusServo import BusServo
from SuctionNozzle import SuctionNozzle

#小幻熊颜色追踪并显示

# 初始化
pwm = PWMServo()
buzzer = Buzzer()
pwm.work_with_time()
bus_servo = BusServo()
arm = ESPMax(bus_servo)
nozzle = SuctionNozzle()
tm = TM1640.TM1640(clk=Pin(33), dio=Pin(32))
i2c = I2C(0, scl=Pin(16), sda=Pin(17), freq=400000)
cam = WonderCam(i2c)
cam.set_func(WONDERCAM_FUNC_COLOR_DETECT)  # 设置为颜色识别功能

if __name__ == '__main__':
  x, y, z = 0, -120, 150
  buzzer.setBuzzer(100) #设置蜂鸣器响100ms
  nozzle.set_angle(0,1000) #吸嘴角度置0
  arm.set_position((x, y, z), 2000)
  time.sleep_ms(2000)
  x_pid = PID(0.026, 0.001, 0.0008) # 设置PID参数
  z_pid = PID(0.030, 0.001, 0.0001)
  tm.update_display() # 点阵清屏
  
  red_buf = [0x0,0x0,0x0,0x0,0x0,0xff,0x19,0x29,0x49,0x86,0x0,0x0,0x0,0x0,0x0,0x0]
  green_buf = [0x0,0x0,0x0,0x0,0x0,0x3c,0x42,0x81,0x81,0xa1,0x62,0x0,0x0,0x0,0x0,0x0]
  blue_buf = [0x0,0x0,0x0,0x0,0x0,0xff,0x89,0x89,0x89,0x76,0x0,0x0,0x0,0x0,0x0,0x0]
  
  while True:
    cam.update_result() # 更新小幻熊结果数据
    if cam.get_color_blob(1): # 判断是否识别id1颜色
      tm.write(red_buf) # 点阵显示‘R’
      color_data = cam.get_color_blob(1) # 获取id1颜色位置数据
    elif cam.get_color_blob(2): # 判断是否识别id2颜色
      tm.write(green_buf) # 点阵显示‘G’
      color_data = cam.get_color_blob(2) # 获取id2颜色位置数据
    elif cam.get_color_blob(3): # 判断是否识别id3颜色
      tm.write(blue_buf) # 点阵显示‘B’
      color_data = cam.get_color_blob(3) # 获取id3颜色位置数据
    else:
      tm.update_display # 点阵清屏
      color_data = None
      
    if color_data:
      center_x = color_data[0] 
      center_y = color_data[1]
      
      if abs(center_x - 160) < 15: # X轴PID算法追踪
        center_x = 160 
      x_pid.SetPoint = 160
      x_pid.update(center_x)
      x -= x_pid.output
      x = 100 if x > 100 else x # 机械臂X轴范围限幅
      x = -100 if x < -100 else x
      
      if abs(center_y - 120) < 5: # Y轴PID算法追踪
        center_y = 120
      z_pid.SetPoint = 120
      z_pid.update(center_y)
      z += z_pid.output
      z = 100 if z < 100 else z # 机械臂Z轴范围限幅
      z = 180 if z > 180 else z
      
      arm.set_position((x,y,z),50)  # 驱动机械臂
      
    time.sleep_ms(50) # 延时50ms


















