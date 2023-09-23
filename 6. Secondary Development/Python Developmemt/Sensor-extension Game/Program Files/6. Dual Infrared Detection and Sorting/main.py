import time
from Buzzer import Buzzer
from espmax import ESPMax
from PWMServo import PWMServo
from BusServo import BusServo
from Infrared_sensor import INFRARED
from RobotControl import RobotControl
from SuctionNozzle import SuctionNozzle

#双红外检测分拣

pwm = PWMServo()
buzzer = Buzzer()
bus_servo = BusServo()
arm = ESPMax(bus_servo)
robot = RobotControl()
nozzle = SuctionNozzle()

infrared_left = INFRARED(23)
infrared_right = INFRARED(32)
infrared_left.set_long_close_time(500)
infrared_right.set_long_close_time(500)

if __name__ == '__main__':
  arm.go_home() # 机械臂复位，回到初始位置
  nozzle.set_angle(0,1000) # 吸嘴角度置0
  time.sleep_ms(2000)
  time_ = time.time()
  while True:
    infrared_left.run_loop() # 传感器检测函数
    time.sleep(0.05)
    infrared_right.run_loop()
    
    if infrared_left.close_long(): # 左边传感器检测到木块
      print("infrared_left")
      buzzer.setBuzzer(100) #设置蜂鸣器响100ms
      arm.set_position((70,-165,120),1500)
      time.sleep_ms(1000) #等待1000ma
      arm.set_position((70,-165,86),800) #吸取色块
      nozzle.on()  #打开气泵
      time.sleep_ms(1000)
      arm.set_position((70,-165,200),1000) #抬起来
      time.sleep_ms(1000)
      arm.set_position((150,-35,200),800) #到达放置区上方
      nozzle.set_angle(15,500)
      time.sleep_ms(500)
      arm.set_position((150,-35,90),800) #到放置区
      time.sleep_ms(800)
      arm.set_position((150,10,88),500) #移动一下进行放置
      time.sleep_ms(500)
      nozzle.off()  #关闭气泵
      arm.set_position((150,10,200),1000) #抬起来
      time.sleep_ms(1000)
      arm.go_home() #机械臂复位，回到初始位置
      nozzle.set_angle(0,2000)
      time.sleep_ms(2000)
    
    elif infrared_right.close_long(): # 右边传感器检测到木块
      print("infrared_right")
      buzzer.setBuzzer(100) #设置蜂鸣器响100ms
      arm.set_position((-70,-165,120),1500)
      time.sleep_ms(1000) #等待1000ma
      arm.set_position((-70,-165,86),800) #吸取色块
      nozzle.on()  #打开气泵
      time.sleep_ms(1000)
      arm.set_position((-70,-165,200),1000) #抬起来
      time.sleep_ms(1000)
      arm.set_position((-150,-35,200),800) #到达放置区上方
      nozzle.set_angle(-15,500)
      time.sleep_ms(500)
      arm.set_position((-150,-35,90),800) #到放置区
      time.sleep_ms(800)
      arm.set_position((-150,10,88),500) #移动一下进行放置
      time.sleep_ms(500)
      nozzle.off()  #关闭气泵
      arm.set_position((-150,10,200),1000) #抬起来
      time.sleep_ms(1000)
      arm.go_home() #机械臂复位，回到初始位置
      nozzle.set_angle(0,2000)
      time.sleep_ms(2000)
    
    else:
      time.sleep(0.1) #延时

















