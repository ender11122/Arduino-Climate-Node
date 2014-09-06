// Arduino Climate Monitor Node
// 30Aug2014
// Adam Green (ndsoccer11122@yahoo.com)

//Acknowledgments:
//Humidity Credit to: dpotthast from arduino forums 
//Temperature Credit to: Milan Malesevic and Zoran Stupid from arduino playground
//Wireless Credit to: Glyn Hudson openenergymonitor.org GNU GPL V3 7/7/11
//Wireless Library Credit to : JCW from Jeelabs.org

//Components:
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

//wireless includes and definitions
#include <JeeLib.h>
#include <Ports.h>
#include <PortsBMP085.h>
#include <PortsLCD.h>
#include <PortsSHT11.h>
#include <RF12.h>
#include <RF12sio.h>
#define myNodeID 10          //node ID of tx (range 0-30)
#define network     210      //network group (can be in the range 1-250).
#define RF_freq RF12_433MHZ     //Freq of RF12B can be RF12_433MHZ, RF12_868MHZ or RF12_915MHZ. Match freq to module

//wireless payload
typedef struct { int temperature, humidity; } PayloadTX;      // create structure - a neat way of packaging data for RF comms
PayloadTX climate;  

#include <math.h>

//temperature calculation values
float vishayA = 0.003354f;
float vishayB = 0.000256985f;
float vishayC = 0.000002620131f;
float vishayD = 0.00000006383091f;                                 
float ThermistorRo = 10000.0f;                 
int TempSensorPin = 0;
float TempBalanceR = 10050.0f;
//humidity calculation values
int HumSensorPin = 4;
float HumTime1=0;
float HumTime2=0;
long HumReadingsPer=4;


void setup() {
  rf12_initialize(myNodeID,RF_freq,network);   //Initialize RFM12 with settings defined above  
  Serial.begin(4800);
}


void loop()                     // run over and over again
{
  climate.humidity=(100 - (0.61364f * ((evalHumid(HumReadingsPer, HumSensorPin)) - 10)));
  climate.temperature = CalculateTemp(analogRead(TempSensorPin));
  delay(50);

  rf12_recvDone(); // ignore incoming packets
  if (rf12_canSend()){
    rf12_sendNow(0, &climate, sizeof climate);  
    delay(10);
    rf12_sendWait(2);
  }
  Serial.println(climate.humidity);
  Serial.println(climate.temperature);
  delay(50);
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
  delay(100);                       // wait 500ms to make sure cap is discharged

  pinMode(sensPin, INPUT);  // turn pin into an input and time till pin goes low
  digitalWrite(sensPin, LOW);// turn pullups off - or it won't work
  while(digitalRead(sensPin)){result++;}    // wait for pin to go low 
  return result; 
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



