// Arduino Climate Monitor Node
// 30Aug2014
// Adam Green (ndsoccer11122@yahoo.com)

//humidity code credit to dpotthast from arduino forums 
//temp code credit to Milan Malesevic and Zoran Stupid from arduino playground

// HCH-1000 Humidity Sensor
// Vishay NTCLE100E3103HT1 Thermistor
// Arduino Uno


// Temp Sensor Circuit Diagram

// 5v ------- Thermistor ------- 10Kohm ------- GND
//                          |
//                       Analog 0


// Humidity Sensor Circuit Diagram

// GND ----------- Sensor ------ 220ohm ----- Dig 4
//              |           |
//              ----1Moh----



#include <math.h>

float vishayA = 0.003354f;
float vishayB = 0.000256985f;
float vishayC = 0.000002620131f;
float vishayD = 0.00000006383091f;                                 
float ThermistorRo = 10000.0f;                 
int TempSensorPin = 0;
float TempBalanceR = 10050.0f;

int HumSensorPin = 4;
float HumTime1=0;
float HumTime2=0;
long HumReadingsPer=4;
long HumReadDelay=100.0;


void setup()                    // run once, when the sketch starts
{
  Serial.begin(9600); 
}

void loop()                     // run over and over again
{
  int HumidityValue=(100 - (0.61364f * ((evalHumid(HumReadingsPer, HumSensorPin)) - 10)));
  int TempValue = CalculateTemp(analogRead(TempSensorPin));
  Serial.println("_____");
  Serial.println("Temperature (degF)");
  Serial.println(TempValue);
  Serial.println("Humidity (%relative)");
  Serial.println(HumidityValue);
  Serial.println("_____");
  delay(100);
}




long evalHumid(long samples, int sPin){
  long avgtime=0; 
  for (int i=0;i<samples;i++){
    //RCtime(sPin);
    //avgtime+= decayTime(3);
    avgtime+= RCtime(sPin);
  }
  avgtime=avgtime/samples; 
  return(avgtime);
  
}

long RCtime(int sensPin){
  long result = 0;
  pinMode(sensPin, OUTPUT);       // make pin OUTPUT
  digitalWrite(sensPin, HIGH);    // make pin HIGH to charge capacitor - study the schematic
  delay(2500);                       // wait 1s to make sure cap is discharged

  pinMode(sensPin, INPUT);  // turn pin into an input and time till pin goes low
  digitalWrite(sensPin, LOW);// turn pullups off - or it won't work
  //decayTime(1);
  while(digitalRead(sensPin)){result++;}    // wait for pin to go low 
  return result;
  //decayTime(2);
  //return decayTime(3);  
}

long decayTime(int input){
  if (input==1){
    HumTime1=micros();
  }
  if (input==2 ){
    HumTime2=micros(); 
  }
  if (input==3){
    return (HumTime2-HumTime1);
  }
}



float CalculateTemp(int RawADC) {
  long Resistance;  
  float Temp;  // Dual-Purpose variable to save space.

  Resistance= TempBalanceR * ((1024.0f / RawADC) - 1.0f); 
  Temp = log(Resistance / ThermistorRo);
  Temp = 1.0f / (vishayA + (vishayB * Temp) + (vishayC * (Temp * Temp)) + (vishayD * (Temp * Temp * Temp)));
  Temp = Temp - 273.15f;                            // Convert Kelvin to Celsius                      
  Temp = (Temp * 9.0f)/ 5.0f + 32.0f;               // Convert to Fahrenheit
  return Temp;                                      // Return the Temperature
}



