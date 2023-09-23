from micropython import const
import time
import struct
import gc

WONDERCAM_FUNC_NONE = const(0x00)
WONDERCAM_FUNC_FACE_DETECT = const(0x01)
WONDERCAM_FUNC_OBJ_DETECT = const(0x02)
WONDERCAM_FUNC_CLASSIFICATION = const(0x03)
WONDERCAM_FUNC_FEATURE_LEARNING = const(0x04)
WONDERCAM_FUNC_COLOR_DETECT = const(0x05)
WONDERCAM_FUNC_LINE_FOLLOWING = const(0x06)
WONDERCAM_FUNC_APRILTAG = const(0x07)
WONDERCAM_FUNC_QRCODE = const(0x08)
WONDERCAM_FUNC_BARCODE = const(0x09)

WONDERCAM_OBJ_AIRPLANE = const(1)
WONDERCAM_OBJ_BICYCLE = const(2)
WONDERCAM_OBJ_BIRD = const(3)
WONDERCAM_OBJ_BOAT = const(4)
WONDERCAM_OBJ_BOTTLE = const(5)
WONDERCAM_OBJ_BUS = const(6)
WONDERCAM_OBJ_CAR = const(7)
WONDERCAM_OBJ_CAT = const(8)
WONDERCAM_OBJ_CHAIR = const(9)
WONDERCAM_OBJ_COW = const(10)
WONDERCAM_OBJ_DINING_TABLE = const(11)
WONDERCAM_OBJ_DOG = const(12)
WONDERCAM_OBJ_HORSE = const(13)
WONDERCAM_OBJ_MOTORBIKE = const(14)
WONDERCAM_OBJ_PERSON = const(15)
WONDERCAM_OBJ_POTTED_PLANT = const(16)
WONDERCAM_OBJ_SHEEP = const(17)
WONDERCAM_OBJ_SOFA = const(18)
WONDERCAM_OBJ_TRAIN = const(19)
WONDERCAM_OBJ_MONITOR = const(20)


def get_addr_bytes(addr):
    lb = addr & 0xFF
    hb = (addr >> 8) & 0xFF
    return bytearray([lb, hb])


class WonderCam:
    def __init__(self, i2c, address=0x32):
        self.address = address  # WonderCam设备地址
        self.bus = i2c  # I2C接口
        self.summary = b''  # 结果概述缓存
        self.result = b''  # 结果内容缓存
        self.curFunc = 0  # 当前缓存结果对应功能

    def read_from_mem(self, addr, length):
        self.bus.writeto(self.address, get_addr_bytes(addr))
        return self.bus.readfrom(self.address, length)

    def firmware_version(self):
        """
            功能:  获取固件版本号
            参数:  无
            返回:  版本号字符串, 如: "v0.6.5"
        """
        # WONDERCAM_REG_SYS_FIRMWARE_VERSION = const(0x0000)
        return self.read_from_mem(0x0000, 16).decode('utf-8').replace('\x00', '')

    def set_led(self, new_st):
        """
            功能:  设置LED开关状态
            参数:  new_st 新的LED状态, False为关, True为开
            返回: None
        """
        # WONDERCAM_REG_SYS_LIGHT_STATE = const(0x0030)
        led = get_addr_bytes(0x0030)
        led += bytes([new_st])
        self.bus.writeto(self.address, led)

    def cur_func(self):
        """
            功能:  获取当前运行中的功能
            参数: fun 要运行的新功能
            返回: None
        """
        # WONDERCAM_REG_SYS_CURRENT_FUNC = const(0x0035)
        return int(self.read_from_mem(0x0035, 1)[0])

    def set_func(self, func, timeout=3000):
        """
            功能: 设置要运行的功能, 此方法会阻塞等待切换成功直至成功或超时
            参数: fun 要运行的新功能,  timeout超时时间单位毫秒
            返回: 成功返回True, 超时返回False
        """
        # WONDERCAM_REG_SYS_CURRENT_FUNC = const(0x0035)
        data = get_addr_bytes(0x0035)
        data += bytes([func])
        self.bus.writeto(self.address, data)
        timeout += time.ticks_ms()
        while True:
            time.sleep_ms(50)
            if self.cur_func() == func:
                return True
            if time.ticks_ms() > timeout:
                return False

    def update_result(self):
        """
            功能: 更新并获取结果到缓存
            参数: 无
            返回: None
        """
        # WONDERCAM_REG_FACE_DETECT_BASE = const(0x0400)
        # WONDERCAM_REG_OBJ_DETECT_BASE = const(0x0800)
        # WONDERCAM_REG_CLASSIFICATION_BASE = const(0x0C00)
        # WONDERCAM_REG_FEATURE_LEARNING_BASE = const(0x0E00)
        # WONDERCAM_REG_COLOR_DETECT_BASE = const(0x1000)
        # WONDERCAM_REG_LINE_FOLLOWING_BASE = const(0x1400)
        # WONDERCAM_REG_APRILTAG_BASE = const(0x1E00)
        # WONDERCAM_REG_QRCODE_BASE = const(0x1800)
        # WONDERCAM_REG_BARCODE_BASE = const(0x1C00)
        cur_func = self.cur_func()
        self.curFunc = cur_func
        tmp = (None, 0x0400, 0x0800, 0x0C00, 0x0E00, 0x1000, 0x1400, 0x1E00, 0x1800, 0x1C00)
        addr = tmp[cur_func]
        if addr is None:
            return
        self.summary = self.read_from_mem(addr, 48)  # 获取结果概述，描述了本次结果的长度等情况
        if self.summary[1] > 0:  # 根据结果个数读取对应长度字节
            if 0 < cur_func < 3 or 5 <= cur_func < 7:
                self.result = self.read_from_mem(addr + 0x30, 16 * self.summary[1])
            elif cur_func == 7:
                self.result = self.read_from_mem(addr + 0x30, 32 * self.summary[1])
            else:  # 二维码等不直接读取，因为可能比较长， 再需要用时再读取
                self.result = b''
        else:
            self.result = b''

    def is_face_detected(self, id_want=None):
        """
            功能: 检查是否在画面中识别到指定ID的人脸
            参数: id_want在1~5间时代表要检查的人脸id, 为None就是为任意人脸, 若为0只指未学习的人脸
            返回: 若识别到则返回 True
                  若未识别到返回 False
        """
        if self.curFunc == WONDERCAM_FUNC_FACE_DETECT and self.summary[1] > 0:  # 当前结果的功能不是人脸识别
            if id_want is None:  # 任意人脸,已学习或未学习的
                return True
            if id_want == 0:
                id_want = 0xFF
            for i in range(4, 4 + self.summary[1]):  # 未学习or已学习人脸
                if self.summary[i] == id_want:
                    return True
        return False

    def get_face(self, id_want, face_type=1):  # 获取人脸， type = 1 为已学习人脸， type = 2 为未学习人脸
        """
          功能: 获取识别到的指定的人脸
          参数: id_want 要获取的人脸ID， 若face_type=2则为要获取的人脸的序号
                face_type 要获取的人脸的类型， face_type = 1为已学习人脸， face_type = 2 为未学习人脸
          返回: 若识别到则返回一个元组人脸的(中心X 中心Y, 宽, 高)
                若未识别到返回 None
        """
        if self.is_face_detected():
            if face_type == 1:  # 获取指定id人脸
                for i in range(4, 4 + self.summary[1]):  # 逐个检查识别到的人脸是否是想要的人脸
                    if self.summary[i] == id_want:
                        index = 16 * (i - 4)
                        return struct.unpack("<hhHH", self.result[index: index + 8])
            elif face_type == 2:  # 获取未学习人脸
                for i in range(4, 4 + self.summary[1]):
                    if self.summary[i] == 0xFF:
                        id_want -= 1  # 要第id_want 个未学习的人脸
                        if id_want == 0:
                            index = 16 * (i - 4)
                            return struct.unpack("<hhHH", self.result[index: index + 8])
            else:
                pass
        return None

    def __is_detected_common(self, func, id_want):
        if self.curFunc == func and self.summary[1] > 0:
            if id_want is None:
                return True
            else:
                for i in range(2, 2 + self.summary[1]):
                    if self.summary[i] == id_want:
                        return True
        return False

    def is_object_detected(self, id_want=None):
        """
            功能: 检查是否在画面中识别到指定ID的人脸
            参数: id_want为None时检查是否识别到任何可以识别的物品, id_want为ID时检查是否识别到特定物品
            返回: 若识别到则返回 True
                 若未识别到返回 False
        """
        return self.__is_detected_common(WONDERCAM_FUNC_OBJ_DETECT, id_want)

    def get_object(self, id_want, index):
        """
            功能: 获取在画面中识别到的物品的数据
            参数: id_want为要获取的物品ID， index为要获取的物品序号(因可能在画面中同时识别到多个同样的物品)
            返回: (X坐标, Y坐标, 宽度, 高度)
        """
        if self.is_object_detected():
            for i in range(2, 2 + self.summary[1]):
                if self.summary[i] == id_want:
                    index -= 1
                    if index == 0:
                        index = 16 * (i - 2)
                        return struct.unpack("<hhHH", self.result[index: index + 8])
        return None

    def most_likely_id(self):
        """
          功能: 图像分类或特征学习中获取可能性最大的ID
          参数: 无
          返回: 一个整数, 可能性最大的ID
        """
        if self.curFunc == WONDERCAM_FUNC_CLASSIFICATION or self.curFunc == WONDERCAM_FUNC_FEATURE_LEARNING:
            return self.summary[1]
        return 0

    def max_conf(self):
        """
          功能: 图像分类或特征学习中获取可能性最大的ID对应的概率
          参数: 无
          返回: 一个小数, 对应的概率
        """
        if self.curFunc == WONDERCAM_FUNC_CLASSIFICATION or self.curFunc == WONDERCAM_FUNC_FEATURE_LEARNING:
            conf = int.from_bytes(self.summary[2:4], "little", False)
            conf = conf / 10000.0
            return conf
        return 0

    def conf_of_id(self, id_want):
        """
          功能: 图像分类或特征学习中获取图像是指定id_want的概率
          参数: 无
          返回: 一个小数, 对应的概率
        """
        if self.curFunc == WONDERCAM_FUNC_CLASSIFICATION or self.curFunc == WONDERCAM_FUNC_FEATURE_LEARNING:
            addr = 0x10 + ((id_want - 1) * 4)
            conf = int.from_bytes(self.summary[addr: addr + 2], "little", False)
            conf = conf / 10000.0
            return conf
        return 0

    def is_color_blob_detected(self, id_want=None):
        """
          功能: 检查是否在画面中识别到指定的颜色ID
          参数: id_want=要获取的颜色ID, id_want为None时检查任意一个ID的颜色， id_want为1~7间时检测指定ID颜色
          返回: 若识别到则返回 True
                若未识别到返回 False
        """
        return self.__is_detected_common(WONDERCAM_FUNC_COLOR_DETECT, id_want)

    def get_color_blob(self, id_want):
        """
          功能: 获取识别到的指定ID的颜色色块位置数据
          参数: id_want=要获取的颜色ID
          返回: 若识别到则返回一个元组色块的(中心X 中心Y, 宽, 高)
                若未识别到返回 None
        """
        if self.is_color_blob_detected(id_want):
            for i in range(2, 2 + self.summary[1]):
                if self.summary[i] == id_want:
                    index = 16 * (i - 2)
                    return struct.unpack("<hhHH", self.result[index: index + 8])
        return None

    def is_line_detected(self, id_want=None):
        """
          功能: 检查是否在画面中识别到指定ID的线条
          参数: id_want=要获取的线条ID, id_want为None时检查任意一个ID的线条， id_want为1~3间时检测指定ID的线条
          返回: 若识别到则返回 True
                若未识别到返回 False
        """
        return self.__is_detected_common(WONDERCAM_FUNC_LINE_FOLLOWING, id_want)

    def get_line(self, id_want):
        """
          功能: 获取识别到的指定ID的线条数据
          参数: id_want=要获取的线条ID
          返回: 若识别到则返回一个元组色块的(箭头尖端X， 简短Y, 尾部X, 尾部Y, 偏移, 角度)
                若未识别到返回 None
        """
        if self.is_line_detected(id_want):
            for i in range(2, 2 + self.summary[1]):
                if self.summary[i] == id_want:
                    index = 16 * (i - 2)
                    x, y, w, h, angle, offset = struct.unpack("<hhHHhh", self.result[index: index + 12])
                    angle = angle - 180 if angle > 90 else angle
                    offset = abs(offset) - 160
                    return x, y, w, h, angle, offset
        return None

    """
    关于apriltag标签识别, WonderCam Standard v1设计只识别TAG36H11族标签,其他族的apriltag会被忽略不识别
    """

    def is_tag_detected(self, id_want=None):
        """
          功能: 检查是否在画面中识别到了标签， 或者指定ID的标签
          参数: id_want 若为None则识别任意ID的标签, 若为指定的ID数值则识别指定ID
          返回:  若识别到则返回 True
                若未识别到返回 False
        """
        return self.__is_detected_common(WONDERCAM_FUNC_APRILTAG, id_want)

    def num_of_tag_detected(self, id_want=None):
        """
          功能: 获取在画面中识别到的标签的个数
          参数: id_want 若为None则获取画面中识别到的全部标签的个数, 若为指定ID则获取画面中识别到的指定ID的标签个数
          返回:  若识别到则返回识别到的个数
                若未识别到返回0
        """
        ret = 0
        if self.curFunc == WONDERCAM_FUNC_APRILTAG:
            if id_want is None:
                return int(self.summary[1])
            else:
                if id_want >= 0:
                    for i in range(2, 2 + self.summary[1]):
                        if self.summary[i] == id_want:
                            ret += 1
        return ret

    def get_tag(self, id_want, index):
        """
          功能: 获取在画面中识别到的第index个id为id_want的标签的位置姿态数据
          参数: id_want为要获取的标签的id, index为画面中识别到的第index个id_want的标签 (一个画面中可以识别到多个同id的标签, 所以要做选择)
          返回: 若获取成功返回标签数据 (中心X, 中心Y, 宽, 高, X轴变换, X轴旋转, Y轴变换, Y轴旋转, Z轴变换, Z轴旋转)
        """
        if self.is_tag_detected():
            for i in range(2, 2 + self.summary[1]):
                if self.summary[i] == id_want:
                    index -= 1
                    if index == 0:
                        index = 32 * (i - 2)
                        return struct.unpack("<hhHHffffff", self.result[index: index + 32])
        return None

    def is_qrcode_detected(self):
        """
          功能: 检查是否在画面中识别到了二维码
          参数: 无
          返回: 若识别则返回 True
                若未识别到返回 False
        """
        if self.curFunc == WONDERCAM_FUNC_QRCODE and self.summary[1] > 0:
            return True
        return False

    def is_barcode_detected(self):
        """
          功能: 检查是否在画面中识别到了条形码
          参数: 无
          返回: 若识别则返回 True
                若未识别到返回 False
        """
        if self.curFunc == WONDERCAM_FUNC_BARCODE and self.summary[1] > 0:
            return True
        return False

    def len_of_code(self):
        """
           功能: 获取扫描到的二维码/条形码的字节数
           参数: 无
           返回: 二维码/条形码的字节数, 若没有识别到二维码/条形码返回0
         """
        if self.is_qrcode_detected() or self.is_barcode_detected():
            return int.from_bytes(self.summary[0x20: 0x22], "little", False)
        return 0

    def update_code_content(self):
        """
           功能: 检查是否需要更新二维码/条形码结果内容,若需要则更新
           参数: 无
           返回: None
        """
        # WONDERCAM_REG_QRCODE_BASE = const(0x1800)
        # WONDERCAM_REG_BARCODE_BASE = const(0x1C00)
        if self.result == b'':
            length = self.len_of_code()
            if length > 0:
                if self.curFunc == WONDERCAM_FUNC_QRCODE:
                    self.result = self.read_from_mem(0x1830, length)
                elif self.curFunc == WONDERCAM_FUNC_BARCODE:
                    self.result = self.read_from_mem(0x1C30, length)
                else:
                    pass

    def string_from_code(self):
        """
          功能: 获取扫描到的二维码/条形码的内容字符串(获取的是条形码还是二维码内容由当前运行的功能决定)
          参数: 无
          返回: 识别到的二维码/条形码的内容字符串, 若没有识别到二维码/条形码或者二维码/条形码内容Decode失败则返回None
        """
        code_str = None
        if self.is_qrcode_detected() or self.is_barcode_detected():
            self.update_code_content()
            try:
                code_str = self.result.decode("utf-8")
            except:
                pass
        return code_str

    def bytes_from_code(self):
        """
          功能: 获取扫描到的二维码/条形码的内容字节串(获取的是条形码还是二维码内容由当前运行的功能决定)
          参数: 无
          返回: 识别到的二维码/条形码内容的字节串, 若没有识别到返回b''
        """
        if self.is_qrcode_detected() or self.is_barcode_detected():
            self.update_code_content()
        return self.result


gc.collect()

if __name__ == "__main__":
    from machine import Pin, I2C
    i2c = I2C(0, scl=Pin(16), sda=Pin(17), freq=400000)
    cam = WonderCam(i2c)
    cam.set_func(WONDERCAM_FUNC_FACE_DETECT)  # 设置功能为人脸识别功能
    while True:
        cam.update_result()
        print(cam.get_face(1, 2))  # 获取获取画面中识别到的第一张未学习人脸(画面中白框框出的人脸), 2代表未学习人脸
        time.sleep(0.2)

