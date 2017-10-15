//Include Wire library for I2C communication
#include <Wire.h>

//Include Grove RGB LCD library
#include "rgb_lcd.h"

//Addresses for I2C devices
byte pump1Address = 9;
byte pump2Address = 10;
byte fracAddress = 11;

//Key inputs for analog buttons
#define UP       0
#define DOWN     1
#define LEFT     2
#define RIGHT    3
#define SELECT   4
#define NOKEY    5
int buttonPin = A3;
int button;

//Start an instance of the rgb_lcd class called lcd
rgb_lcd lcd;

//Variables for cursor position
int cursorX = 0;
int cursorY = 0;

//Variables to monitor menu progression
int state = 0;
int cont = 0;

//Variables for sensor values
int UV;
int pressureV;

//Other variables
int pumpNr;
int flowRate;

void setup()
{
  //Start serial communication
  Serial.begin(9600);
  
  //Start I2C communication
  Wire.begin();
  
  //Setup the LCD - 16 characters per line, 2 lines - background white
  lcd.begin(16,2);
  lcd.setRGB(255,255,255);
  lcd.noBlink();
  lcd.clear();  
}

void loop()
{
 //Title screen
  while (state == 0)
  {
    //Show the title screen
    lcd.setCursor(2,0);
    lcd.print("Open-source");
    lcd.setCursor(4,1);
    lcd.print("FPLC 1.0");
    delay(2000);
    lcd.clear();
    lcd.setCursor(4,0);
    lcd.print("Biomaker");
    lcd.setCursor(1,1);
    lcd.print("Challenge 2017");
    delay(2000);
    lcd.clear();
    state = 1;
  }
  
  //Check connection to I2C devices 
  while(state ==1)
  {
    lcd.setCursor(1,0);
    lcd.print("Connecting....");
    delay(1000);
    
    //Check if pump 1 is connected and display results on LCD
    if(checkI2Cdevice(pump1Address))
    {
      lcd.clear();
      lcd.print("Pump 1 connected");
      delay(1000);
    }
    else
    {
      lcd.clear();
      lcd.setCursor(1,0);
      lcd.print("Cannot connect");
      lcd.setCursor(5,1);
      lcd.print("Pump 1");
      delay(1000);    
    }
    
    //Check if pump 2 is connected and display results on LCD
    if(checkI2Cdevice(pump2Address))
    {
      lcd.clear();
      lcd.print("Pump 2 connected");
      delay(1000);
    }
    else
    {
      lcd.clear();
      lcd.setCursor(1,0);
      lcd.print("Cannot connect");
      lcd.setCursor(5,1);
      lcd.print("Pump 2");
      delay(1000);    
    }
  
    //Check if fraction collector is connected and display results on LCD
    if(checkI2Cdevice(fracAddress))
    {
      lcd.clear();
      lcd.print(" Frac connected ");
      delay(1000);
    }
    else
    {
      lcd.clear();
      lcd.setCursor(1,0);
      lcd.print("Cannot connect");
      lcd.setCursor(6,1);
      lcd.print("Frac");
      delay(1000);    
    }
  
  //Display a menu that allows user to either continue or try to reconnect devices
    lcd.clear();
    lcd.setCursor(4,0);
    lcd.print("Continue");
    lcd.setCursor(4,1);
    lcd.print("Reconnect");
    lcd.setCursor(0,0);
    lcd.blink();
    cursorX = 0;
    cursorY = 0;
    
    //Process input from buttons to allow menu selection
    while(true)
    {
      //Read the input buttons and move cursor according to the choice made (up or down only) 
      button = ReadButton();
      ScrollMenu(button);
      
      //Process input from the select button
      if(button == SELECT)
      {
        if(cursorY == 0)
        {
          state = 2;
          delay(200);
          lcd.clear();
          lcd.noBlink();
          break;
        }
        else if (cursorY == 1)
        {
          lcd.clear();
          delay(200);
          break;
        }
      }
    }
  }

//Start the startup procedure to calibrate the robot arm
  while(state == 2)
  { 
    //Display instructions prior to calibration
    lcd.setCursor(0,0);
    lcd.print("Remove platerack");
    lcd.setCursor(0,1);
    lcd.print("then press enter");
    button = ReadButton();
    
    //Handle button input
    if(button == SELECT)
    {
      lcd.clear();
      lcd.setCursor(2,0);
      lcd.print("Moving arm...");
      delay(1000);
      
      //When user presses select start calibrating
      while(true)
      {
        //Calibrate arm code
        if(CalibrateArm() == 1)
        {
          lcd.clear();
          lcd.setCursor(1,0);
          lcd.print("Arm calibrated");
          state = 3;
          delay(1000);
          lcd.clear();
          break;
        }
      }      
    }   
      //Process input from BACK button (LEFT)
      if(button == LEFT)
      {
          state = 1;
          delay(200);
          lcd.clear();
          lcd.noBlink();
          break;        
      }    
  }
  
  //Main menu
  while(state == 3)
  {
    //Display the menu
    lcd.setCursor(1,0);
    lcd.print("Manual Run");
    lcd.setCursor(1,1);
    lcd.print("Method Run");
    lcd.setCursor(0,0);
    lcd.blink();
    
    cursorY = 0;
    cursorX = 0;
    
    //Allow button input for up/down scrolling through the menu and selecting
    while(true)
    {
      //Read button and process input accordingly
      button = ReadButton();
      ScrollMenu(button);
      
      //Process input from the select button - jump to appropriate menu item
      if(button == SELECT)
      {
        if(cursorY == 0)
        {
          state = 4;
          delay(200);
          lcd.clear();
          lcd.noBlink();
          break;
        }
        else if (cursorY == 1)
        {
          state = 5;
          delay(200);
          lcd.clear();
          lcd.noBlink();
          break;
        }
      }
      
      //Process input from BACK button (LEFT)
      if(button == LEFT)
      {
          state = 2;
          delay(200);
          lcd.clear();
          lcd.noBlink();
          break;        
      }
    }
  }

//Manual run menu
while(state == 4)
{
  //Code for manual run mode - display menu
  lcd.setCursor(1,0);
  lcd.print("Start pumps");
  lcd.setCursor(1,1);
  lcd.print("Sensor values");
  lcd.setCursor(0,0);
  lcd.blink();
  cursorY = 0;
  cursorX = 0;
   
  //Button input handling
  while(true)
  {
    button = ReadButton();
    ScrollMenu(button);
    
    //Process input from the select button - jump to appropriate menu item
      if(button == SELECT)
      {
        if(cursorY == 0)
        {
          state = 7;
          delay(200);
          lcd.clear();
          lcd.noBlink();
          break;
        }
        else if (cursorY == 1)
        {
          state = 6;
          delay(200);
          lcd.clear();
          lcd.noBlink();
          break;
        }
      }
    
    //Process input from BACK button (LEFT)
      if(button == LEFT)
      {
          state = 3;
          delay(200);
          lcd.clear();
          lcd.noBlink();
          break;        
      }
  }
}

//Method run menu
while(state == 5)
{
  //Code for automatic run mode - GFP program - step elute
  //Display available programs
  //Allow button input to select
  
  //Control pumps and fraction collector accordingly
}

//Display sensor values menu
while(state == 6)
{
  lcd.setCursor(1,0);
  lcd.print("Abs280");
  lcd.setCursor(10,0);
  UV = ReadUVSensor();
  lcd.print(UV);
  lcd.setCursor(1,1);
  lcd.print("Pressure");
  lcd.setCursor(10,1);
  pressureV = ReadPressureSensor();
  lcd.print(pressureV);
  
  for(int i = 0; i <=20; ++i)
  {
    button = ReadButton();
    //Process input from BACK button (LEFT)
      if(button == LEFT)
      {
          state = 4;
          delay(200);
          lcd.clear();
          lcd.noBlink();
          break;        
      }
     delay(100);    
  }
}

//Manual pump control menu
while(state == 7)
{
  pumpNr = 1;
  flowRate = 0;
  
  //Add code for manual pump control - for now, only allow back
  lcd.setCursor(1,0);
  lcd.print("Pump nr.");
  lcd.setCursor(15,0);
  lcd.print(pumpNr);
  lcd.setCursor(1,1);
  lcd.print("Flow rate");
  lcd.setCursor(15,1);
  lcd.print(flowRate);
  lcd.setCursor(0,0);
  lcd.blink();
  cursorY = 0;
  cursorX = 0;

  
  while(true)
  {
    button = ReadButton();
    ScrollMenu(button);
    lcd.blink();
    
    //Process input from BACK button (LEFT)
      if(button == LEFT)
      {
          state = 4;
          delay(200);
          lcd.clear();
          lcd.noBlink();
          break;        
      }
      
      //Enter the scroll menu to change value
      if(button == RIGHT)
      {
        //Change the pump nr.
        if(cursorY == 0)
        {
          while(true)
          {
            lcd.setCursor(15,cursorY);
            button = ReadButton();
            
            //If press select, return to menu
            if(button == SELECT)
            {
              delay(200);
              button = ReadButton();
              lcd.setCursor(0,cursorY);
              break;        
             }
            //If press up or down, change value of the pump
            if(button == UP)
            {
              if(pumpNr == 1)
              {
                pumpNr += 1;
              }
              else if(pumpNr == 2)
              {
                pumpNr -= 1;
              }
              lcd.print(pumpNr);
              delay(200);
            }
            if(button == DOWN)
            {
              if(pumpNr == 1)
              {
                pumpNr += 1;
              }
              else if(pumpNr == 2)
              {
                pumpNr -= 1;
              }
              lcd.print(pumpNr);
              delay(200);
            }           
          }
        }
        //Change the flow rate
        if(cursorY == 1)
        {
          while(true)
          {
            lcd.setCursor(15,cursorY);
            button = ReadButton();
            //Process input from BACK button (LEFT)
            if(button == SELECT)
            {
              delay(200);
              button = ReadButton();
              lcd.setCursor(0,cursorY);
              break;        
             }
            //If press up or down, change value of the pump
            if(button == UP)
            {
              if(flowRate < 5)
              {
                flowRate += 1;
              }
              else if(flowRate >= 5)
              {
                flowRate = 0;
              }
              lcd.print(flowRate);
              delay(200);
            }
            if(button == DOWN)
            {
              if(flowRate > 0)
              {
                flowRate -= 1;
              }
              else if(flowRate <= 0)
              {
                flowRate = 5;
              }
              lcd.print(flowRate);
              delay(200);
            } 
          }
        }
      }
      
      //Start the run if press SELECT
      if(button == SELECT)
      {
        lcd.clear();
        lcd.noBlink();
        
      //Confirmation screen  
        while(true)
        {
          lcd.setCursor(0,0);
          lcd.print("FR = ");
          lcd.print(flowRate);
          lcd.print(" Pump =");
          lcd.print(pumpNr);
          lcd.setCursor(0,1);
          lcd.print("Proceed?");
          delay(200);
        
          button = ReadButton();
          if(button == LEFT)
          {
            state = 7;
            delay(200);
            lcd.clear();
            lcd.noBlink();
            break;        
          }
          if(button == SELECT)
           {
             state = 8;
             delay(200);
             lcd.clear();
             lcd.noBlink();
             break;
           } 
         }
        break;
      }
    
  }
}

//Manual run monitoring
while(state == 8)
{
  //Display UV and pressure data allow cancelling of run using back button
  //Display flow rate
}
  
}

//Function to read out the UV sensor
int ReadUVSensor()
{
  int UVvalue = 120;
  return UVvalue;
}

//Function to read out the pressure sensor
int ReadPressureSensor()
{
  int pressureValue = 140;
  return pressureValue;
}

//Function that will scroll through a menu and display the cursor appropriately
void ScrollMenu(int button)
{
  //Process input from DOWN button and move cursor depending on the current location of the cursor    
      if(button == DOWN)
      {
        if(cursorY == 0)
        {
          cursorY += 1;
          delay(200);
        }
        else if(cursorY == 1)
        {
          cursorY -= 1;
          delay(200);
        }
        lcd.setCursor(0,cursorY);
      }
  //Process input from UP button and move cursor depending on the current location of the cursor
      if(button == UP)
      {
        if(cursorY == 0)
        {
          cursorY += 1;
          delay(200);
        }
        else if(cursorY == 1)
        {
          cursorY -= 1;
          delay(200);
        }
        lcd.setCursor(0,cursorY);
      }  
}

//Function that communicates with the arm Arduino and reports back when the arm is calibrated
int CalibrateArm()
{
  //Send information to the arm to start calibration
  
  //Get acknowledgement that the arm is calibrated
  
  return 1;
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
