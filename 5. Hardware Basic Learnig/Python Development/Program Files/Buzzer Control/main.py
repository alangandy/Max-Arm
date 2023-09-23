import time
from Buzzer import Buzzer

# 控制蜂鸣器

buzzer = Buzzer()

if __name__ == '__main__':
  buzzer.setBuzzer(100)  # 设置蜂鸣器响100毫秒
  time.sleep_ms(1000)    # 延时1000毫秒
  buzzer.setBuzzer(300)  # 设置蜂鸣器响300毫秒







