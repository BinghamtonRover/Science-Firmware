#include "temp_humidity.h"

void TempHumiditySensor::readReg(){
  Wire.beginTransmission(addr);
  // Rewrote with overloaded method so that you don't need to use & on a literal
  Wire.write(reg);
  uint8_t code = Wire.endTransmission();
  if(code != 0){
    // Wiping buffer if there is an error
    memset(buf, 0, 4);
    Serial.print("Error: Could not read temperature/humidity (Error code ");
    Serial.print(code);
    Serial.println(")");
    return;
  }

  delay(20);
  
  // Requesting 4 bytes to get data from registers 0x00 and 0x01 and putting it in buf[0:4]
  Wire.requestFrom(addr, 4);
  for(int i = 0; i < 4; i++){
    buf[i] = Wire.read();
  }
  return;
}

void TempHumiditySensor::setup(){ 
  // 5 second delay before issuing error
  unsigned long startTime = millis();
  unsigned long timeout = 5000;

  Serial.print("Initializing temp/humidity sensor...");
  while (Wire.begin() != 1) {
    Serial.print(".");
     if (millis() - startTime > timeout) {
        // Timeout reached, so issue an error
        Serial.println("Sensor setup failed.");
        return;
    }
    delay(500);
  }
  Serial.println(" Done!");
}

float TempHumiditySensor::getTemperature(){
  readReg();
  uint16_t temp = buf[0] << 8 | buf[1];
  return ((float) temp * 165 / 65535.0) - 40.0;
}

float TempHumiditySensor::getHumidity(){
  readReg();
  uint16_t hum = buf[2] << 8 | buf[3];
  return ((float) hum / 65535.0) * 100;
}

TempHumiditySensor(uint8_t reg, uint8_t addr) : reg(reg), addr(addr) {}