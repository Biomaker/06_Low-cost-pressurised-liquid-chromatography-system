//Include Wire library for I2C communication
#include <Wire.h>

//Include Grove RGB LCD library
#include "rgb_lcd.h"

//Key inputs for analog buttons
#define UP       0
#define DOWN     1
#define LEFT     2
#define RIGHT    3
#define SELECT   4
#define NOKEY    5
int buttonPin = A3;

//Start an instance of the rgb_lcd class called lcd
rgb_lcd lcd;

//Variables for cursor position
int cursorX = 0;
int cursorY = 0;

void setup()
{
  //Start serial communication
  Serial.begin(9600);
  
  //Start I2C communication
  Wire.begin();
  
  //Setup the LCD - 16 characters per line, 2 lines - background white
  lcd.begin(16,2);
  lcd.setRGB(255,255,255);
  
}

void loop()
{


  
}

//Function to check if I2C device responds - derived from I2Cscanner by Nick Gammon 
//http://www.gammon.com.au/forum/?id=10896
bool checkI2Cdevice(byte deviceaddress)
{
  byte error;
  
  //Check if the device at the indicated address responds and then return true if connected
  Wire.beginTransmission(deviceaddress);
  error = Wire.endTransmission();
  
  if(error == 0)
  {
    return true; 
  }
  else
  {
    return false;
  }
}


//Function to read button input from one analog pin defined in buttonPin
//Uses five buttons attached to different resistor values to get a different analog
//output depending on the button pressed.
int ReadButton()
{
  //Read the analog button
  int keyvalue = analogRead(buttonPin);
  
  //Check value and assign key pressed (calibrated values) - if you change the resistors, these values will change
  if(keyvalue <= 300)
  {
    return NOKEY;
  }
  if(keyvalue <= 599)
  {
    return SELECT;
  }
  if(keyvalue <= 749)
  {
    return RIGHT;
  }
  if(keyvalue <= 949)
  {
    return LEFT;
  }
   if(keyvalue <= 989)
  {
    return DOWN;
  } 
  if(keyvalue <= 1023)
  {
    return UP;    
  } 
}
