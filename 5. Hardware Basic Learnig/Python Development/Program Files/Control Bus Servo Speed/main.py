import time
from BusServo import BusServo

# 控制总线舵机速度例程

bus_servo = BusServo() 

if __name__ == '__main__':
  bus_servo.run(1, 500, 1500) # 设置1号舵机运行到500脉宽位置，运行时间为1500毫秒
  time.sleep_ms(1500)         # 延时1500毫秒
  
  for t in (500, 2000):      # 分别以不同时间运行一轮，时间越长速度越慢
    bus_servo.run(1, 600, t) # 设置1号舵机运行到700脉宽位置
    time.sleep_ms(t)
    
    bus_servo.run(1, 500, t) # 设置1号舵机运行到500脉宽位置
    time.sleep_ms(t)
    
    bus_servo.run(1, 400, t) # 设置1号舵机运行到300脉宽位置
    time.sleep_ms(t)
    
    bus_servo.run(1, 500, t) # 设置1号舵机运行到500脉宽位置
    time.sleep_ms(t)


