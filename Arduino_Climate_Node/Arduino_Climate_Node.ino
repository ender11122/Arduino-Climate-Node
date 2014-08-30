// Arduino Climate Monitor Node
// 30Aug2014
// Adam Green (ndsoccer11122@yahoo.com)

// HCH-1000 Humidity Sensor
// 10Mohm R1
// 220ohm R2

int HumSensorPin = 4;

long HumResult = 0;
unsigned long HumTime1=0;
unsigned long HumTime2=0;

long HumReadingsPer=30.0;
long HumReadDelay=200.0;

void setup()                    // run once, when the sketch starts
{

  Serial.begin(9600);
  Serial.println("start");      // a personal quirk 
}
void loop()                     // run over and over again
{
  long H=evalHumid(HumReadingsPer, HumSensorPin);
  Serial.println("_____");
  Serial.println(H);
  Serial.println("_____");
  delay(100);

}

long evalHumid(long samples, int sPin){
  long avgtime=0; 
  for (int i=0;i<samples;i++){
    RCtime(sPin);
    avgtime+= decayTime(3);
  }
  avgtime=avgtime/samples; 
  return(avgtime);

}

long RCtime(int sensPin){
  long result = 0;
  pinMode(sensPin, OUTPUT);       // make pin OUTPUT
  digitalWrite(sensPin, HIGH);    // make pin HIGH to discharge capacitor - study the schematic
  delay(1);                       // wait a  ms to make sure cap is discharged

  pinMode(sensPin, INPUT);  // turn pin into an input and time till pin goes low
  digitalWrite(sensPin, LOW);// turn pullups off - or it won't work
  decayTime(1);  
  while(digitalRead(sensPin)){    // wait for pin to go low
    //result++;
  }
  decayTime(2);
  delay(readDelay); 
  //return result;                   // report results   
}

long decayTime(int input){
  if (input==1){
    time1=micros();
  }
  if (input==2 ){
    time2=micros(); 
  }
  if (input==3){
    return (time2-time1);
  }
}
