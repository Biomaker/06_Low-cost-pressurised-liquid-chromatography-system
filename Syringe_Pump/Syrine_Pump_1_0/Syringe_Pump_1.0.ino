//#include <Event.h>
//#include <Timer.h>

/*Sketch to run a syringe pump manually using two buttons, an up- and a down button. Speed cannot be set at present.
 * Future versions will allow the speed to be set either using in-built LCD and Raspberry Pi control unit.
 * For now, all units are for a standard 50 ml syringe.
*/

//Motor control variables
int stepPin = 3;
int dirPin = 6;
int upButtonPin = 12;
int downButtonPin = 11;

int steps;
int stepInterval = 10; //Milliseconds between steps

//Syringe and pump variables
float distPerStep = 0.004;  //Pitch of an M5 threaded rod 0.8 mm - 0.8 mm/360 degrees. The motor used does 1.8 deg/step -> distance per step is 0.8/(360/1.8)
float volumePerStep = 1.8/0.004;   //60 ml syringe has 1.8 mm/ml, volume per step is in ml
float flowRate;
float runVolume;

bool flowOn = false;

//Button control variables
bool upButtonPressed = false;
bool downButtonPressed = false;

String menuChoice = "";

void setup() 
{
  // Start a serial connection
  Serial.begin(9600);
  Serial.println("Initialize...");

  //Set up pins
  pinMode(stepPin, OUTPUT);
  pinMode(dirPin, OUTPUT);
  pinMode (upButtonPin, INPUT);
  pinMode (downButtonPin, INPUT);

  Serial.println("Initializing...");

}

void loop() 
{    
    Serial.println("Enter m for manual run mode, s for system control.");

    while(!Serial.available()){} 
    menuChoice += char(Serial.read());
    if(menuChoice.indexOf("m") >0)
    { 
      flowOn = true;
      while(flowOn)
      {
        RunPumpManually();

        //Add code to allow escape
      }
      menuChoice = "";
    }
    else if(menuChoice.indexOf("s")>0)
    {
      Serial.println("Need to program this part");
      menuChoice ="";
    }
}


//Uses two buttons connected to the Arduino to manually control the pump
void RunPumpManually()
{
  upButtonPressed = digitalRead(upButtonPin);
  downButtonPressed = digitalRead(downButtonPin);
  
  if(upButtonPressed)
  {
    TurnStepper(false,10,3);
  } 
  if (downButtonPressed)
  {
    TurnStepper(true,10,3);
  }

}

//Controls the stepper motors (this function is for multiple steps)
void TurnStepper (bool motorDir, int steps, int interval)
{
  if(motorDir)
  {
    digitalWrite(dirPin,HIGH);
  }
  else
  {
    digitalWrite(dirPin,LOW);
  }
  
  for (int i = 0; i < steps; i++)
  {
    digitalWrite(stepPin,HIGH);
    digitalWrite(stepPin,LOW);
    delay(interval);
  }
}

//Steps the stepper motors by one step
void OneStep(bool motorDir)
{
  if(motorDir)
  {
    digitalWrite(dirPin,HIGH);
  }
  else
  {
    digitalWrite(dirPin,LOW);
  }

    digitalWrite(stepPin,HIGH);
    digitalWrite(stepPin,LOW);
}

//Function to receive a number input from the Serial port largely from https://startingelectronics.org/software/arduino/learn-to-program-course/19-serial-input/ - want to 
//tidy this function up a little - has been adjusted to handle floats instead of int and to also allow negative numbers. Need to add error handling for multiple - or multiple .
float ReceiveNumberFromSerial()
{
char rx_byte = 0;
String rx_str = "";
bool not_number = false;
float result;
bool isNegative = false;
 
while(!not_number)
{
  if (Serial.available()) 
  {    
    // is a character available?
    rx_byte = Serial.read();       // get the character
    
    if ((rx_byte >= '0') && (rx_byte <= '9')) 
    {
      rx_str += rx_byte;
    }
    else if(rx_byte == '.')
    {
      rx_str += rx_byte;
    }
    else if (rx_byte == '\n') 
    {
      // end of string
      if (not_number) 
      {
        Serial.println("Not a number");
      }
      else 
      {
        result = rx_str.toFloat();

        //Check if the number is negative
        if(isNegative)
        {
          result = -result;
          return result;
        }
        else
        {
          return result; 
        }
      }
      not_number = false;         // reset flag
      rx_str = "";                // clear the string for reuse
    }
    else 
    {
      // non-number character received
      not_number = true;    // flag a non-number
    }
  }
}

}





