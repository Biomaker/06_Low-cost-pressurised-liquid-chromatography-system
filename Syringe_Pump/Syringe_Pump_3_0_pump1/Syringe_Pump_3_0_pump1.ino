/*
Version 3.0 - Biomaker Challenge 2017 - Team 6 - October 2017
Sketch for running a syringe pump either controlled manually using the up and down buttons on the pump
or automatically under control by a control unit over I2C communication.

For now all units are coded for a 50 ml syringe and a NEMA17 12 V, 350 mA stepper, 1.8 deg per step.
If another setup is required change the appropriate variables below.

I2C communication allows an external Arduino to set the flow rate in ml/min. This sketch receives the input
and converts this to the required steps/min for the syringe and motor used.

Default state, with no I2C device connected is manual run using an UP and a DOWN button on the front of
the syringe pump. I2C communication, if present takes precedence and will disable the manual running.
*/

//Include the Wire library for I2C communication to the control unit
#include <Wire.h>

//Motor control variables - defined by wiring - see circuit diagrams
int stepPin = 8;
int dirPin = 10;
int upButtonPin = 9;
int downButtonPin = 11;

int steps;
int stepInterval = 10; //Milliseconds between steps

//Syringe and pump variables - change these when changing syringe
float volumePerMM = 1.8;    //50 ml syringe has 1.8 mm/ml, volume per step is in ml
float distPerStep = 0.004;  //Pitch of an M5 threaded rod 0.8 mm - 0.8 mm/360 degrees. The motor used does 1.8 deg/step -> distance per step is 0.8/(360/1.8)
float volumePerStep = volumePerMM*distPerStep;   
float flowRate;
float delayBetwSteps;

//Button control variables
bool upButtonPressed = false;
bool downButtonPressed = false;

//Data transmission variables
int data = 0;
byte pumpAddress = 9;

void setup() 
{ 
  //Set up pins
  pinMode(stepPin, OUTPUT);
  pinMode(dirPin, OUTPUT);
  pinMode (upButtonPin, INPUT);
  pinMode (downButtonPin, INPUT);
  
  //Start I2C communication - set pump address in address variables
  Wire.begin(pumpAddress);
  
  //Initialize event for processing data from Control unit
  Wire.onReceive(receiveEvent);
}

void loop() 
{ 
  //Default state of the pump - data is at 0 - allows manual control using the up and down buttons on the pump
  if(data == 0)
  {
    RunPumpManually();  
  }
  //When receive data from the control unit - this changes the variable data to the flow rate - disable manual control and run automatically
  else if(data >0)
  {
    RunPump(data);
  }
}

//Receive flow rate data from control unit
void receiveEvent(int nrOfBytes)
{
  //Read in data
  int x = Wire.read();
  data = x;
}

//Function that runs the pump under control of the control unit using the control unit data as input
void RunPump(int rate)
{
  //Set the pump flow rate to the received rate
  flowRate = float(rate);
  
  //Calculate the delay required between steps (dependent on the syringe size and motors - if you want to change these change the associated variables)
  delayBetwSteps = (volumePerStep/flowRate);
  delayBetwSteps = delayBetwSteps * 1000 * 60;
  
  //Run pump
  TurnStepper(false, 1, delayBetwSteps);     
}


//Uses two buttons connected to the Arduino to manually control the pump - use for filling and resetting pump
//Only works if the pump is not under control of the control unit (data = 0)
void RunPumpManually()
{
  while(data == 0)
  {
   //Read in data from the buttons 
    upButtonPressed = digitalRead(upButtonPin);
    downButtonPressed = digitalRead(downButtonPin);
  
    //While pressing the up button, move the syringe pump up
    if (upButtonPressed)
    {
      TurnStepper(true,10,3);
    }
   //While pressing the down button, move the syringe pump down 
    if (downButtonPressed)
    {
      TurnStepper(false,10,3);
    }
  }
}

//Controls the stepper motors (this function allows for multiple steps per function call)
void TurnStepper (bool motorDir, int steps, int interval)
{
  //Set the motor direction to CW or CCW (depending on the motor wiring)
  if(motorDir)
  {
    digitalWrite(dirPin,HIGH);
  }
  else
  {
    digitalWrite(dirPin,LOW);
  }
  
  //Run the motor for the defined number of steps
  for (int i = 0; i < steps; i++)
  {
    digitalWrite(stepPin,HIGH);
    digitalWrite(stepPin,LOW);
    delay(interval);
  }
}





