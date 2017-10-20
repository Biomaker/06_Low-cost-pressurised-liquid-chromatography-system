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
int button;
int UV;
float pressureV;
int cursorY = 0;

//Start an instance of the rgb_lcd class called lcd
rgb_lcd lcd;

byte pump1Address = 9;
byte pump2Address = 10;
byte fracAddress = 11;

void setup()
{
  Wire.begin();
  Serial.begin(9600);
  //Setup the LCD - 16 characters per line, 2 lines - background white
  lcd.begin(16,2);
  lcd.setRGB(255,255,255);
  lcd.noBlink();
  lcd.clear();
  delay(2000);
}

void loop()
{
  //Program the run using multiple arrays
  //This program runs:
  // 5 ml equilibration, pause to load sample, 5 ml wash, 5 ml elution - collects fractions during wash and elution
//  float flow[4] = {1, 0, 1, 1};    //Set a flow rate for each step - 
//  float flowVol[4] = {1, 0, 1, 1};
//  int pump[4] = {1, 1, 1, 2};
//  int frac[4] = {1, 0, 1, 1};
//  float fracVol[4] = {1, 0, 1, 1};

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
  delay(5000);
  
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
   //Read data from SD card and display somehow
}


//Set of dummy functions mimicking the real fraction collector functions for testing purposes

//Dummy UV sensor function - send arbitrary values
int ReadUVSensor()
{
  int UVvalue = millis()/1000;
  return UVvalue;
}

//Dummy pressure sensor function - send arbitrary values
float ReadPressureSensor()
{
  float value = millis()/4000;
  return value;
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

//Runs a 1 ml HisTrap column with a step elution
void OnemlHisTrapStep(float flow[], float flowVol[], int pump[], int frac[], float fracVol[])
{
  float writeflow[4] = {1, 0, 1, 1};    //Set a flow rate for each step - 
  float writeflowVol[4] = {5, 0, 5, 5};
  int writepump[4] = {1, 1, 1, 2};
  int writefrac[4] = {0, 0, 1, 1};
  float writefracVol[4] = {0, 0, 0.5, 0.5};

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
  float writeflow[2] = {5, 5};    //Set a flow rate for each step - 
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

