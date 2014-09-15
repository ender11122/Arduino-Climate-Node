

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

void setup() {  
  Serial.begin(9600);
}

void loop() {
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
  Serial.println("***************");
  Serial.print("VSenseRMS: ");
  Serial.println(VSenseRMS);
  Serial.print("Irms = ");
  Serial.println(Irms);
  Serial.print("Power = ");
  Serial.println(Irms*VAC);
  Serial.println("***************");
  sensorValue = 0;
  delay(2000);
               
}
