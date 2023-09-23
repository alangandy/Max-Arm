#ifndef WONDER_CAM_H_
#define WONDER_CAM_H_

#include <Arduino.h>
#include <Wire.h>


#define CAM_DEFAULT_I2C_ADDRESS       (0x32)
#pragma pack(1)
struct WonderCamQrCodeResultSumm {
  uint8_t current;
  uint8_t id;
  uint8_t __1[14];
  int16_t points[4][2];
  uint16_t len;
  uint8_t __2[14];
};

struct WonderCamFaceDetectResult {
  int16_t x;
  int16_t y;
  uint16_t w;
  uint16_t h;
} __attribute__((aligned(16)));

struct WonderCamObjDetectResult {
  int16_t x;
  int16_t y;
  uint16_t w;
  uint16_t h;
} __attribute__((aligned(16)));

struct WonderCamColorDetectResult {
  int16_t x;
  int16_t y;
  uint16_t w;
  uint16_t h;
} __attribute__((aligned(16)));

struct WonderCamLineResult {
  int16_t start_x;
  int16_t start_y;
  int16_t end_x;
  int16_t end_y;
  int16_t angle;
  int16_t offset;
} __attribute__((aligned(16)));

struct WonderCamAprilTagResult {
  int16_t x;
  int16_t y;
  uint16_t w;
  uint16_t h;
  float x_t;
  float x_r;
  float y_t;
  float y_r;
  float z_t;
  float z_r;
} __attribute__((aligned(32)));

#pragma pack()



#define WONDERCAM_LED_ON           (true)
#define WONDERCAM_LED_OFF          (false)


typedef enum {
  APPLICATION_NONE = 0,
  APPLICATION_FACEDETECT,
  APPLICATION_OBJDETECT,
  APPLICATION_CLASSIFICATION,
  APPLICATION_FEATURELEARNING,
  APPLICATION_COLORDETECT,
  APPLICATION_LINEFOLLOW,
  APPLICATION_APRILTAG,
  APPLICATION_QRCODE,
  APPLICATION_BARCODE,
  APPLICATION_MAX,
} APPLICATION;

typedef enum {
  Aeroplane = 1,
  Bicycle,
  Bird,
  Boat,
  Bottle,
  Bus,
  Car,
  Cat,
  Chair,
  Cow,
  Diningtable,
  Dog,
  Horse,
  Motorbike,
  Person,
  Pottedplant,
  Sheep,
  Sofa,
  Train,
  Monitor
}Objects;

class WonderCam {
  public:
    WonderCam(): wire(Wire) {};
    void begin(void);
    bool firmwareVersion(char* str);
    bool hardwareVersion(char* str);
    bool protocalVersion(char* str);
    int currentFunc(void);
    bool changeFunc(uint8_t new_func);
    void setLed(bool new_state);
    bool updateResult(void);
    int readFromAddr(uint16_t addr,  uint8_t *buf, uint16_t leng);
    int writeToAddr(uint16_t addr, const uint8_t *buf, uint16_t leng);
    //
    // face detect
    bool anyFaceDetected();
    int numOfTotalFaceDetected();
    bool anyLearnedFaceDetected();
    int numOfTotalLearnedFaceDetected();
    bool anyUnlearnedFaceDetected();
    int numOfTotalUnlearnedFaceDetected();
    bool faceOfIdDetected(uint8_t id);
    bool getFaceOfId(uint8_t id, WonderCamFaceDetectResult *p);
    bool getFaceOfIndex(uint8_t index, WonderCamFaceDetectResult *p);
    //
    // obj detect
    bool anyObjDetected();
    int numOfObjDetected();
    bool objIdDetected(uint8_t id);
    int numOfObjIdDetected(uint8_t id);
    bool objDetected(uint8_t id, uint8_t index, WonderCamObjDetectResult *p);
    //
    //classification
    int classIdOfMaxProb();
    float classMaxProb();
    float classProbOfId(uint8_t id);
    //
    //Featuren Learning
    int featureIdOfMaxProb();
    float featureMaxProb();
    float featureProbOfId(uint8_t id);
    //
    //color detect
    bool anyColorDetected(void);
    int numOfColorDetected(void);
    bool colorIdDetected(uint8_t id);
    bool colorId(uint8_t id, WonderCamColorDetectResult *p);
    //
    //line follow
    bool anyLineDetected(void);
    int numOfLineDetected(void);
    bool lineIdDetected(uint8_t id);
    bool lineId(uint8_t id, WonderCamLineResult *p);
    //
    //AprilTag
    bool anyTagDetected(void);
    int numOfTotalTagDetected(void);
    bool tagIdDetected(uint16_t id);
    int numOfTagIdDetected(uint16_t id);
    bool tagId(uint16_t id, int index, WonderCamAprilTagResult *p);

    //
    //qrcode result
    bool qrCodeDetected(void);
    int qrCodeDataLength(void);
    int qrCodeData(uint8_t *buf);
    //
    //bar_code result
    bool barCodeDetected(void);
    int barCodeDataLength(void);
    int barCodeData(uint8_t *buf);
    //
    uint8_t current;
    uint8_t result_summ[128];
  private:
    TwoWire &wire;

};


#endif
