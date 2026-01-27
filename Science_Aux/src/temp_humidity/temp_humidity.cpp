#include "temp_humidity.h"

void TempHumiditySensor::readReg(){
  if(!isReady){
    Serial.println("Error: Temp/humidity sensor has not been properly initialized.");
    return;
  }
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
  uint8_t bytesRead = Wire.requestFrom(addr, 4);
  if (bytesRead != 4) {
    memset(buf, 0, 4);
    Serial.println("Error: Incomplete I2C read from sensor.");
    return;
  }
  for(int i = 0; i < 4; i++){
    buf[i] = Wire.read();
  }
}

void TempHumiditySensor::setup(){ 
  Serial.print("Initializing temp/humidity sensor...");
  Wire.begin();
  delay(100);

  // Creating a test write to ensure that the bus is properly connected
  // 5 seconds before considering failure
  uint8_t error = 0;
  for(int i = 0; i < 5; i++){
    Wire.beginTransmission(addr);
    error = Wire.endTransmission();
    if (error == 0) {
      Serial.println(" Done!");
      isReady = true;
      return;
    }
    Serial.print(".");
    delay(1000);
  }
  Serial.print(" Temp/humidity sensor failed to connect (Error code ");
  Serial.print(error);
  Serial.println(")");
  isReady = false;
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

TempHumiditySensor::TempHumiditySensor(uint8_t reg, uint8_t addr) : reg(reg), addr(addr) {}
