import time
from Buzzer import Buzzer
from espmax import ESPMax
from PWMServo import PWMServo
from BusServo import BusServo
from Touch_sensor import TOUCH
from RobotControl import RobotControl
from SuctionNozzle import SuctionNozzle

# 触控摆放

touch = TOUCH()
pwm = PWMServo()
buzzer = Buzzer()
bus_servo = BusServo()
arm = ESPMax(bus_servo)
robot = RobotControl()
nozzle = SuctionNozzle()

if __name__ == '__main__':
  arm.go_home() #机械臂复位，回到初始位置
  nozzle.set_angle(0,1000) #吸嘴角度置0
  time.sleep_ms(2000)
  num = 0 #木块计数变量
  angle = [-8,-30,-50] #角度补偿
  while True:
    touch.run_loop() #触摸传感器检测函数
    if touch.down_up(): #短按触摸传感器
      print('num:',num+1)
      buzzer.setBuzzer(100) #蜂鸣器响一下
      arm.set_position((0,-160,100),1200) #机械臂移动到吸取位置上方，等待2秒后吸取
      time.sleep_ms(2000)
      arm.set_position((0,-160,85),600) #吸取木块
      nozzle.on() # 打开气泵
      time.sleep_ms(1000)
      arm.set_position((0,-160,180),1000) #机械臂抬起来
      time.sleep_ms(1000)
      arm.set_position((120,-20-60*num,180),1500) # 移动到放置位置上方
      nozzle.set_angle(angle[num],1500) #设置角度补偿，使木块放正
      time.sleep_ms(1500)
      arm.set_position((120,-20-60*num,83+num),1000) #放置木块
      time.sleep_ms(1200)
      nozzle.off() # 关闭气泵
      arm.set_position((120,-20-60*num,200),1000) # 机械臂抬起来
      time.sleep_ms(1000)
      arm.go_home() #机械臂复位，回到初始位置
      nozzle.set_angle(0,1800) #吸嘴角度置0
      time.sleep_ms(2000)
      num += 1      #木块计数变量加1
      if num >= 3: 
        num = 0
        buzzer.setBuzzer(80) #蜂鸣器响一下
        time.sleep_ms(100)
        buzzer.setBuzzer(80) #蜂鸣器响一下
      
    else:
      time.sleep_ms(300)
      
