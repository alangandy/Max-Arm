import time
from PWMServo import PWMServo

# 控制单个PWM舵机转动

pwm = PWMServo()
pwm.work_with_time()

if __name__ == '__main__':
  pwm.run(1, 500, 1000) # 设置1号PWM舵机脉宽500，运行时间1000毫秒(PWM舵机无法读取当前位置，所以首次运行，运行时间不可控)
  time.sleep_ms(2000) # 延时2000毫秒
  
  pwm.run(1, 2500, 2000) # 设置1号PWM舵机脉宽2500，运行时间2000毫秒
  time.sleep_ms(2000)
  
  pwm.run(1, 500, 2000) # 设置1号PWM舵机脉宽500，运行时间2000毫秒
  time.sleep_ms(2000)
 





