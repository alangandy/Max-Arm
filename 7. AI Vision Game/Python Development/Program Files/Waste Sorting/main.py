import time
from machine import Pin, I2C
from PID import PID
from WonderCam import *
from Buzzer import Buzzer
from espmax import ESPMax
from PWMServo import PWMServo
from BusServo import BusServo
from SuctionNozzle import SuctionNozzle

#小幻熊垃圾分类

# 初始化
pwm = PWMServo()
buzzer = Buzzer()
pwm.work_with_time()
bus_servo = BusServo()
arm = ESPMax(bus_servo)
nozzle = SuctionNozzle()
i2c = I2C(0, scl=Pin(16), sda=Pin(17), freq=400000)
cam = WonderCam(i2c)
cam.set_func(WONDERCAM_FUNC_CLASSIFICATION)  # 设置为颜色识别功能
cam.set_led(True)

if __name__ == '__main__':
  x, y, z = 0, -120, 150
  buzzer.setBuzzer(100) #设置蜂鸣器响100ms
  nozzle.set_angle(0,1000) #吸嘴角度置0
  arm.set_position((x, y, z), 2000)
  time.sleep_ms(2000)
  result_data = []
  result = 0
  
  while True:
    cam.update_result() # 更新小幻熊结果数据
    result_data.append(cam.most_likely_id()) # 获得当前置信度最大的id,并缓存起来
    if len(result_data) == 30: # 多次检测
      result = sum(result_data) / 30.0 # 结果取平均值
      result_data = []
      
      if result != int(result): # 判断结果是不是整数，不是整数说明识别不稳定
        result = 0
        continue # 跳过这一次循环，重新识别
      
      if 2 <= result and result <= 4: # 有害垃圾
        print('id:',int(result),' Hazardous waste')
        angle = 38 # 设置放置补偿角度
        move_time = 1000
        (place_x, place_y, place_z) = (-120,-170,60) # 设置放置坐标位置
        
      elif 5 <= result and result <= 7: # 可回收物
        print('id:',int(result),' Recyclable material')
        angle = 52
        move_time = 1200
        (place_x, place_y, place_z) = (-120,-120,60)
        
      elif 8 <= result and result <= 10: # 厨余垃圾
        print('id:',int(result),' Kitchen garbage')
        angle = 68
        move_time = 1400
        (place_x, place_y, place_z) = (-120,-70,60)
        
      elif 11 <= result and result <= 13: # 其他垃圾
        print('id:',int(result),' Other garbage')
        angle = 90
        move_time = 1600
        (place_x, place_y, place_z) = (-120,-20,60)
        
      else: # 检测到其他id
        continue # 跳过这一次循环，重新识别

      d_y = 65
      buzzer.setBuzzer(100) #蜂鸣器响一下
      arm.set_position((x,y-d_y,100),1000) # 机械臂移动到吸取位置上方，等待2秒后吸取
      time.sleep_ms(1000)
      arm.set_position((x,y-d_y,50),600) # 吸取卡片
      nozzle.on() # 打开气泵
      time.sleep_ms(1000)
      arm.set_position((x,y-d_y,150),800) # 机械臂抬起来
      time.sleep_ms(1000)
      arm.set_position((place_x,place_y,150),move_time) # 移动到放置位置上方
      nozzle.set_angle(angle,move_time) # 设置角度补偿，使卡片放正
      time.sleep_ms(move_time)
      arm.set_position((place_x,place_y,place_z),800) # 放置卡片
      time.sleep_ms(1000)
      nozzle.off() # 关闭气泵
      arm.set_position((place_x,place_y,150),800) # 机械臂抬起来
      time.sleep_ms(1000)
      
      x, y, z = 0, -120, 150
      arm.set_position((x, y, z), move_time) # 机械臂复位，回到初始位置
      nozzle.set_angle(0,move_time) # 吸嘴角度置0
      time.sleep_ms(move_time)
      
    time.sleep_ms(50) # 延时50ms





















