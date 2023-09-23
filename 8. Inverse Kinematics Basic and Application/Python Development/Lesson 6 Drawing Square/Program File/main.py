import time
from espmax import ESPMax
from BusServo import BusServo

# 画正方形

# 实例化库函数
bus_servo = BusServo()
arm = ESPMax(bus_servo)


if __name__ == '__main__':
  # 机械臂复位
  arm.go_home()
  time.sleep(2)

  # 机械臂运行到起始点
  arm.set_position((50,-260,80),2000)
  time.sleep(3)

  # 画上横边
  for i in range(50,-52,-5):
    arm.set_position((i,-260,80),30)
    time.sleep_ms(30)
  time.sleep(0.5)

  # 画右竖边
  for i in range(-260,-158,5):
    arm.set_position((-50,i,80-(26+i/10)),30)
    time.sleep_ms(30)
  time.sleep(0.5)

  # 画下横边
  for i in range(-50,52,5):
    arm.set_position((i,-160,70),30)
    time.sleep_ms(30)
  time.sleep(0.5)
   
  # 画左竖边
  for i in range(-160,-262,-5):
    arm.set_position((50,i,80-(26+i/10)),30)
    time.sleep_ms(30)
  time.sleep(0.5)

  # 机械臂复位
  arm.go_home()










