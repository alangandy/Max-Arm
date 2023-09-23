import time
from BusServo import BusServo

# 控制总线舵机转动例程

bus_servo = BusServo() 

if __name__ == '__main__':
  bus_servo.run(1, 500, 1000) # 设置1号舵机运行到500脉宽位置，运行时间为1000毫秒
  time.sleep_ms(1000)         # 延时1000毫秒
  
  bus_servo.run(1, 700, 1000) # 设置1号舵机运行到700脉宽位置，运行时间为1000毫秒
  time.sleep_ms(1000)
  
  bus_servo.run(1, 300, 2000) # 设置1号舵机运行到300脉宽位置，运行时间为2000毫秒
  time.sleep_ms(2000)
  
  bus_servo.run(1, 500, 1000) # 设置1号舵机运行到500脉宽位置，运行时间为1000毫秒
  time.sleep_ms(1000)

