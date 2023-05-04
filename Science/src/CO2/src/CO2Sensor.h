#ifndef CO2Sensor_H
#define CO2Sensor_H

#include <Arduino.h>

#define         CO2_PIN                       17     //define which analog input channel you are going to use
#define         BOOL_PIN                     (2)
#define         DC_GAIN                      (8.5)   //define the DC gain of amplifier

/***********************Software Related Macros************************************/
#define         READ_SAMPLE_INTERVAL         (100)    //define how many samples you are going to take in normal operation
#define         READ_SAMPLE_TIMES            (1)     //define the time interval(in milisecond) between each samples in
                                                     //normal operation

/**********************Application Related Macros**********************************/
//These two values differ from sensor to sensor. user should derermine this value.
#define         ZERO_POINT_VOLTAGE           (0.406) //define the output of the sensor in volts when the concentration of CO2 is 400PPM
#define         REACTION_VOLTAGE             (0.05) //define the voltage drop of the sensor when move the sensor from air into 1000ppm CO2

/*****************************Globals***********************************************/

class CO2Sensor{
  public:
    float MGRead();
    float getPercentage();
    void setup();

  private:
    int percentage;
    float volts;

    /// Two points are taken from the curve to form a line that is approximately equivalent to the 
    /// original curve.
    /// 
    /// Format: { x, y, slope}; point1: (lg400, 0.324), point2: (lg4000, 0.280)
    /// slope = ( reaction voltage ) / (log400 –log1000)
    float CO2Curve[3]  =  {2.602,ZERO_POINT_VOLTAGE,(REACTION_VOLTAGE/(2.602-3))};
};

#endif
