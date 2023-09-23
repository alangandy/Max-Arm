from machine import Pin,ADC
import time
# 在我们的拓展板上现有的IO口中只有IO33、IO32能够使用ADC这个功能。
adc = ADC(Pin(39))
# 设置衰减比 满量程3.6v
adc.atten(ADC.ATTN_11DB)
# 设置数据宽度为10bit 满量程1023
adc.width(ADC.WIDTH_10BIT)
# 在死循环中读取ADC数据
while True:
  print(adc.read() / 1023 * 3.3 * 4)
  time.sleep_ms(100)
  












