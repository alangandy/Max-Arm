import time
from PWMServo import PWMServo

# 控制PWM舵机速度

pwm = PWMServo()
pwm.work_with_time()

if __name__ == '__main__':
  pwm.run(1, 500, 1000) # 设置1号PWM舵机脉宽500，运行时间1000毫秒(PWM舵机无法读取当前位置，所以首次运行，运行时间不可控)
  time.sleep_ms(2000) # 延时2000毫秒
  
  for t in (500, 2000): # 分别以不同时间运行一轮，时间越长速度越慢
    pwm.run(1, 500, t) # 设置1号PWM舵机脉宽500
    time.sleep_ms(t)
    
    pwm.run(1, 2500, t) # 设置1号PWM舵机脉宽2500
    time.sleep_ms(t)
    
    pwm.run(1, 500, t) # 设置1号PWM舵机脉宽500
    time.sleep_ms(t)
    






