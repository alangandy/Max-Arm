import time
from BusServo import BusServo

# 读取总线舵机状态

bus_servo = BusServo() 

if __name__ == '__main__':
  
  print('Position:', bus_servo.get_position(1)) # 获取1号舵机位置
  
  print('Vin:', bus_servo.get_vin(1)/1000)      # 获取1号舵机电压
  
  print('Offset:', bus_servo.get_offset(1))     # 获取1号舵机偏差值



