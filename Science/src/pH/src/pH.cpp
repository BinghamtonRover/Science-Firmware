#include "pH.h"


pHSensor::pHSensor(int PIN){
  pHPIN = PIN;
}

void pHSensor::sample_pH() {
  static unsigned long samplingTime = millis();
  static unsigned long printTime = millis();
  static float pHValue,voltage;
  if(millis()-samplingTime > SAMPLING_INTERVAL)
  {
      pHArray[pHArrayIndex++]=analogRead(pHPIN);
      if(pHArrayIndex==ARRAY_LENGTH)pHArrayIndex=0;
      voltage = avergearray(pHArray, ARRAY_LENGTH)*5.0/1024;
      pHValue = 3.5*voltage+OFFSET;
      samplingTime=millis();
  }
  if(millis() - printTime > PRINT_INTERVAL)   //Every 800 milliseconds, print a numerical, convert the state of the LED indicator
  {
    Serial.print("Voltage:");
        Serial.print(voltage,2);
        Serial.print("    pH value: ");
    Serial.println(pHValue,2);
        printTime=millis();
  }
}

double pHSensor::avergearray(int* arr, int number){
  int i;
  int max,min;
  double avg;
  long amount=0;
  if(number<=0){
    Serial.println("Error number for the array to avraging!/n");
    return 0;
  }
  if(number<5){   //less than 5, calculated directly statistics
    for(i=0;i<number;i++){
      amount+=arr[i];
    }
    avg = amount/number;
    return avg;
  }
  else{
    if(arr[0]<arr[1]){
      min = arr[0];max=arr[1];
    }
    else{
      min=arr[1];max=arr[0];
    }
    for(i=2;i<number;i++){
      if(arr[i]<min){
        amount+=min;        //arr<min
        min=arr[i];
      }
      else {
        if(arr[i]>max){
          amount+=max;    //arr>max
          max=arr[i];
        }
        else{
          amount+=arr[i]; //min<=arr<=max
        }
      }//if
    }//for
    avg = (double)amount/(number-2);
  }//if
  return avg;
}
