

#include "Arduino.h"


#include <Wire.h> // Enable this line if using Arduino Uno, Mega, etc.
#include <Adafruit_GFX.h>
#include "Adafruit_LEDBackpack.h"
#include <Key.h>
#include <Keypad.h>


Adafruit_7segment matrix = Adafruit_7segment();

const byte ROWS = 4; //four rows
const byte COLS = 3; //three columns
char keys[ROWS][COLS] = {
  {'1','2','3'},
  {'4','5','6'},
  {'7','8','9'},
  {'*','0','#'}
};
byte rowPins[ROWS] = {5, 6, 7, 8}; //connect to the row pinouts of the keypad
byte colPins[COLS] = {2, 3, 4}; //connect to the column pinouts of the keypad

Keypad keypad = Keypad( makeKeymap(keys), rowPins, colPins, ROWS, COLS );  


void quietBuzz();
void loudBuzz();
void countDown();
void displayUpdate();
void checkPassword();
void fail();



int keyDigit1 = 0;
int keyDigit2 = 0;
int keyDigit3 = 0;
int keyDigit4 = 0;
long Timer = 1000;
int WrongCounter=0;
int delayLength=100;
int wrongCounterTracker = 0;

int keyForChecking1 = 10;
int keyForChecking2 = 10;
int keyForChecking3 = 10;
int keyForChecking4 = 10;

bool keyDigit1Check = false;
bool keyDigit2Check = false;
bool keyDigit3Check = false;
bool keyDigit4Check = false;

bool passwordInput = false;
bool colon = true;
bool firstTimeThrough = true;
bool breakProgram = false;



void setup(){
  matrix.begin(0x70);
  Serial.begin(9600);
  
  
  matrix.print(10000, DEC);
  matrix.writeDisplay();

//Set the code
char keyDigit1Char = keypad.waitForKey();
keyDigit1 = keyDigit1Char - '0'; 
  quietBuzz();
  Serial.println(keyDigit1);
    matrix.writeDigitNum(0, keyDigit1, false);
      matrix.writeDisplay();
char keyDigit2Char = keypad.waitForKey();
keyDigit2 = keyDigit2Char - '0'; 
  quietBuzz();
   matrix.writeDigitNum(1, keyDigit2, false);
    matrix.writeDisplay();
char keyDigit3Char = keypad.waitForKey();
keyDigit3 = keyDigit3Char - '0'; 
  quietBuzz();
   matrix.writeDigitNum(3, keyDigit3, false);
    matrix.writeDisplay();
char keyDigit4Char = keypad.waitForKey();
keyDigit4 = keyDigit4Char - '0'; 
   quietBuzz();
    matrix.writeDigitNum(4, keyDigit4, false);
    matrix.writeDisplay();

  
  //Set Timer
  matrix.print(10000, DEC);
  matrix.drawColon(true);
  matrix.writeDisplay();
 char timerMinuteTensChar=keypad.waitForKey();
  //  Display the new passkey
 int timerMinuteTens = timerMinuteTensChar - '0'; 
  Serial.println(timerMinuteTens);
            matrix.writeDigitNum(0, timerMinuteTens, false); 
              quietBuzz();          
            matrix.writeDisplay(); 

char  timerMinuteOnesChar=keypad.waitForKey(); 
int timerMinuteOnes = timerMinuteOnesChar - '0'; 

            matrix.writeDigitNum(1, timerMinuteOnes, false);  
              quietBuzz();                   
            matrix.writeDisplay(); 

char timerSecondTensChar=keypad.waitForKey(); 
int timerSecondTens = timerSecondTensChar - '0';
            matrix.writeDigitNum(3, timerSecondTens, false);
              quietBuzz();          
            matrix.writeDisplay(); 

char timerSecondOnesChar=keypad.waitForKey();
int timerSecondOnes = timerSecondOnesChar - '0';
            matrix.writeDigitNum(4, timerSecondOnes, false);
              quietBuzz();                      
            matrix.writeDisplay(); 

Timer=(timerMinuteTens*10*60)+
        (timerMinuteOnes*60)+
        (timerSecondTens*10)+
        timerSecondOnes;
delay(500);
  
keypad.addEventListener(keypadEvent); // Add an event listener for this keypad  
Serial.print("Password is ");
Serial.print(keyDigit1);
Serial.print(keyDigit2);
Serial.print(keyDigit3);
Serial.print(keyDigit4);
}

void loop(){
  if (breakProgram){
  return;}
delay (delayLength);
  keypad.getKey();
delay (delayLength);
keypad.getKey();
delay (delayLength);
keypad.getKey();
//***************************************************************************************************
//output the timer on the display 
 
 // print a floating point 
//  matrix.print(1234);
//  matrix.drawColon(true);
//  matrix.writeDisplay();
  //delay(500);
  //  // method #2 - draw each digit
//  uint16_t blinkcounter = 0;
//  boolean drawDots = false;
//  for (uint16_t counter = 0; counter < 9999; counter ++) {
//    matrix.writeDigitNum(0, (counter / 1000), drawDots);
//    matrix.writeDigitNum(1, (counter / 100) % 10, drawDots);
//    matrix.drawColon(drawDots);
//    matrix.writeDigitNum(3, (counter / 10) % 10, drawDots);
//    matrix.writeDigitNum(4, counter % 10, drawDots);

//output the timer to the serial monitor for testing
//Serial.println(Timer);
//****************************************************************************************************

//***************************************************************************************************
if (WrongCounter>0 || Timer<120)
loudBuzz();
//else quietBuzz();
delay (delayLength);
keypad.getKey();

delay (delayLength);
keypad.getKey();
delay (delayLength);
keypad.getKey();
delay (delayLength);
keypad.getKey();
countDown();
delay (delayLength);
keypad.getKey();
displayUpdate();
keypad.getKey();
delay (delayLength);
if (Timer<=0)
fail();
keypad.getKey();
}//endOfLoop



//**********************Functions**********************************************************************************************
void quietBuzz(){
//  Serial.println("quiet Buzz");
  analogWrite (11,2);
  delay (20);
  analogWrite (11,0);
  return;
}


void loudBuzz(){
    Serial.println("Loud Buzz");
  analogWrite (11,255);
  delay (20);
  analogWrite (11,0);
  return;
}

void countDown(){
//    Serial.println("Countdown");
//  Serial.print("Timer ");
//Serial.println(Timer);
Serial.print ("Wrong Counter ");
Serial.println(WrongCounter);
  Timer = Timer-1;
if (WrongCounter==0)
  quietBuzz();

if (wrongCounterTracker != WrongCounter){
wrongCounterTracker = WrongCounter;  
if (WrongCounter<5)
  delayLength = delayLength/2;}

  }


void keypadEvent(KeypadEvent key){
    switch (keypad.getState()){
    case RELEASED:
if(firstTimeThrough){
  firstTimeThrough=false;  
char throwAwayKey = key;
}  
else if (key == '*' || key == '#')
char throwAwayKey = key;
//Serial.println("Keypad Event");
else if (keyForChecking1==10){
char keyForChecking1Char = key;
keyForChecking1 = keyForChecking1Char - '0'; 
Serial.print("Digit 1 is ");
Serial.print(keyForChecking1Char);
Serial.print("      ");
Serial.println(keyForChecking1);
}
else if  (keyForChecking2==10){
char  keyForChecking2Char = key;
keyForChecking2 = keyForChecking2Char - '0'; 
Serial.print("Digit 2 is ");
Serial.println(keyForChecking2);
}
else if  (keyForChecking3==10){
char  keyForChecking3Char = key;
  keyForChecking3 = keyForChecking3Char - '0'; 
Serial.print("Digit 3 is ");
Serial.println(keyForChecking3);
}
else if  (keyForChecking4==10){
char  keyForChecking4Char = key;
  keyForChecking4 = keyForChecking4Char - '0'; 
  Serial.print("Digit 4 is ");
Serial.println(keyForChecking4);
}

if (keyForChecking4!=10){
  checkPassword();
  Serial.println("Resetting check Keys");
  keyForChecking1 = 10;
  keyForChecking2 = 10;
  keyForChecking3 = 10;
  keyForChecking4 = 10;
  }
}              
}

void displayUpdate(){
//  Serial.println("DisplayUpdate ");
 int tensOfMinutes= Timer/60/10;
 int minutes = (Timer/60)%10;
 int tensOfSeconds = (Timer%60)/10;
 int seconds = (Timer%60)%10;
// Serial.print(tensOfMinutes);
// Serial.print(minutes);
// Serial.print(":");
// Serial.print(tensOfSeconds);
// Serial.println(seconds);
if(Timer>0){

colon=!colon;

matrix.drawColon(colon);
matrix.writeDigitNum(0, tensOfMinutes);
matrix.writeDigitNum(1, minutes);
matrix.writeDigitNum(3, tensOfSeconds);
matrix.writeDigitNum(4, seconds);
matrix.writeDisplay();
}
}


void checkPassword(){
  Serial.println("Check Password");
if(keyForChecking1==keyDigit1){
keyDigit1Check = true;
Serial.println("Digit 1 is correct");}
if(keyForChecking2==keyDigit2){
keyDigit2Check = true;
Serial.println("Digit 2 is correct");}
if(keyForChecking3==keyDigit3){
keyDigit3Check = true;
Serial.println("Digit 3 is correct");}
if(keyForChecking4==keyDigit4){
keyDigit4Check = true;
Serial.println("Digit 4 is correct");}

if (keyDigit1Check && keyDigit2Check && keyDigit3Check && keyDigit4Check){
  matrix.drawColon(colon);
  matrix.writeDisplay();
  Serial.println("Password Correct");
  breakProgram=true;
  }
else { 
passwordInput = false;
 keyDigit1Check = false;
 keyDigit2Check = false;
 keyDigit3Check = false;
 keyDigit4Check = false;
 WrongCounter++;
}
  }

void fail(){
  while (1){Serial.println("Fail");
  analogWrite(11,255);}
}


