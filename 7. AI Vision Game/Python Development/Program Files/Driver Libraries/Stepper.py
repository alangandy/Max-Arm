

class Stepper:
  EN = 0x04
  SLEEP = 0x03
  RST = 0x01
  DIV_1 = 0
  DIV_1_2 = 1
  DIV_1_4 = 2
  DIV_1_8 = 3
  DIV_1_16 = 7
  
  def __init__(self, i2c, address = 0x35):
    self.bus = i2c
    self.address = address
    self.bus.writeto_mem(self.address, 21, bytes([self.DIV_1_4, ]))

  def set_mode(self, mode):
    self.bus.writeto_mem(self.address, 20, bytes([mode, ]))

  def set_div(self, new_div):
    self.bus.writeto_mem(self.address, 21, bytes([new_div, ]))

  def go_home(self):
    self.bus.writeto_mem(self.address, 22, bytes([1, ]))

  def goto(self, steps):
    a = steps & 0xFF
    b = (steps >> 8) & 0xFF
    c = (steps >> 16) & 0xFF
    d = (steps >> 24) & 0xFF
    self.bus.writeto_mem(self.address, 24, bytes([a, b, c, d, ]))

  def set_speed(self, speed):
    a = speed & 0xFF
    b = (speed >> 8) & 0xFF
    self.bus.writeto_mem(self.address, 28, bytes([a, b]))
  
  
