// Arduino Receiver Node
// 30Aug2014
// Adam Green (ndsoccer11122@yahoo.com)

//Acknowledgments:
//Wireless Credit to: Glyn Hudson openenergymonitor.org GNU GPL V3 7/7/11
//Wireless Library Credit to : JCW from Jeelabs.org

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

const int emonTx_NodeID=10;            //emonTx node ID
int read_buffer;

void setup() {
  rf12_initialize(myNodeID,RF_freq,network);   //Initialize RFM12 with settings defined above  
  Serial.begin(9600);   
}

void loop() {
  if (rf12_recvDone()) {    
     if (rf12_crc == 0 && (rf12_hdr & RF12_HDR_CTL) == 0) {
  
       int node_id = (rf12_hdr & 0x1F);		  //extract nodeID from payload
          
       if (node_id == emonTx_NodeID)  {             //check data is coming from node with the corrct ID
           climate=*(PayloadTX*) rf12_data;            // Extract the data from the payload

       } 
     }
   }
   Serial.println((int) Serial.read());
   delay(200);
}

 
