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
#define Vishay_10K_Therm 4977.0f,298.15f,10000.0f //define thermistor B,To,R

// High Humidity (95%) Val = 18;
// Low Humidity (5%) Val = ??;
// Check Humidity (68%) Value = 62

int HumSensorPin = 4;
int TempSensorPin = 0;
float TempBalanceR = 10000;

long HumResult = 0;
float HumTime1=0;
float HumTime2=0;

long HumReadingsPer=1;
long HumReadDelay=1000.0;

void setup()                    // run once, when the sketch starts
{

  Serial.begin(9600);
  Serial.println("start");      // a personal quirk 
}
void loop()                     // run over and over again
{
  long H=evalHumid(HumReadingsPer, HumSensorPin);
  long TempValue = Temperature(TempSensorPin,Vishay_10K_Therm,TempBalanceR);
  Serial.println("_____");
  Serial.println("Temperature");
  Serial.println(TempValue);
  Serial.println("Humidity");
  Serial.println(H);
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
  delay(5000);                       // wait 1s to make sure cap is discharged

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

float Temperature(int AnalogInputNumber,float B,float T0,float R0,float R_Balance)
{
  float R,T;
  R=R_Balance*(1024.0f/float(analogRead(AnalogInputNumber))-1);
  T=1.0f/(1.0f/T0+(1.0f/B)*log(R/R0));
  T=9.0f*(T-273.15f)/5.0f+32.0f;
  return T;
}
