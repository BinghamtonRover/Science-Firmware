#include "TempSensor.h"
#include "DallasTemperature.h"
#include "OneWire.h"

// Setup a oneWire instance to communicate with any OneWire devices
OneWire oneWire(ONE_WIRE_BUS);

// Pass our oneWire reference to Dallas Temperature sensor 
DallasTemperature sensors(&oneWire);

TempSensor :: TempSensor(int _index)
{
	index = _index;
}

TempSensor :: requestTempeatures()
{
}

TempSensor :: begin()
{
}

TempSensor :: getTempCByIndex(uint8_t deviceIndex)
{
	return sensors.getTempCByIndex(index)
}


