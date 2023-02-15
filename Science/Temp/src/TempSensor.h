#ifndef TEMP_H
#define TEMP_H

#include "OneWire.h"
#include "DallasTemperature.h"

#define ONE_WIRE_BUS 13

class TempSensor { 
  public:
	TempSensor(int _index);
	void begin();
	void requestTemperatures();
	float getTempCByIndex(uint8_t deviceIndex);
	
  private:
  int index;
};


#endif
