
// 串口通信例程

HardwareSerial uart(2);  // 定义串口2

void setup() {
  // put your setup code here, to run once:
  uart.begin(115200,SERIAL_8N1,33,32); // 初始化串口,波特率115200; rx:33; tx:32
}

void loop() {
  // put your main code here, to run repeatedly:
  uart.println("Hiwonder");   // 串口打印“Hiwonder”
  int len = uart.available(); // 返回接收缓存可读取字节数
  if(len){                    // 判断是否有数据发送过了
    byte buf[len];            // 定义缓存变量
    for(int i=0; i<len; i++){ 
      buf[i] = uart.read();   // 按字节读取发过来的数据
    }
    uart.write(buf, len);     // 按字节发送数据
  }
  delay(1000);   // 延时1000ms
}
