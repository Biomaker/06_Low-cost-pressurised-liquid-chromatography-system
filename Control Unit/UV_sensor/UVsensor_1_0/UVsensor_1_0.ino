//Simple sketch for testing of the UV sensor in isolation - in the control unit, this simple code is implemented as a function

//Constants for UV sensor
const int sensorPin = A0;
int sensorValue;

//Constants for UV LED
const int UVledPin = 10;
int UVbrightness = 0;

//Constants for the potentiometer, wh
const int potPin = A1;
int potValue;

unsigned long timeElapsed;

void setup()
{
  //Set pins
  pinMode(sensorPin, INPUT);
  pinMode(UVledPin, OUTPUT);
  pinMode(potPin, INPUT);
  
  //Start serial communication
  Serial.begin(9600);
  
  timeElapsed = 0;
  
}

void loop()
{
  //Read all sensors and timestamp
   potValue = analogRead(potPin);
   timeElapsed = millis()/1000;
   sensorValue = analogRead(sensorPin);
   
   //Output from sensors
   dataOutput(timeElapsed,sensorValue); 
   UVbrightness = convertAnalogToDigital(potValue);
    
   //Use the potentiometer value to set the UV LED brightness 
   analogWrite(UVledPin,UVbrightness);
  
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
