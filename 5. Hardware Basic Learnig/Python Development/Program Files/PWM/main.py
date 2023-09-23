from machine import Pin, PWM
import time
# 初始化PWM，Pin()为所要调用的IO口，freq为PWM的频率，freq=200为频率200Hz，
# duty代表PWM占空比范围为0-1023
led =  PWM(Pin(2), freq=200, duty=0)
time.sleep_ms(500)

# 板载LED来演示一下，这里duty代表的是亮度，以下程序会让LED灯逐渐的从暗到亮再从亮到暗，也就是呼吸灯。

for i in range(3): # 循环3次
  for i in range(0,1024,1): 
    led.duty(i)
    time.sleep_ms(2)
    
  for i in range(1023,0,-1):
    led.duty(i)
    time.sleep_ms(2)
  
led.duty(0) # 关闭LED



