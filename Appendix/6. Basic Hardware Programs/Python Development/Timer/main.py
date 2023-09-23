from machine import Pin,Timer
import time
# 为了直观的表现效果使用了两个LED，拓展板上的LED设置为led1，ESP32上的LED设置为led2
led1 = Pin(26,Pin.OUT)
led2 = Pin(2,Pin.OUT)
# 设置两个LED的初始状态
led1.value(1)
led2.value(1)
time.sleep_ms(100)
# 定义回调函数，使led1、led2的输出取反
def fun(tim):
  led1.value(not led1.value())
  led2.value(not led2.value())
# 开启RTOS定时器，编号为2
tim = Timer(2)
# period为周期，period=1000代表1000ms执行一次，mode代表执行模式，
# Timer.PERIODIC代表重复执行，callback为回调函数，callback=fun代表周期结束后调用fun这个函数
tim.init(period=1000, mode=Timer.PERIODIC,callback=fun) 
# 死循环中只有休眠
while True:
  time.sleep_ms(10)



