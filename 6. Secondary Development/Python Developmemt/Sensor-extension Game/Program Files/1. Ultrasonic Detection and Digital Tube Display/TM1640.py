
from micropython import const
from machine import Pin
from time import sleep_us

TM1640_CMD1 = const(64)  # 0x40 data command
TM1640_CMD2 = const(192) # 0xC0 address command
TM1640_CMD3 = const(128) # 0x80 display control command
TM1640_DSP_ON = const(8) # 0x08 display on
TM1640_DELAY = const(10) # 10us delay between clk/dio pulses

# 字模数据
tube_font = {'0':0x3f,'1':0x06,'2':0x5b,'3':0x4f,'4':0x66,'5':0x6d,'6':0x7d,'7':0x07,'8':0x7f,'9':0x6f, '.': 0x40}

class TM1640(object):
    # Library for LED matrix display modules based on the TM1640 LED driver.
    def __init__(self, clk, dio, brightness=7):
        self.clk = clk
        self.dio = dio
        self.display_buf = [0] * 16

        if not 0 <= brightness <= 7:
            raise ValueError("Brightness out of range")
        self._brightness = brightness

        self.clk.init(Pin.OUT, value=0)
        self.dio.init(Pin.OUT, value=0)
        sleep_us(TM1640_DELAY)

        self._write_data_cmd()
        self._write_dsp_ctrl()

    def _start(self):
        self.dio(0)
        sleep_us(TM1640_DELAY)
        self.clk(0)
        sleep_us(TM1640_DELAY)

    def _stop(self):
        self.dio(0)
        sleep_us(TM1640_DELAY)
        self.clk(1)
        sleep_us(TM1640_DELAY)
        self.dio(1)

    def _write_data_cmd(self):
        # automatic address increment, normal mode
        self._start()
        self._write_byte(TM1640_CMD1)
        self._stop()

    def _write_dsp_ctrl(self):
        # display on, set brightness
        self._start()
        self._write_byte(TM1640_CMD3 | TM1640_DSP_ON | self._brightness)
        self._stop()

    def _write_byte(self, b):
        for i in range(8):
            self.dio((b >> i) & 1)
            sleep_us(TM1640_DELAY)
            self.clk(1)
            sleep_us(TM1640_DELAY)
            self.clk(0)
            sleep_us(TM1640_DELAY)

    def brightness(self, val=None):
        # Set the display brightness 0-7.
        # brightness 0 = 1/16th pulse width
        # brightness 7 = 14/16th pulse width
        if val is None:
            return self._brightness
        if not 0 <= val <= 7:
            raise ValueError("Brightness out of range")

        self._brightness = val
        self._write_data_cmd()
        self._write_dsp_ctrl()

    def write(self, rows, pos=0):
        if not 0 <= pos <= 7:
            raise ValueError("Position out of range")

        self._write_data_cmd()
        self._start()

        self._write_byte(TM1640_CMD2 | pos)
        for row in rows:
            self._write_byte(row)

        self._stop()
        self._write_dsp_ctrl()

    def write_int(self, int, pos=0, len=8):
        self.write(int.to_bytes(len, 'big'), pos)

    def write_hmsb(self, buf, pos=0):
        self._write_data_cmd()
        self._start()

        self._write_byte(TM1640_CMD2 | pos)
        for i in range(7-pos, -1, -1):
            self._write_byte(buf[i])

        self._stop()
        self._write_dsp_ctrl()
    
    def set_bit(self, x, y, s):
        self.display_buf[x] = (self.display_buf[x] & (~(0x01 << y))) | (s << y)
    
    #显示整数
    def tube_display_int(self, num):
        s = str(num) #将数转为字符串
        buf = [tube_font[c] for c in s[:4]] #只显示数值的前4位数字
        self.display_buf = buf
        if len(buf) < 4: #如果不够4位，前面填充0, 这样数字就会靠右显示
            buf_zero = [0] * (4 - len(buf))
            buf_zero.extend(buf)
            self.display_buf = buf_zero
        self.update_display()

    #显示小数
    def tube_display_float(self, num):
        s = "{:0.1f}".format(num) #将数字转为字符串保留1位小数
        buf = []
        for c in s:
            if c in tube_font:
                buf.append(tube_font[c])
            else:
                if c == '.': #如果是小数点， 将前面一位数字的显示增加小点
                    buf[-1] = buf[-1] | 0x80
        self.display_buf = buf
        if len(buf) < 4: #如果不够4位，前面填充0, 这样数字就会靠右显示
            buf_zero = [0] * (4 - len(buf))
            buf_zero.extend(buf)
            self.display_buf = buf_zero
        self.update_display()
    
    def update_display(self, buf = 0):
        if not buf:
          buf = self.display_buf
        self.write(buf)
        self.display_buf = [0] * 16





