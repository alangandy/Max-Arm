import time
from machine import Pin, PWM

class FAN:
  
  def __init__(self, io1=22, io2=23, hz=1000):
    self.fan1 = PWM(Pin(io1))
    self.fan2 = PWM(Pin(io2))
    self.fan1.freq(hz)
    self.fan2.freq(hz)
    self.hz = hz
  def on(self):
    self.fan1.duty(self.hz)
    self.fan2.duty(0)
    
  def off(self):
    self.fan1.duty(0)
    self.fan2.duty(0)
    
  def pwm(self,hz = 500):
    self.fan1.duty(hz)
    self.fan2.duty(0)



