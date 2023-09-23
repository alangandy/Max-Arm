import time
from Buzzer import Buzzer
from espmax import ESPMax
from PWMServo import PWMServo
from BusServo import BusServo
from Infrared_sensor import INFRARED
from RobotControl import RobotControl
from SuctionNozzle import SuctionNozzle

#红外检测控制

pwm = PWMServo()
buzzer = Buzzer()
infrared = INFRARED()
bus_servo = BusServo()
arm = ESPMax(bus_servo)
robot = RobotControl()
nozzle = SuctionNozzle()

infrared.set_long_close_time(1000)

if __name__ == '__main__':
  arm.go_home()
  nozzle.set_angle(0,1000)
  time.sleep_ms(2000)
  time_ = time.time()
  while True:
    infrared.run_loop()
    
    if infrared.close_long():
      buzzer.setBuzzer(100) #设置蜂鸣器响100ms
      arm.set_position((0,-160,100),1500)
      time.sleep_ms(1000) #等待1000ma
      arm.set_position((0,-160,85),800) #吸取色块
      nozzle.on()  #打开气泵
      time.sleep_ms(1000)
      arm.set_position((0,-160,200),1000) #抬起来
      time.sleep_ms(1000)
      arm.set_position((70,-150,200),800) #到达放置区上方
      nozzle.set_angle(30,600)
      time.sleep_ms(1000)
      nozzle.set_angle(35,300)
      arm.set_position((70,-150,90),800) #到放置区
      time.sleep_ms(800)
      arm.set_position((130,-150,88),500) #向左移动一下进行放置
      time.sleep_ms(500)
      nozzle.off()  #关闭气泵
      arm.set_position((130,-150,200),1000) #抬起来
      time.sleep_ms(1000)
      arm.go_home() #机械臂复位，回到初始位置
      nozzle.set_angle(0,2000)
      time.sleep_ms(2000)
      
    else:
      time.sleep_ms(300) #延时












