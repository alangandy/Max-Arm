#include "WonderCam.h"

void WonderCam::begin(void) {
  this->wire = Wire;
  Wire.begin(17,16);
  Wire.setClock(80000);
}

int WonderCam::readFromAddr(uint16_t addr,  uint8_t *buf, uint16_t leng) {
  int len = 0;
  Wire.beginTransmission(CAM_DEFAULT_I2C_ADDRESS);
  Wire.write(byte(addr & 0x00FFu));
  Wire.write(byte((addr >> 8) & 0x00FFu));
  Wire.endTransmission();

  uint16_t ts = leng >> 5;
  uint16_t ls = leng - (ts << 5);
  for (int i = 0; i < ts; ++i) {
    Wire.requestFrom((uint8_t)CAM_DEFAULT_I2C_ADDRESS, (size_t)32, true);
    while (Wire.available()) {
      *buf++ = Wire.read();
      ++len;
    }
  }
  if (ls > 0) {
    Wire.requestFrom((uint8_t)CAM_DEFAULT_I2C_ADDRESS, (size_t)ls, true);
    while (Wire.available()) {
      *buf++ = Wire.read();
      ++len;
    }
  }
  return len;
}

int WonderCam::writeToAddr(uint16_t addr, const uint8_t *buf, uint16_t leng) {
  Wire.beginTransmission(CAM_DEFAULT_I2C_ADDRESS);
  Wire.write(byte(addr & 0x00FFu));
  Wire.write(byte((addr >> 8) & 0x00FFu));
  Wire.write(buf, leng);
  Wire.endTransmission();
  return leng;
}

bool WonderCam::firmwareVersion(char* str) {
  readFromAddr(0x0000, (uint8_t*)str, 16);
  return true;
}

int WonderCam::currentFunc(void) {
  uint8_t buf;
  readFromAddr(0x0035, &buf, 1);
  this->current = buf;
  return (int)buf;
}


bool WonderCam::changeFunc(uint8_t new_func) {
  uint8_t count = 0;
  writeToAddr(0x0035, &new_func, 1);
  delay(50);
  while (true) {
    if (currentFunc() != new_func) {
      delay(50);
    } else {
      return true;
    }
    ++count;
    if (count > 80) {
      return false;
    }
  }
}

void WonderCam::setLed(bool new_state) {
  byte buf[3] = {0x30, 0x00, 0x00};
  byte ns_b = new_state ? 1 : 0;
  writeToAddr(0x0030, &ns_b, 1);
}

/*识别到了任意人脸？*/
bool WonderCam::anyFaceDetected() {
  if (current == APPLICATION_FACEDETECT) {
    return result_summ[1] > 0 ? true : false;
  }
  return false;
}

/*识别到的人脸总数*/
int WonderCam::numOfTotalFaceDetected() {
  if (current == APPLICATION_FACEDETECT) {
    return (int)result_summ[1];
  }
  return 0;
}

bool WonderCam::anyLearnedFaceDetected() {
  if (current == APPLICATION_FACEDETECT) {
    return result_summ[2] > 0 ? true : false;
  }
  return false;
}

int WonderCam::numOfTotalLearnedFaceDetected() {
  if (current == APPLICATION_FACEDETECT) {
    return (int)result_summ[2];
  }
  return 0;
}

bool WonderCam::anyUnlearnedFaceDetected() {
  if (current == APPLICATION_FACEDETECT) {
    return result_summ[3] > 0 ? true : false;
  }
  return false;
}

int WonderCam::numOfTotalUnlearnedFaceDetected() {
  if (current == APPLICATION_FACEDETECT) {
    return (int)result_summ;
  }
  return 0;
}

bool WonderCam::faceOfIdDetected(uint8_t id) {
  if (current == APPLICATION_FACEDETECT) {
    for (int i = 4; i < 4 + 29; ++i) {
      if (result_summ[i] == id) {
        return true;
      }
    }
  }
  return false;
}

/*返回指定ID的人脸 */
bool WonderCam::getFaceOfId(uint8_t id, WonderCamFaceDetectResult *p) {
  memset(p, 0, sizeof(WonderCamFaceDetectResult));
  if (current != APPLICATION_FACEDETECT) {
    return false;
  }
  for (int i = 4; i < 4 + 29; ++i) {
    if (result_summ[i] == id) {
      uint16_t index = i - 4;
      index = 0x30 + index * 16;
      readFromAddr(0x0400 + index, (uint8_t*)p, 16);
      return true;
    }
  }
  return false;
}

/*返回指定序号的没有ID的人脸*/
bool WonderCam::getFaceOfIndex(uint8_t index, WonderCamFaceDetectResult *p) {
  memset(p, 0, sizeof(WonderCamFaceDetectResult));
  if (current != APPLICATION_FACEDETECT) {
    return false;
  }
  for (int i = 4; i < 4 + 29; ++i) {
    if (result_summ[i] == 0xFF) {
      --index;
      if (index == 0) {
        uint16_t index_ = i - 4;
        index_ = 0x30 + index_ * 16;
        readFromAddr(0x0400 + index_, (uint8_t*)p, 16);
        return true;
      }
    }
  }
  return false;
}

/* 识别到任意物品？*/
bool WonderCam::anyObjDetected() {
  if (current != APPLICATION_OBJDETECT) {
    return false;
  }
  return result_summ[1] > 0 ? true : false;
}

int WonderCam::numOfObjDetected() {
  if (current != APPLICATION_OBJDETECT) {
    return 0;
  }
  return (int8_t)(result_summ[1]);
}

bool WonderCam::objIdDetected(uint8_t id) {
  if (current != APPLICATION_OBJDETECT) {
    return false;
  }
  for (int i = 2; i < 2 + 29; ++i) {
    if (result_summ[i] == id) {
      return true;
    }
  }
  return false;
}

int WonderCam::numOfObjIdDetected(uint8_t id) {
  if (current != APPLICATION_OBJDETECT) {
    return 0;
  }
  int count = 0;
  for (int i = 2; i < 2 + 29; ++i) {
    if (result_summ[i] == id) {
      ++count;
    }
  }
  return count;
}

bool WonderCam::objDetected(uint8_t id, uint8_t index, WonderCamObjDetectResult *p) {
  memset(p, 0, sizeof(WonderCamObjDetectResult));
  if (current != APPLICATION_OBJDETECT) {
    return false;
  }
  for(int i = 2; i < 2 + 29; ++i) {
    if(result_summ[i] ==id) {
      --index;
    }
    if(index == 0) {
      int r = readFromAddr(0x0800 + 48 + ((i - 2) * 16), (uint8_t*)p, 16);
      if(r != 16) {
        return false;
      }
      return true;
    }
  }
}
//
int WonderCam::classIdOfMaxProb() {
  if (current != APPLICATION_CLASSIFICATION) {
    return 0;
  }
  return (int8_t)result_summ[1];
}
//
float WonderCam::classMaxProb() {
  uint16_t prob_u16;
  if (current != APPLICATION_CLASSIFICATION) {
    return 0;
  }
  memcpy(&prob_u16, &result_summ[2], 2);
  return ((float)((int)(prob_u16))) / 10000.0;
}
//
float WonderCam::classProbOfId(uint8_t id) {
  uint16_t prob_u16;
  if (current != APPLICATION_CLASSIFICATION) {
    return 0;
  }
  memcpy(&prob_u16, &result_summ[16 + (id - 1) * 4], 2);
  return ((float)((int)(prob_u16))) / 10000.0;
}

int WonderCam::featureIdOfMaxProb() {
  if (current != APPLICATION_FEATURELEARNING) {
    return 0;
  }
  return (int8_t)result_summ[1];
}

float WonderCam::featureMaxProb() {
  uint16_t prob_u16;
  if (current != APPLICATION_FEATURELEARNING) {
    return 0;
  }
  memcpy(&prob_u16, &result_summ[2], 2);
  return ((float)((int)(prob_u16))) / 10000.0;
}

float WonderCam::featureProbOfId(uint8_t id) {
  uint16_t prob_u16;
  if (current != APPLICATION_FEATURELEARNING) {
    return 0;
  }
  memcpy(&prob_u16, &result_summ[16 + (id - 1) * 4], 2);
  return ((float)((int)(prob_u16))) / 10000.0;
}


bool WonderCam::anyTagDetected(void) {
  if (current != APPLICATION_APRILTAG) {
    return false;
  }
  return result_summ[1] > 0 ? true : false;
}

int WonderCam::numOfTotalTagDetected(void) {
  if (current != APPLICATION_APRILTAG) {
    return 0;
  }
  return result_summ[1];
}

bool WonderCam::tagIdDetected(uint16_t id) {
  if (current != APPLICATION_APRILTAG) {
    return false;
  }
  int num = result_summ[1];
  for (int i = 2; i < 2 + num; ++i) {
    if (result_summ[i] == id) {
      return true;
    }
  }
  return false;
}

int WonderCam::numOfTagIdDetected(uint16_t id) {
  int count = 0;
  if (current != APPLICATION_APRILTAG) {
    return 0;
  }
  int num = result_summ[1];
  for (int i = 2; i < 2 +  num; ++i) {
    if (result_summ[i] == id) {
      ++count;
    }
  }
  return count;
}

bool WonderCam::tagId(uint16_t id, int index, WonderCamAprilTagResult *p) {
  memset(p, 0, 32);
  if (current != APPLICATION_APRILTAG) {
    return false;
  }
  int num = result_summ[1];
  for (int i = 2; i < 2 + num; ++i) {
    if (result_summ[i] == id) {
      --index;
      if (index == 0) {
        readFromAddr(0x1E00 + 0x30 + 0x32 * (i - 2), (uint8_t*)p, 32);
        return true;
      }
    }
  }
  return false;
}



//是否扫描到QRCode
bool WonderCam::qrCodeDetected(void) {

  if (current != APPLICATION_QRCODE) {
    return false;
  }
  return result_summ[1] > 0 ? true : false;
}

//QRCode的数据长度
int WonderCam::qrCodeDataLength(void) {
  WonderCamQrCodeResultSumm *p = (WonderCamQrCodeResultSumm *)result_summ;
  if (current != APPLICATION_QRCODE) {
    return 0;
  }
  return (int)p->len;
}

//读取Qrcode的数据
int WonderCam::qrCodeData(uint8_t *buf) {
  int len, ret;
  WonderCamQrCodeResultSumm *p = (WonderCamQrCodeResultSumm*)result_summ;
  len = p->len;
  buf[0] = '\0';
  if (current != APPLICATION_QRCODE) {
    return 0;
  }
  if (!qrCodeDetected()) {
    return 0;
  }
  ret = readFromAddr(0x1800 + 48, buf, len);
  return ret;
}

//是否扫描到barCode
bool WonderCam::barCodeDetected(void) {

  if (current != APPLICATION_BARCODE) {
    return false;
  }
  return result_summ[1] > 0 ? true : false;
}

//BarCode的数据长度
int WonderCam::barCodeDataLength(void) {
  WonderCamQrCodeResultSumm *p = (WonderCamQrCodeResultSumm*)result_summ;
  if (current != APPLICATION_BARCODE) {
    return 0;
  }
  return (int)p->len;
}

//读取Barcode的数据
int WonderCam::barCodeData(uint8_t *buf) {
  int len, ret;
  WonderCamQrCodeResultSumm *p = (WonderCamQrCodeResultSumm*)result_summ;
  len = p->len;
  buf[0] = '\0';
  if (current != APPLICATION_BARCODE) {
    return 0;
  }
  if (!barCodeDetected()) {
    return 0;
  }
  ret = readFromAddr(0x1C00 + 48, buf, len);
  return ret;
}

/* 是否识别到了颜色 */
bool WonderCam::anyColorDetected(void) {
  if (current != APPLICATION_COLORDETECT) {
    return false;
  }
  return result_summ[1] > 0 ? true : false;
}

/* 识别到的颜色种数 */
int WonderCam::numOfColorDetected(void) {
  if (current != APPLICATION_COLORDETECT) {
    return 0;
  }
  return result_summ[1];
}

/* 是否识别到了指定颜色*/
bool WonderCam::colorIdDetected(uint8_t id) {
  if (current != APPLICATION_COLORDETECT) {
    return false;
  }
  int num = result_summ[1];
  for (int i = 2; i < 2 + num; ++i) {
    if (result_summ[i] == id) {
      return true;
    }
  }
  return false;
}

/* 获取指定的识别到的颜色的位置数据 */
bool WonderCam::colorId(uint8_t id, WonderCamColorDetectResult *p) {
  memset(p, 0, sizeof(WonderCamColorDetectResult));
  if (current != APPLICATION_COLORDETECT) {
    return false;
  }
  if (!anyColorDetected()) {
    return false;
  }
  int num = result_summ[1];
  for (int i = 2; i < 2 + num; ++i) {
    if (result_summ[i] == id) {
      int r = readFromAddr(0x1000 + 48 + (16 * (i - 2)), (uint8_t*)p, 16);
      return r == 16 ? true : false;
    }
  }
}

/* 是否识别到了线 */
bool WonderCam::anyLineDetected(void) {
  if (current != APPLICATION_LINEFOLLOW) {
    return false;
  }
  return result_summ[1] > 0 ? true : false;
}

/* 识别到的线种数 */
int WonderCam::numOfLineDetected(void) {
  if (current != APPLICATION_LINEFOLLOW) {
    return 0;
  }
  return result_summ[1];
}

/* 是否识别到了指定线*/
bool WonderCam::lineIdDetected(uint8_t id) {
  if (current != APPLICATION_LINEFOLLOW) {
    return false;
  }
  int num = result_summ[1];
  for (int i = 2; i < 2 + num; ++i) {
    if (result_summ[i] == id) {
      return true;
    }
  }
  return false;
}

/* 获取指定的识别到的线位置数据 */
bool WonderCam::lineId(uint8_t id, WonderCamLineResult *p) {
  memset(p, 0, sizeof(WonderCamLineResult));
  if (current != APPLICATION_LINEFOLLOW) {
    return false;
  }
  if (!anyLineDetected()) {
    return false;
  }
  int num = result_summ[1];
  for (int i = 2; i < 2 + num; ++i) {
    if (result_summ[i] == id) {
      int r = readFromAddr(0x1400 + 48 + (16 * (i - 2)), (uint8_t*)p, 16);
      if (r != 16) {
        return false;
      }
      p->angle = p->angle > 90 ? p->angle - 180 : p->angle;
      p->offset = abs(p->offset) - 160;
      return true;
    }
  }
}

//更新结果
bool WonderCam::updateResult(void) {
  readFromAddr(0x0035, &current, 1);
  switch (current) {
    case APPLICATION_FACEDETECT: {
        readFromAddr(0x0400, result_summ, 48);
        break;
      };
    case APPLICATION_OBJDETECT: {
        readFromAddr(0x0800, result_summ, 48);
        break;
      }
    case APPLICATION_CLASSIFICATION: {
        readFromAddr(0x0C00, result_summ, 128);
        break;
      }
    case APPLICATION_FEATURELEARNING: {
        readFromAddr(0x0E00, result_summ, 64);
        break;
      }
    case APPLICATION_COLORDETECT: {
        readFromAddr(0x1000, result_summ, 48);
        break;
      }
    case APPLICATION_LINEFOLLOW: {
        readFromAddr(0x1400, result_summ, 48);
        break;
      }
    case APPLICATION_APRILTAG: {
        readFromAddr(0x1E00, result_summ, 48);
        break;
      }
    case APPLICATION_QRCODE: {
        readFromAddr(0x1800, result_summ, 48);
        break;
      }
    case APPLICATION_BARCODE: {
        readFromAddr(0x1C00, result_summ, 48);
      }
    default: {
        break;
      }
  }
  return true;
}
