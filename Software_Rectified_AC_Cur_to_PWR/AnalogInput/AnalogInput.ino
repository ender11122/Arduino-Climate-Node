/*
  Analog Input
 Demonstrates analog input by reading an analog sensor on analog pin 0 and
 turning on and off a light emitting diode(LED)  connected to digital pin 13. 
 The amount of time the LED will be on and off depends on
 the value obtained by analogRead(). 
 
 The circuit:
 * Potentiometer attached to analog input 0
 * center pin of the potentiometer to the analog pin
 * one side pin (either one) to ground
 * the other side pin to +5V
 * LED anode (long leg) attached to digital output 13
 * LED cathode (short leg) attached to ground
 
 * Note: because most Arduinos have a built-in LED attached 
 to pin 13 on the board, the LED is optional.
 
 
 Created by David Cuartielles
 modified 30 Aug 2011
 By Tom Igoe
 
 This example code is in the public domain.
 
 http://arduino.cc/en/Tutorial/AnalogInput
 
 */

long int RL = 234;
long int Turns = 2000;
float VoltsPer = 0.00488;
float RmsFactor = 0.707;
float VSenseRMS = 0;
float Irms = 0;
int VAC = 134;
int sensorPin = A0;    // select the input pin for the potentiometer
int ledPin = 13;      // select the pin for the LED
int sensorValue = 0;  // variable to store the value coming from the sensor
int newValue = 0;

void setup() {
  // declare the ledPin as an OUTPUT:
  pinMode(ledPin, OUTPUT);  
  Serial.begin(9600);
}

void loop() {
  for(int i=0; i < 1000; i++){
  // read the value from the sensor:
  newValue = analogRead(sensorPin);
  if (newValue > sensorValue){
    sensorValue = newValue;
  }
  }
  VSenseRMS = sensorValue*VoltsPer*RmsFactor;
  Irms = VSenseRMS / RL * Turns;
  Serial.println("***************");
  Serial.print("Irms = ");
  Serial.println(Irms);
  Serial.print("Power = ");
  Serial.println(Irms*VAC);
  Serial.println("***************");
  sensorValue = 0;
  delay(2000);
  // turn the ledPin on  
  // stop the program for <sensorValue> milliseconds:         
  // turn the ledPin off:        
 
  // stop the program for for <sensorValue> milliseconds:                 
}
