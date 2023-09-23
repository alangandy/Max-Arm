import time
from espmax import ESPMax
from BusServo import BusServo

# 画十字

# 实例化库函数
bus_servo = BusServo()
arm = ESPMax(bus_servo)

if __name__ == '__main__':
  # 机械臂到初始位置
  arm.go_home()
  time.sleep(2)
  # 画竖边
  arm.set_position((0,-120,80),1500)
  time.sleep(1.6)

  arm.set_position((0,-280,75),1000)
  time.sleep(1.2)

  arm.set_position((0,-280,150),500)
  time.sleep(0.8)
  # 切换到横边的左端上方
  arm.set_position((100,-200,150),1000)
  time.sleep(1.2)
  # 切换到横边的左端
  arm.set_position((100,-200,80),500)
  time.sleep(0.6)
  # 画横边
  for i in range(100,-101,-2):
    arm.set_position((i,-200,80),5)
    time.sleep_ms(5)

  time.sleep(0.5)  
  arm.set_position((-100,-200,80),1000)
  time.sleep(1)
    
  arm.go_home() # 机械臂回到初始位置









