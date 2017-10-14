//Constants for UV sensor
const int sensorPin = A2;
int sensorValue;

unsigned long timeElapsed;

void setup()
{
  //Set pins
  pinMode(sensorPin, INPUT);;
  
  //Start serial communication
  Serial.begin(9600);
  
  timeElapsed = 0;
  
}

void loop()
{
  //Read all sensors and timestamp
   timeElapsed = millis()/1000;
   sensorValue = analogRead(sensorPin);
   
   //Output from sensors
   dataOutput(timeElapsed,sensorValue); 
  
   delay(1000 );
   
}


//Function to convert the potentiometer analog output as a digital PWM input
int convertAnalogToDigital(int input)
{
   float convert;
   convert = (float(input)/1023)*255;
   
   int output = int(convert);
   return output;
}

void dataOutput(unsigned long time,int value)
{
  Serial.print("Time: ");
  Serial.print(time);
  Serial.print("  ");
  Serial.print("OD280: ");
  Serial.print(value);
  Serial.println();  
}
