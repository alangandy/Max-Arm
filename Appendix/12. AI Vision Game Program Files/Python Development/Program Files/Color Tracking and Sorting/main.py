import time
from machine import Pin, I2C
from PID import PID
from WonderCam import *
from Buzzer import Buzzer
from espmax import ESPMax
from PWMServo import PWMServo
from BusServo import BusServo
from SuctionNozzle import SuctionNozzle

#小幻熊颜色追踪并分拣

# 初始化
pwm = PWMServo()
buzzer = Buzzer()
pwm.work_with_time()
bus_servo = BusServo()
arm = ESPMax(bus_servo)
nozzle = SuctionNozzle()
i2c = I2C(0, scl=Pin(16), sda=Pin(17), freq=400000)
cam = WonderCam(i2c)
cam.set_func(WONDERCAM_FUNC_COLOR_DETECT)  # 设置为颜色识别功能
cam.set_led(False)

if __name__ == '__main__':
  i = 0
  x, y, z = 0, -120, 150
  buzzer.setBuzzer(100) #设置蜂鸣器响100ms
  nozzle.set_angle(0,1000) #吸嘴角度置0
  arm.set_position((x, y, z), 2000)
  time.sleep_ms(2000)
  x_pid = PID(0.08, 0.003, 0.0003) # 设置PID参数
  y_pid = PID(0.08, 0.003, 0.0003)
  
  while True:
    cam.update_result() # 更新小幻熊结果数据
    if cam.get_color_blob(1): # 判断是否识别id1颜色
      color_num = 1
      color_data = cam.get_color_blob(1) # 获取id1颜色位置数据
    elif cam.get_color_blob(2): # 判断是否识别id2颜色
      color_num = 2
      color_data = cam.get_color_blob(2) # 获取id2颜色位置数据
    elif cam.get_color_blob(3): # 判断是否识别id3颜色
      color_num = 3
      color_data = cam.get_color_blob(3) # 获取id3颜色位置数据
    else:
      color_num = 0
      color_data = None
      
    if color_data:
      center_x = color_data[0] 
      center_y = color_data[1]
      
      if abs(center_x - 160) < 15: # X轴PID算法追踪
        center_x = 160 
      x_pid.SetPoint = 160
      x_pid.update(center_x)
      dx = x_pid.output
      x -= dx
      x = 100 if x > 100 else x # 机械臂X轴范围限幅
      x = -100 if x < -100 else x
      
      if abs(center_y - 120) < 5: # Y轴PID算法追踪
        center_y = 120
      y_pid.SetPoint = 120
      y_pid.update(center_y)
      dy = y_pid.output
      y -= dy
      y = -60 if y > -60 else y # 机械臂Z轴范围限幅
      y = -200 if y < -200 else y 
      
      arm.set_position((x,y,z),50)  # 驱动机械臂
      
      if abs(dx) < 0.1 and abs(dy) < 0.1:
        i += 1
        if i > 10:
          i = 0
          buzzer.setBuzzer(100) #蜂鸣器响一下
          if color_num == 1: # 检测到红色块
            print('color: red')
            angle = 45 # 设置放置补偿角度
            (place_x, place_y, place_z) = (-120,-140,85) # 设置放置坐标位置
          elif color_num == 2: # # 检测到绿色块
            print('color: green')
            angle = 62
            (place_x, place_y, place_z) = (-120,-80,85)
          elif color_num == 3: # 检测到蓝色块
            print('color: blue')
            angle = 90
            (place_x, place_y, place_z) = (-120,-20,85)
          else:
            pass 
          
          d_x = x/2.3
          d_y = (68-abs(d_x/3))
          arm.set_position((x+d_x,y-d_y,100),1000) # 机械臂移动到吸取位置上方，等待2秒后吸取
          time.sleep_ms(1000)
          arm.set_position((x+d_x,y-d_y,86),600) # 吸取木块
          nozzle.on() # 打开气泵
          time.sleep_ms(1000)
          arm.set_position((x+d_x,y-d_y,150),1000) # 机械臂抬起来
          time.sleep_ms(1000)
          arm.set_position((place_x,place_y,150),1500) # 移动到放置位置上方
          nozzle.set_angle(angle,1500) # 设置角度补偿，使木块放正
          time.sleep_ms(1500)
          arm.set_position((place_x,place_y,place_z),1000) # 放置木块
          time.sleep_ms(1200)
          nozzle.off() # 关闭气泵
          arm.set_position((place_x,place_y,150),1000) # 机械臂抬起来
          time.sleep_ms(1000)
          
          x, y, z = 0, -120, 150
          arm.set_position((x, y, z), 2000) # 机械臂复位，回到初始位置
          nozzle.set_angle(0,1800) # 吸嘴角度置0
          time.sleep_ms(2000)
      
    time.sleep_ms(50) # 延时50ms





















