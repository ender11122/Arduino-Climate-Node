// Simple RFM12B sender program, with ACK and optional encryption
// It initializes the RFM12B radio with optional encryption and passes through any valid messages to the serial port
// felix@lowpowerlab.com

#include <RFM12B.h>
#include <avr/sleep.h>

// You will need to initialize the radio by telling it what ID it has and what network it's on
// The NodeID takes values from 1-127, 0 is reserved for sending broadcast messages (send to all nodes)
// The Network ID takes values from 0-255
// By default the SPI-SS line used is D10 on Atmega328. You can change it by calling .SetCS(pin) where pin can be {8,9,10}
#define NODEID        2  //network ID used for this unit
#define NETWORKID    99  //the network ID we are on
#define GATEWAYID     1  //the node ID we're sending to
#define ACK_TIME     50  // # of ms to wait for an ack
#define SERIAL_BAUD  115200

//encryption is OPTIONAL
//to enable encryption you will need to:
// - provide a 16-byte encryption KEY (same on all nodes that talk encrypted)
// - to call .Encrypt(KEY) to start encrypting
// - to stop encrypting call .Encrypt(NULL)
uint8_t KEY[] = "ABCDABCDABCDABCD";

int interPacketDelay = 1000; //wait this many ms between sending packets
char input = 0;


double RL = 219; 
double Turns = 2000;
double VoltsPer = 0.00488;
double RmsFactor = 0.707;
double VSenseRMS = 0;
double Irms = 0;
double VAC = 134;
long int cumulativeVal = 0;
int sensorPin = A1;    
int sensorValue = 0;  
int newValue = 0;


// Need an instance of the Radio Module
RFM12B radio;
byte sendSize=0;
//char payload[] = "abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ1234567890~!@#$%^&*(){}[]`|<>?+=:;,.";
typedef struct { float Voltage, Current, Power; } PayloadTX;
PayloadTX powerstats;
bool requestACK=true;

void setup()
{
  Serial.begin(SERIAL_BAUD);
  radio.Initialize(NODEID, RF12_433MHZ, NETWORKID);
  radio.Encrypt(KEY);
  radio.Sleep(); //sleep right away to save power
  Serial.println("Transmitting...\n\n");
}

void loop()
{
  
  for(int p=0; p < 25; p++){   
    for(int i=0; i < 100; i++){
      // read the value from the sensor:
        newValue = analogRead(sensorPin);
        if (newValue > sensorValue){
          sensorValue = newValue;
        }
      }
    cumulativeVal = cumulativeVal + sensorValue;
  }
  sensorValue = cumulativeVal / 25;
  cumulativeVal = 0;
  Serial.println(sensorValue);
  VSenseRMS = sensorValue * VoltsPer * RmsFactor;
  Irms = VSenseRMS * Turns / RL;
  //Serial.println("***************");
  //Serial.print("VSenseRMS: ");
  //Serial.println(VSenseRMS);
  //Serial.print("Irms = ");
  //Serial.println(Irms);
  //Serial.print("Power = ");
  //Serial.println(Irms*VAC);
  //Serial.println("***************");
  powerstats.Voltage = VSenseRMS;
  powerstats.Current = Irms;
  powerstats.Power = Irms*VAC;
  sensorValue = 0;
  delay(2000);
  
  
  Serial.print("Sending: ");
  Serial.print(powerstats.Voltage);
  Serial.print(" ");
  Serial.print(powerstats.Current);
  Serial.print(" ");
  Serial.print(powerstats.Power);
  
  //requestACK = !(sendSize % 3); //request ACK every 3rd xmission
  
  radio.Wakeup();
  radio.Send(GATEWAYID, &powerstats, sizeof powerstats, requestACK);
  if (requestACK)
  {
    Serial.print(" - waiting for ACK...");
    if (waitForAck()) Serial.print("ok!");
    else Serial.print("nothing...");
  }
  radio.Sleep();
  Serial.println();
  delay(2000);
}

// wait a few milliseconds for proper ACK, return true if received
static bool waitForAck() {
  long now = millis();
  while (millis() - now <= ACK_TIME)
    if (radio.ACKReceived(GATEWAYID))
      return true;
  return false;
}
