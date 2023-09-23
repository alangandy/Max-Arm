import time
import TM1640
from machine import Pin, I2C
from WonderCam import *
from Buzzer import Buzzer
from espmax import ESPMax
from PWMServo import PWMServo
from BusServo import BusServo
from SuctionNozzle import SuctionNozzle

#小幻熊口罩识别并显示

# 实例化库
pwm = PWMServo()
buzzer = Buzzer()
pwm.work_with_time()
bus_servo = BusServo()
arm = ESPMax(bus_servo)
nozzle = SuctionNozzle()
tm = TM1640.TM1640(clk=Pin(33), dio=Pin(32)) # 实例化点阵库
i2c = I2C(0, scl=Pin(16), sda=Pin(17), freq=400000)
cam = WonderCam(i2c) # 实例化小幻熊库
cam.set_func(WONDERCAM_FUNC_CLASSIFICATION)  # 设置为图像分类功能

if __name__ == '__main__':
  # 初始化 
  arm.go_home()
  buzzer.setBuzzer(100)
  nozzle.set_angle(0,1000)
  time.sleep_ms(2000)

  tm.update_display()
  # 定义点阵显示数据
  cross_buf = [0x0,0x0,0x0,0x81,0xc3,0xe7,0x7e,0x3c,0x3c,0x7e,0xe7,0xc3,0x81,0x0,0x0,0x0]
  smiling_buf = [0x0,0xc,0x2,0x19,0x21,0x42,0x80,0x80,0x80,0x80,0x42,0x21,0x19,0x2,0xc,0x0]
  result_data = []
  
  while True:
    cam.update_result() # 更新检测结果
    result_data.append(cam.most_likely_id()) # 获得当前置信度最大的id,并缓存起来
    if len(result_data) == 5: # 多次检测
      result = sum(result_data) / 5.0 # 结果取平均值
      result_data = []
      if result == 2.0: # id=2，则是戴口罩的
        tm.update_display(smiling_buf) # 点阵显示笑脸
      elif result == 3.0: # id=3，则是不戴口罩的
        tm.update_display(cross_buf) # 点阵显示叉
      else: # 背景
        tm.update_display() # 点阵清屏
        
    time.sleep(0.05) # 延时50ms





















