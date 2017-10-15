#include <Wire.h>

int anInt = 1;
int sizeOfInt;

int steps;

void setup()
{
  Wire.begin();
  Serial.begin(9600);
  
  //To be able to calculate array sizes, calculate the size of an int using this compiler
  sizeOfInt = sizeof(anInt);
}

void loop()
{
  //Program the run using multiple arrays
  //This program runs:
  // 5 ml equilibration, pause to load sample, 5 ml wash, 5 ml elution - collects fractions during wash and elution
  float flow[4] = {1, 0, 1, 1};    //Set a flow rate for each step - 
  float flowVol[4] = {0.2, 0, 5, 5};
  int pump[4] = {1, 1, 1, 2};
  int frac[4] = {0, 0, 1, 1};
  float fracVol[4] = {0, 0, 0.5, 0.5};
  
  //Calculate the nr. of steps the program has
  int programSteps = sizeof(flow)/sizeOfInt;
  int currentStep = 0;
  int previousCurrentStep = 0;
  
  //Start the program
  long startTime = millis();
  long runTime;
  long stepTime;
  

    Serial.println("Starting");
    for (int i = 0; i < programSteps -1; i++)
    {
      currentStep = i;
      long stepStartTime = millis();
      bool stepFinished = false;
      bool sampleLoaded = false;
      
      while(!stepFinished)
      {
        //If using pumps
        if(flowVol[currentStep] != 0)
        {     
          float totalStepTime = flowVol[currentStep]/flow[currentStep];
          totalStepTime = totalStepTime * 60 * 1000;
          Serial.println(totalStepTime);
          
          //Send a message to the pumps to start flowing
          
          //Check if need to collect fractions
          
          //If yes, activate fration collector
          
          while(totalStepTime > stepTime)
          {  //Update the run timers
            runTime = millis() - startTime;
            stepTime = millis() - stepStartTime;
            Serial.println(stepTime);
            delay(1000);       
          }
          
          stepFinished = true;
        }
        
        //Special case - loading step - only happens if the flow rate is set to 0
        else if(flowVol[currentStep] == 0)
        {
          //Wait until sample loaded, wait for input from a button saying the sample has been loaded 
          while(!sampleLoaded)
          {
            //Update the run timers
            runTime = millis() - startTime;
            stepTime = millis() - stepStartTime;
            
            //Add code to check if button has been pressed that sample is loaded
            delay(2000);
            sampleLoaded = true;
          }
        }
        stepFinished = true;
      }
    }
  

}
