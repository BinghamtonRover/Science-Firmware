#include "temp_hum_sensor.h"
#define REG 0x00
#define ADDR 0x40

void Sensor::readReg(){
  Wire.beginTransmission(ADDR);
  // Rewrote with overloaded method so that you don't need to use & on a literal
  Wire.write(REG);
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
  Wire.requestFrom(ADDR, 4);
  for(int i = 0; i < 4; i++){
    buf[i] = Wire.read();
  }
  return;
}

void Sensor::setup(){
  Wire.begin();
}

float Sensor::getTemp(){
  readReg();
  uint16_t temp = buf[0] << 8 | buf[1];
  return ((float) temp * 165 / 65535.0) - 40.0;
}

float Sensor::getHum(){
  readReg();
  uint16_t hum = buf[2] << 8 | buf[3];
  return ((float) hum / 65535.0) * 100;
}
