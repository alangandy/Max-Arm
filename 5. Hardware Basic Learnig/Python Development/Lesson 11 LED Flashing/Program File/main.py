import time
from machine import Pin

# ESP32上的板载LED灯是连接到IO2上的，所以我们要先设置IO2的引脚为输出模式
led = Pin(2, Pin.OUT)
for i in range(3):  # 循环3次
    # 为了点亮LED要调用on函数使IO2为高电平，这时LED亮起
    led.on()
    time.sleep(1)  # 延时1秒
    led.off()  # 关闭LED
    time.sleep(1)  # 延时1秒


