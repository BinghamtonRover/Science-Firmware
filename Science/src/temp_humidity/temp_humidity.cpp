#include "temp_humidity.h"

TempHumiditySensor::TempHumiditySensor() { 
  Data[100] = {0};
  buff[100] = {0};
  buf[4] = {0};
}

void TempHumiditySensor::setup() {
  Wire.begin();
}

float TempHumiditySensor::getTemperature() {
  readReg(0x00, buf, 4);
  data = buf[0] << 8 | buf[1];
  temp = ((float)data * 165 / 65535.0) - 40.0;  
  return temp;
}

float TempHumiditySensor::getHumidity() {
  readReg(0x00, buf, 4);
  data1 = buf[2] << 8 | buf[3];
  hum =  ((float)data1 / 65535.0) * 100;
  return hum;
}

uint8_t TempHumiditySensor::readReg(uint8_t reg, const void* pBuf, size_t size) {
  uint8_t * _pBuf = (uint8_t *)pBuf;
  Wire.beginTransmission(address);
  Wire.write(&reg, 1);
  if ( Wire.endTransmission() != 0) {
    return 0;
  }
  delay(20);
  Wire.requestFrom(address, (uint8_t) size);
  for (uint16_t i = 0; i < size; i++) {
    _pBuf[i] = Wire.read();
  }
  return size;
}