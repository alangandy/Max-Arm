import time
import TM1640
from machine import Pin, I2C
from PID import PID
from WonderCam import *
from Buzzer import Buzzer
from espmax import ESPMax
from FanModule import FAN
from PWMServo import PWMServo
from BusServo import BusServo
from SuctionNozzle import SuctionNozzle

#小幻熊人脸追踪风扇

# 初始化
fan = FAN()
pwm = PWMServo()
buzzer = Buzzer()
pwm.work_with_time()
bus_servo = BusServo()
arm = ESPMax(bus_servo)
nozzle = SuctionNozzle()
tm = TM1640.TM1640(clk=Pin(33), dio=Pin(32))
i2c = I2C(0, scl=Pin(16), sda=Pin(17), freq=400000)
cam = WonderCam(i2c)
cam.set_func(WONDERCAM_FUNC_FACE_DETECT)  # 设置为人脸识别功能

if __name__ == '__main__':
  fan.off() # 关闭风扇模块
  fan_st = False # 风扇状态量
  x, y, z = 0, -120, 150
  buzzer.setBuzzer(100) #设置蜂鸣器响100ms
  nozzle.set_angle(0,1000) #吸嘴角度置0
  arm.set_position((x, y, z), 2000)
  time.sleep_ms(2000)
  x_pid = PID(0.026, 0.001, 0.0008) # 设置PID参数
  z_pid = PID(0.030, 0.001, 0.0001)
  tm.update_display() # 点阵清屏
  
  smiling_buf = [0x0,0xc,0x2,0x19,0x21,0x42,0x80,0x80,0x80,0x80,0x42,0x21,0x19,0x2,0xc,0x0]
  
  while True:
    cam.update_result() # 更新小幻熊结果数据
    face_data = cam.get_face(1, 2) # 获取未学习人脸坐标数据
    if face_data: # 识别到人脸
      if not fan_st: # 判断风扇之前是否是关闭状态，避免频繁发送开启指令
        fan.on() # 开启风扇模块
        fan_st = True
        tm.write(smiling_buf) # 点阵显示笑脸
        
      center_x = face_data[0]
      center_y = face_data[1]
      
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
      
      arm.set_position((x,y,z),50) # 驱动机械臂
      
    else: # 未识别到人脸
      if fan_st:
        fan.off() # 关闭风扇模块
        fan_st = False
        tm.update_display() # 点阵清屏
        
    time.sleep_ms(50) # 延时50ms



















