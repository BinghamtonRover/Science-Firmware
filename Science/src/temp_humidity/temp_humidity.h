#pragma once

#include <Arduino.h>
#include "dfrobot/DFRobot_SHT3x.h"

class TempHumiditySensor {
  private:
    DFRobot_SHT3x sensor;

  public:
    TempHumiditySensor();

    void setup();
    float getTemperature();
    float getHumidity();
};
