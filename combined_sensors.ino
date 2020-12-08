
//Code needs to have all pins mapped to teensy 4.1 pins 
//also needs to be adapted to teensy 4.1 library 


//arduino code for collecting data from science sensors
//Written by Dillon Kane to be used by the Binghamton University Rover Team
//code modified and adapted from existing sensor library examples

//code needs to have sensor data converted to useful information
//CO2 sensor needs 2 hour calibration time... returns value in ppm (400ppm is reference voltage)
//user notes -- should calibrate the co2 sensor using sensor calibration code -- need to first let sensor read value from room then let it read from bag filled with exhaled air


#include <MG811.h>//library for MQ4 sensor
#include <dht.h> //library for DHT11 temp & humidity sensor
dht DHT; //create dht variable to be used when reading temperature & humidity values
#define pH A2            //pH meter Analog output to Arduino Analog Input 0
#define MQ4 A1     //need to convert to Rs/ Ro values as seen in data sheet to plot against ppm
#define DHT11 A3 //DHT only returns integer values      
#define Offset 0.00            //deviation compensate for pH
#define samplingInterval 20    //used in pH
#define printInterval 800       //used ih Ph
#define ArrayLenth  40    //used in pH -- times of collection
int pHArray[ArrayLenth];   //Store the average value of the sensor feedback
int pHArrayIndex=0;

float v400 = 4.535;     // change 4.353 to be value from co2 open air reading (from calibration code)
float v40000 = 3.206;   // change 3.206 to be value from co2 exhaled air reading (from calibration code)
    
MG811 co2Sensor = A0; //define Co2 Sensor pin
int Co2Value, methaneValue; //global variables used to store & write final values of co2 and methane
float pHValue; //global variable used to store & write final value of pH
int temperature, humidity; //global variable to store temp & humidity values

void setup(void)
{
  Serial.begin(9600);
  pinMode(MQ4, INPUT);
  pinMode(pH, INPUT);
  pinMode(DHT11, INPUT);
  co2Sensor.calibrate(); //calibrate Co2 Sensor  (should be done in a separate code)
  co2Sensor.begin(v400, v40000); // set the reference values for co2 sensor in the library  (this stays here (only remove above line)
  Serial.print("Co2 Sensor calibrated \n");
}

void loop(void)
  {
  
    collect_data(); //collect & print data from sensors to serial monitor
    delay(500); //probably going to increase this
  
  }

int collect_data(void)
  {
    int sensorThres = 400; //need to figure out what value to give this
    int methaneValue = analogRead(MQ4); //read MQ4 sensor ///need to figure out singificance of this value and convert it to useful data
    int Co2Value = co2Sensor.read();  //read co2 sensor (value in ppm)
    int checkDHT = DHT.read11(DHT11); //used to check if DHT sensor is operating correctly
    pHValue = find_pH(); //call the pH function to calculate the average pH ///code for this was taken from an example online -- need to rework the code to be personalizd to what we need
    switch (checkDHT){ //checks DHT sensor is working ok, prints status
          case DHTLIB_OK:  
          Serial.print("OK,\n"); 
          break;
          case DHTLIB_ERROR_CHECKSUM: 
          Serial.print("Checksum error,\n"); 
          break;
          case DHTLIB_ERROR_TIMEOUT: 
          Serial.print("Time out error,\n"); 
          break;
          default: 
          Serial.print("Unknown error,\n"); 
          break;
    }
    delay(500);
    print_results(pHValue, Co2Value, methaneValue, sensorThres); //send results to serial monitor
  }

float find_pH(void)
  {
     
  static unsigned long samplingTime = millis();
  static unsigned long printTime = millis();
  static float pHValue,voltage;
  if(millis()-samplingTime > samplingInterval)
    {
        pHArray[pHArrayIndex++]=analogRead(pH);
        if(pHArrayIndex==ArrayLenth)pHArrayIndex=0;
        voltage = avergearray(pHArray, ArrayLenth)*5.0/1024;
        pHValue = 3.5*voltage+Offset;
        samplingTime=millis();
    }
  if(millis() - printTime > printInterval)   //Every 800 milliseconds, print a numerical, convert the state of the LED indicator
    {
      Serial.print("Voltage:");
          Serial.print(voltage,2);
          Serial.print("    pH value: ");
      Serial.println(pHValue,2);
          printTime=millis();
    }
 }
 
double avergearray(int* arr, int number){
  int i;
  int max,min;
  double avg;
  long amount=0;
  if(number<=0)
    {
      Serial.println("Error number for the array to avraging!/n");
      return 0;
    }
  if(number<5)
     {   //less than 5, calculated directly statistics
      for(i=0;i<number;i++)
        {amount+=arr[i];}
      
     avg = amount/number;
     return avg;
    
     }
     
     else
       {
      if(arr[0]<arr[1])
        {min = arr[0];max=arr[1];}
      else
      {min=arr[1];max=arr[0];}
      
      for(i=2;i<number;i++){
        if(arr[i]<min){
          amount+=min;        //arr<min
          min=arr[i];
        }else {
          if(arr[i]>max){
            amount+=max;    //arr>max
            max=arr[i];
          }else{
            amount+=arr[i]; //min<=arr<=max
          }
        }//if
      }//for
      avg = (double)amount/(number-2);
    }//if
    
  return avg;
  
  }
  
void print_results(float pHValue, int Co2Value, int methaneValue, int sensorThres)
  {
    Serial.print("pH Value is ");
    Serial.print(pHValue);
    Serial.print("\n");
    Serial.print("Co2 Value is ");
    Serial.print(Co2Value);
    Serial.print("\n");
    Serial.print("methane Value is ");
    Serial.print(methaneValue);
      if (methaneValue > sensorThres) //likely want to do more complex math here to allow for an accepted average range without telling user value is high or low
        {Serial.print(" (relatively high)");}
      else
        {Serial.print(" (relatively low)");}
    Serial.print("\n");  
    Serial.print("Humidity is ");        
    Serial.print(DHT.humidity, 1); 
    Serial.print("\n");
    Serial.print("Temperature is ");
    Serial.println(DHT.temperature, 1);
    Serial.print("\n");
    }
