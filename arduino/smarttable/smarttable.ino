/****************************************
Sound Level Sketch for the 
Adafruit Microphone Amplifier
+
StepperMotor
+
ThingSpeak Client
****************************************/
#include <Bridge.h>
/**************************************
 * build the internet
****************************************/
#include <HttpClient.h>
#include <ThingSpeak.h>
/**************************************
 * build the file
****************************************/
#include <FileIO.h>
const int sampleWindow = 50; // Sample window width in mS (50 mS = 20Hz)
unsigned int sample0;
unsigned int sample1; 
int pin0 = 3; // set up the pin 0
int pin1 = 4; // set up the pin 1


int counterA = 0; //set up the counter for the talking duration for A
int counterB = 0; // set up the counter for the talking duration for B
int counterSilence = 0; //set up the counter for no one
String flag = "no"; //set up the flag that the motor works
String talkingRole = "s"; //set the talking role default as null
String dominance = "S";


int movementLevel = 10;//control how obvious the movement is 
int voiceDifference = 20; // control how loud is one dominance

/*thingSpeak------------------------------------------*/
const unsigned long postingInterval = 500;  //delay between updates to ThingSpeak
String APIkey ="28QJR1OBAO9J79V0";              // Enter YOUR key here
uint32_t data = 0;                       // The data is a simple number
/*thingSpeak------------------------------------------*/


void setup() 
{
  // Initialize the Bridge and the Serial
  Bridge.begin();
  Serial.begin(9600); 
  //establish the access to SD card
//  FileSystem.begin();
  

//  while (!SerialUSB); // wait for Serial port to connect.
//  SerialUSB.println("Filesystem datalogger\n");
  int cnt = 0;
  while (!Serial && cnt++ < 10)
  {
    delay(1000);
  }; 
  //establish motor direction toggle pins
  pinMode(12, OUTPUT); //CH A -- HIGH = forwards and LOW = backwards???
  pinMode(13, OUTPUT); //CH B -- HIGH = forwards and LOW = backwards???
  
  //establish motor brake pins
  pinMode(9, OUTPUT); //brake (disable) CH A
  pinMode(8, OUTPUT); //brake (disable) CH B

//  adjust();
  // Wait for Linux to boot
//  Serial.println("Waiting 10 seconds for Linux to boot ...");
//  delay(10000);
  
  // Bridge startup
//  Bridge.begin();
}

void adjust(){
  for(int i=0; i <10; i++){
    directionB();
  }
}
 
void loop() 
{
  //read the difference from two microphones
  double microDifference = readSensorValues();
//  directionA();
//  directionB();
      Serial.println("dominant role: " + flag);
      //if A is talking too much or too loud
  if (dominanceDetect(microDifference) == "A" ){
    //rotate to drive the A side up
    if(flag != "A"){//if the motor has not rotated to A yet
      for(int i = 0; i<movementLevel; i++){
        directionA();
      }
      flag = "A";
    }
  }else if ( dominanceDetect(microDifference) == "B") {
    //rotate to drive the B side up
    if(flag != "B"){
      for(int i = 0; i<movementLevel; i++){
      directionB();
      }
      flag = "B";
    }
  }else if (dominanceDetect(microDifference) == "S"){
    //if the motor has rotated to A then rotate back;
    if(flag == "A"){
      for(int i = 0; i<movementLevel; i++){
      directionB();
      }
       Serial.println("turn it back to balance from A");
    }else if(flag == "B"){
      for(int i = 0; i<movementLevel; i++){
      directionA();
      }
      
      Serial.println("turn it back to balance from A");
    }
    flag="no";
  }
  
  // Pause
  delay(postingInterval);
}

String dominanceDetect(double microDifference){
  //when the value <-1, it's A talking
  if (microDifference < -1.0) {
    //if A has already been talking then add the talking time
    if (talkingRole == "A") {
       counterA++;
       Serial.println("A is talking");
       //reset the other two counters
       counterB = 0;
       counterSilence = 0;
    }
    //set the talking person as A
    talkingRole = "A";
  }else if (microDifference >1.0) {
    //if B has already been talking then add the talking time
    //and clear out the A value
    if(talkingRole == "B"){
       counterB++;
       Serial.println("B is talking");
       //reset the other two counters
       counterA = 0;
       counterSilence = 0;
    }
    talkingRole = "B";
  }else {
    /*if the silence is more than 5, then reset the roles*/
    counterSilence ++;
    talkingRole = "S";
    Serial.println("counterA:"+String(counterA)+"counterB:"+String(counterB)+"counterS:"+String(counterSilence)+" no one is talking");
    if ((counterA<5 && counterB <5) || counterSilence > 12) {
      counterA = 0;
      counterB = 0;
      dominance = "S";
    }
  }

  //check who's talking too much, after continue talking more than 20s the table starts
  if (counterA > 8 || microDifference < -voiceDifference){
    Serial.println("counterA:"+String(counterA)+"counterB:"+String(counterB)+"counterS:"+String(counterSilence)+" dominanceA");
    dominance = "A";
  }
  if (counterB > 8  || microDifference > voiceDifference){
    //rotate to drive the B side up
    dominance = "B";
    
    Serial.println("counterA:"+String(counterA)+"counterB:"+String(counterB)+"counterS:"+String(counterSilence)+" dominanceA");
  }

  return dominance;
//  sendDataToThingSpeak(String(data));
}

//read sensor values and process them
double readSensorValues(){
   unsigned long startMillis= millis();  // Start of sample window
   unsigned int peakToPeak = 0;   // peak-to-peak level

    // init
   unsigned int signalMax0 = 0;
   unsigned int signalMin0 = 1024;
   unsigned int signalMax1 = 0;
   unsigned int signalMin1 = 1024;
 
   // collect data for 50 mS
   while (millis() - startMillis < sampleWindow)
   {
      sample0 = analogRead(pin0);
      sample1 = analogRead(pin1);

      if (sample0 < 1024)  // toss out spurious readings
      {
         if (sample0 > signalMax0)
         {
            signalMax0 = sample0;  // save just the max levels
         }
         else if (sample0 < signalMin0)
         {
            signalMin0 = sample0;  // save just the min levels
         }
      }
      if (sample1 < 1024)  // toss out spurious readings
      {
         if (sample1 > signalMax1)
         {
            signalMax1 = sample1;  // save just the max levels
         }
         else if (sample1 < signalMin1)
         {
            signalMin1 = sample1;  // save just the min levels
         }
      }
   }
  //process the peak-peak signals
   double volt0 = process(signalMax0, signalMin0);
   double volt1 = process(signalMax1, signalMin1);
   Serial.print("1:");
   Serial.println(volt0);
   Serial.print("2:");
   Serial.println(volt1);
   double difference = volt0 - volt1;
  // sendDataToSD(volt0, volt1);
//   Serial.println(difference);  
   return difference;
}

//find the voltage fluctuation
double process(int signalMax, int signalMin){
  int peakToPeak = signalMax - signalMin; // max - min = peak-peak amplitude
  double volts = (peakToPeak * 5.0) / 1024 * 100; // convert to volts
  return volts;
}

