import time
from machine import Pin,ADC
from Buzzer import Buzzer
from espmax import ESPMax
from PWMServo import PWMServo
from BusServo import BusServo
from RobotControl import RobotControl
from SuctionNozzle import SuctionNozzle

# 声音感应摆放

pwm = PWMServo()
buzzer = Buzzer()
bus_servo = BusServo()
arm = ESPMax(bus_servo)
robot = RobotControl()
nozzle = SuctionNozzle()

# 初始化声音传感器
sound_sendor = ADC(Pin(32)) 
sound_sendor.atten(ADC.ATTN_11DB)
sound_sendor.width(ADC.WIDTH_10BIT)

if __name__ == '__main__':
  arm.go_home() #机械臂复位，回到初始位置
  nozzle.set_angle(0,1000) #吸嘴角度置0
  time.sleep_ms(2000)
  angle = [12, 35, 55] #角度补偿
  time_ms = time.ticks_ms()
  num_st = False
  num = 0
  
  while True:
    sound = sound_sendor.read() #声音传感器检测函数
    
    if sound > 50: 
      if num == 0 or (time.ticks_ms()-time_ms) < 1000:
        time_ms = time.ticks_ms()
        time.sleep_ms(80)
        num += 1
  
    if num and (time.ticks_ms()-time_ms) > 1500:
      num = 3 if num > 3 else num
      num_st = True
      print(num)

    if num_st: 
      buzzer.setBuzzer(100) # 蜂鸣器响一下
      arm.set_position((0,-160,100),1200) # 机械臂移动到吸取位置上方，等待2秒后吸取
      time.sleep_ms(2000)
      arm.set_position((0,-160,86),600) # 吸取木块
      nozzle.on() # 打开气泵
      time.sleep_ms(1000)
      arm.set_position((0,-160,180),1000) # 机械臂抬起来
      time.sleep_ms(1000)
      arm.set_position((120,-20-60*(num-1),180),1500) # 移动到放置位置上方
      nozzle.set_angle(angle[(num-1)],1500) # 设置角度补偿，使木块放正
      time.sleep_ms(1500)
      arm.set_position((120,-20-60*(num-1),88),1000) # 放置木块
      time.sleep_ms(1200)
      nozzle.off() # 关闭气泵
      arm.set_position((120,-20-60*(num-1),200),1000) # 机械臂抬起来
      time.sleep_ms(1000)
      arm.go_home() # 机械臂复位，回到初始位置
      nozzle.set_angle(0,1800) # 吸嘴角度置0
      time.sleep_ms(2000)
      num_st = False
      num = 0
      
    else:
      time.sleep_ms(20)






