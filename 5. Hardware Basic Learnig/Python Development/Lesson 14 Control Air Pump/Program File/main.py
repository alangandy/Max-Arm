import time
from SuctionNozzle import SuctionNozzle

# 控制气泵

nozzle = SuctionNozzle()

if __name__ == '__main__':
  
  nozzle.on()          # 打开气泵，同时关闭电磁阀
  time.sleep_ms(2000)  # 延时2000毫秒
  nozzle.off()         # 关闭气泵，同时打开电磁阀
  







