import time
from espmax import ESPMax
from BusServo import BusServo

# 运动学例程

bus_servo = BusServo()
arm = ESPMax(bus_servo)

if __name__ == '__main__':
  arm.go_home() # 机械臂回到初始位置
  time.sleep_ms(2000) # 延时2秒
  
  (x,y,z) = arm.ORIGIN  # 读取机械臂初始位置的XYZ位置
  print(x,y,z)          # 串口打印XYZ位置，单位毫米
  
  # 机械臂初始位置已经是处于机械臂可移动空间的边缘了，所以要先下移，否则机械臂是无法在X、Y轴上移动的
  # arm.set_position((x,y,z),t), x: x轴坐标, y: y轴坐标, z: z轴坐标, t: 移动的总时间（时间越长，速度越慢）
  
  arm.set_position((x,y,z-100),2000) # Z轴相对初始位置下移100毫米
  time.sleep_ms(2000)
  arm.set_position((x,y,z-50),1000)  # Z轴相对初始位置下移50毫米
  time.sleep_ms(1000)
  
  arm.set_position((x-50,y,z-50),1000) # X轴相对初始位置左移50毫米
  time.sleep_ms(1000)
  arm.set_position((x+50,y,z-50),2000) # X轴相对初始位置右移50毫米
  time.sleep_ms(2000)
  arm.set_position((x,y,z-50),1000)    # X轴回到初始位置
  time.sleep_ms(1000)
  
  arm.set_position((x,y-50,z-50),1000) # Y轴相对初始位置后移50毫米
  time.sleep_ms(1000)
  arm.set_position((x,y+50,z-50),2000) # Y轴相对初始位置前移50毫米
  time.sleep_ms(2000)
  arm.set_position((x,y,z-50),1000)    # Y轴回到初始位置
  time.sleep_ms(1000)

  

  






