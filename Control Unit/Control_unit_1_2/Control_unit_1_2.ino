 /*
         Biomaker Challenge 2017 - Control unit code
Version 0.8 - added method run capabilities         
Version 0.7 - try adding control of the fraction collector
The pressure sensor library is by Luke Miller and can be found at http://github.com/millerlp
*/

//Include Wire library for I2C communication
#include <Wire.h>

//Include library for pressure sensor
#include <MS5803_14.h>

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
int UVPin = A2;
float pressureV;

//Setup an object of the pressure sensor class
MS_5803 pressureSensor = MS_5803(512);

//Other variables
int pumpNr = 1;
int flowRate = 0;

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

  //Initialize the MS5803 pressure sensor
  pressureSensor.initializeMS_5803(false);

  //Initialize data pins
  pinMode(UVPin,INPUT);  
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
    lcd.print("For calibration");
    lcd.setCursor(2,1);
    lcd.print("press enter");
    button = ReadButton();
    
    int senddata = 1;
    
    //Handle button input
    if(button == SELECT)
    {
      lcd.clear();
      lcd.setCursor(2,0);
      lcd.print("Moving arm...");
      delay(1000);
      
      //Send signal to arm to start calibration
      Wire.beginTransmission(fracAddress);
      Wire.write(senddata);
      Wire.endTransmission();
      
      button = ReadButton();
      
      //Wait for calibration to finish and user to press button to do so
      while(true)
      {
        button = ReadButton();
        delay(200);
        lcd.setCursor(0,0);
        lcd.print("Press Enter once");
        lcd.setCursor(0,1);
        lcd.print("calibration done");
        
        //If user confirms,continue
        if(button ==SELECT)
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
  //Initialize dummy arrays
  float flow[10] = {-1,-1,-1,-1,-1,-1,-1,-1,-1,-1};
  float flowVol[10] = {-1,-1,-1,-1,-1,-1,-1,-1,-1,-1};
  int pump[10] = {-1,-1,-1,-1,-1,-1,-1,-1,-1,-1};
  int frac[10] = {-1,-1,-1,-1,-1,-1,-1,-1,-1,-1};
  float fracVol[10] = {-1,-1,-1,-1,-1,-1,-1,-1,-1,-1};

  //To be able to calculate array sizes, calculate the size of a float and an int using this compiler
  float aFloat = 1.0;
  int sizeOfFloat = sizeof(aFloat);
  int anInt = 1;
  int sizeOfInt = sizeof(anInt);

  //Program selection
  lcd.setCursor(1,0);
  lcd.print("Pump Wash");
  lcd.setCursor(1,1);
  lcd.print("GFP-> step");
  lcd.setCursor(0,0);
  cursorY = 0;
  lcd.blink();

  //Wait for user to select a program
  while(true)
  {
    //Allowing scrolling through the menu
    ScrollMenu(button);

    //Check for button presses
    button = ReadButton();

    //Depending on the selection, run the appropriate program
    if(button == SELECT)
    {
      if(cursorY == 0)
      {
        PumpWash(flow,flowVol,pump,frac,fracVol);
        lcd.clear();
        lcd.noBlink();
        break;
      }
      else if (cursorY == 1)
      {
        OnemlHisTrapStep(flow,flowVol,pump,frac,fracVol);
        lcd.clear();
        lcd.noBlink();
        break;
      }
    }
  }
  
  //Calculate the nr. of steps the program has
  int programSteps = 0;
  int ending = 0;

  while(ending == 0)
  {
    if(flow[programSteps] == -1)
    {
      ending = 1;
    }
    else
    {
      programSteps += 1;
    }
  }
  Serial.println(programSteps);
  
  int currentStep = 0;
  int previousCurrentStep = 0;
  
//  //Array for results during fraction run
//  int nrOfFractions = 0;
//  for(int i = 0; i < programSteps; i++)
//  { 
//    int add = flowVol[i]/fracVol[i];
//    nrOfFractions += add;
//  }
//  int fracUVresults[nrOfFractions];
  
  //Start the program
  long startTime = millis();
  long runTime = 0;
  long stepTime = 0;

  for(int i = 0; i < programSteps; i++)
  {
    Serial.println(flow[i]);
  }
  
   //Main program body - this loop will run until all program steps have finished
    for (int j = 0; j < programSteps; j++)
    {
      currentStep = j;
      long stepStartTime = millis();
      stepTime = millis() - stepStartTime;
      bool stepFinished = false;
      bool sampleLoaded = false;
      byte address;
      lcd.clear();
      lcd.setRGB(255,255,255);
      Serial.println("J is:");
      Serial.print(j);
    
      while(!stepFinished)
      {
        
        //If using pumps
        if(flowVol[currentStep] > 0)
        {     
          long totalStepTime = flowVol[currentStep]/flow[currentStep];
          totalStepTime = totalStepTime * 60 * 1000;
          
          //Send a message to the pumps to start flowing
          if(pump[j] == 1)
          {
            address = pump1Address;
          }
          else if(pump[j] == 2)
          {
            address = pump2Address;
          }
          Wire.beginTransmission(address);
          Wire.write(int(flow[j]));
          Wire.endTransmission();
          
           //Start a separate while loop as below to enable saving of average UV values/fraction
           while(totalStepTime > stepTime)
          {
              //Update the run timers
              runTime = millis() - startTime;
              stepTime = millis() - stepStartTime;
              
          //Check if need to collect fractions
          if(frac[j] == 1)
          {
            int fracOn = 4;
            Serial.println("Collect fracs");
            Serial.println(frac[j]);
            
            //Start fraction timer to know when to pulse
            long fracStartTime = millis();
            long fracTime = millis() - fracStartTime;
            long totalFracTime = (fracVol[currentStep]/flow[currentStep])*1000*60;
            Serial.println(totalFracTime);
            delay(1000);
            
            //If yes, activate fraction collector
            Wire.beginTransmission(fracAddress);
            Wire.write(fracOn);
            Wire.endTransmission();
           
           while(totalFracTime > fracTime)
           {
             //Update run timers
              runTime = millis() - startTime;
              stepTime = millis() - stepStartTime;
              fracTime = millis() - fracStartTime;
              Serial.println(fracTime);
             
              //Code to read sensor values
              UV = ReadUVSensor();
              pressureV = float(ReadPressureSensor());

              //Code to save sensor values to SD card
              
              //Display sensor values and maybe also save/send to Rpi?
              lcd.setCursor(0,0);
              lcd.print("A280= ");
              lcd.print(UV);
              lcd.setCursor(11,0);
              lcd.print("*Pmp");
              lcd.print(pump[j]);
              lcd.setCursor(0,1);
              lcd.print("P= ");
              lcd.print(pressureV);
              lcd.setCursor(12,1);
              lcd.print("FR=");
              lcd.print(int(flow[j]));
              
              //Delay depending on interval for sampling UV and pressure values
              delay(1000);     
            }
          } 
                //Code to read sensor values
              UV = ReadUVSensor();
              pressureV = float(ReadPressureSensor());

              //Code to save sensor values to SD card
              
              //Display sensor values and maybe also save/send to Rpi?
              lcd.setCursor(0,0);
              lcd.print("A280= ");
              lcd.print(UV);
              lcd.setCursor(11,0);
              lcd.print("*Pmp");
              lcd.print(pump[j]);
              lcd.setCursor(0,1);
              lcd.print("P= ");
              lcd.print(pressureV);
              lcd.setCursor(12,1);
              lcd.print("FR=");
              lcd.print(int(flow[j]));
              
              //Delay depending on interval for sampling UV and pressure values
              delay(1000);                   
          }
          
          //Finish the step and tell pump to stop flowing
            Wire.beginTransmission(address);
            Wire.write(0);
            Wire.endTransmission();                  
            stepFinished = true;
        }   
        
        //Special case - loading step - only happens if the flow rate is set to 0
        else if(flowVol[currentStep] == 0)
        {
          lcd.clear();
          //Wait until sample loaded, wait for input from a button saying the sample has been loaded 
          while(!sampleLoaded)
          {
            //Update the run timers
            runTime = millis() - startTime;
            stepTime = millis() - stepStartTime;
            
            //Code to read sensor values
            UV = ReadUVSensor();
            pressureV = float(ReadPressureSensor());

            //Code to save sensor values to SD card
            
            //Display sensor values and maybe also save/send to Rpi?
            lcd.setRGB(245,15,15);
            lcd.setCursor(0,0);
            lcd.print("A280= ");
            lcd.print(UV);
            lcd.setCursor(10,0);
            lcd.print("Press");
            lcd.setCursor(0,1);
            lcd.print("P= ");
            lcd.print(pressureV);
            lcd.setCursor(10,1);
            lcd.print("Select");
            
            //Add code to check if button has been pressed that sample is loaded
            button = ReadButton();
            if(button == SELECT)
            {
              sampleLoaded = true;
             Serial.println("Finished load"); 
            }            
          }
          stepFinished = true;
        }
      }
    }
  
   //Code describing what to do when the run is finished
   state = 3;
   lcd.clear();
   lcd.noBlink();
   //Read data from SD card and display somehow
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
            if(button == LEFT)
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
            if(button == LEFT)
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
          lcd.print(" Pump = ");
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
  //Start the run by sending pump ID and flow rate to syringe pump
  if(pumpNr == 1)
  {
    //Start a transmission to the pump at the indicated address
    Wire.beginTransmission(pump1Address);
    
    //Send the flow rate
    Wire.write(flowRate);
    
    //End transmission
    Wire.endTransmission();
  }
  //Start the run by sending pump ID and flow rate to syringe pump
  if(pumpNr == 2)
  {
    //Start a transmission to the pump at the indicated address
    Wire.beginTransmission(pump2Address);
  
    //Send the flow rate
    Wire.write(flowRate);
    
    //End transmission
    Wire.endTransmission();
  }
  
  //Allow initialization of the sensor display values
  int previousUV = ReadUVSensor() + 100;
  float previousPressure = ReadPressureSensor() + 2;
  
  //Display flow rate
  lcd.setCursor(10,1);
  lcd.print("FR: ");
  lcd.print(flowRate);
  
  //Loop to update the sensor values and detect any abort commands
  while(true)
  {
    //Read the UV and pressure sensors and update the display if the value changes significantly
    UV = ReadUVSensor();
    pressureV = ReadPressureSensor();
    
    if(abs(UV - previousUV) > 5)
    {
      lcd.setCursor(1,0);
      lcd.print("A280");
      lcd.setCursor(6,0);
      lcd.print(UV);
    }
    if(abs(pressureV - previousPressure) > 0.05)
    {
      lcd.setCursor(1,1);
      lcd.print("P");
      lcd.setCursor(3,1);
      pressureV = ReadPressureSensor();
      lcd.print(pressureV); 
    }
    
    //Allow run abort using the back button
    button = ReadButton();
    if(button == LEFT)
     {
       state = 7;
       delay(200);
       lcd.clear();
       lcd.noBlink();
       button = ReadButton();
       break;        
     }
  }
}
  
}

//Function to read out the UV sensor
int ReadUVSensor()
{
  int UVvalue = analogRead(UVPin);
  return UVvalue;
}

//Function to read out the pressure sensor - will read in the value from the sensor in Bar and convert to mPa by dividing by 10
float ReadPressureSensor()
{
  //Sensor value
  float value;
  
  //Read the sensor
  pressureSensor.readSensor();
  
  //Assign pressure sensor value (function returns value in mbar) and convert to bar
  value = pressureSensor.pressure()/1000;
  
  //Return the value in mPa
  return value;
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
  byte c;
  
  //Send information to the arm to start calibration
  Wire.beginTransmission(fracAddress);
  Wire.write(1);
  Wire.endTransmission();  
  delay(5000);
  
  //Get acknowledgement that the arm is calibrated
  Wire.requestFrom(fracAddress, byte(1));
  while(Wire.available())
  {
    c = Wire.read();
  }
  
  //If arm acknowledges calibration, return 1 else return 0 - main function will try again
  if(c == 1)
  {
    return 1;
  }
  else
  {
    return 0;
  }
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



/*FPLC methods section - call functions below to populate the methods arrays
 * Add new methods here
 * then add code above to display and allow selection
 * 
 */

 //Runs a 1 ml HisTrap column with a step elution
void OnemlHisTrapStep(float flow[], float flowVol[], int pump[], int frac[], float fracVol[])
{
  float writeflow[4] = {1, 0, 1, 1};    //Set a flow rate for each step - 
  float writeflowVol[4] = {1, 0, 1, 1};
  int writepump[4] = {1, 1, 1, 2};
  int writefrac[4] = {0, 0, 1, 1};
  float writefracVol[4] = {0, 0, 0.2, 0.2};

  float aFloat = 1.0;
  int sizeOfFloat = sizeof(aFloat);

  int arraySize = sizeof(writeflow)/sizeOfFloat;

  //Add values to main array
  for(int i = 0; i < arraySize; i++)
  {
    flow[i] = writeflow[i];
    flowVol[i] = writeflowVol[i];
    pump[i] = writepump[i];
    frac[i] = writefrac[i];
    fracVol[i] = writefracVol[i];
  }
}

//Washes the pumps
void PumpWash(float flow[], float flowVol[], int pump[], int frac[], float fracVol[])
{
  float writeflow[2] = {10, 10};    //Set a flow rate for each step - 
  float writeflowVol[2] = {10,10};
  int writepump[2] = {2, 1};
  int writefrac[2] = {0, 0};
  float writefracVol[2] = {0, 0};

  float aFloat = 1.0;
  int sizeOfFloat = sizeof(aFloat);

  int arraySize = sizeof(writeflow)/sizeOfFloat;

  //Add values to main array
  for(int i = 0; i < arraySize; i++)
  {
    flow[i] = writeflow[i];
    flowVol[i] = writeflowVol[i];
    pump[i] = writepump[i];
    frac[i] = writefrac[i];
    fracVol[i] = writefracVol[i];
  }
}
