/*
Change baud rate according to computer to allow proper communication
*/
#include <OLED_I2C.h>

OLED  myOLED(SDA, SCL, 8);
//the time we give the sensor to calibrate (10-60 secs according to the datasheet)
int calibrationTime = 1;       

//the time when the sensor outputs a low impulse
long unsigned int lowIn;        
 
//the amount of milliseconds the sensor has to be low
//before we assume all motion has stopped
long unsigned int pause = 5000; 
 
boolean lockLow = true;
boolean takeLowTime; 
 
const int pirPin = 2;    //the digital pin connected to the PIR sensor's output
int ledPin = 4; //LED notification
int buzPin = 6; //Sound notification

int pirState; //State of the sensor
int lastpirState = LOW; //Previous state of the sensor defined as low so that if state is same OLED won't display another

unsigned long lastDebounceTime = 0;  // the last time the output pin was toggled
unsigned long debounceDelay = 10;

extern uint8_t BigNumbers[];

int value = 0;


void setup(){
  Serial.begin(9600);
  pinMode(pirPin, INPUT);
  pinMode(ledPin, OUTPUT);
  pinMode(buzPin, OUTPUT);
  digitalWrite(pirPin, LOW);
  
  myOLED.begin();
  myOLED.setFont(BigNumbers);

  Serial.print("calibrating sensor ");
    for(int i = 0; i < calibrationTime; i++){
      Serial.print(".");
      delay(1000);
      }
    Serial.println(" done");
    Serial.println("SENSOR ACTIVE");
    delay(50);
  }

void loop(){
 
     if(digitalRead(pirPin) == HIGH){
       digitalWrite(ledPin, HIGH);   //the led visualizes the sensors output pin state
       digitalWrite(buzPin, HIGH);
       
       increaseParameter();
       myOLED.printNumI(value,CENTER,24);
       myOLED.update();
       myOLED.clrScr();
  
       if(lockLow){ 
         //makes sure we wait for a transition to LOW before any further output is made:
         lockLow = false;           
         Serial.println("---");
         Serial.print("motion detected at ");
         Serial.print(millis()/1000);
         Serial.println(" sec");
         delay(50);
         }        
         takeLowTime = true;
       }
 
     if(digitalRead(pirPin) == LOW){      
       digitalWrite(ledPin, LOW);  //the led visualizes the sensors output pin state
       digitalWrite(buzPin, LOW);

       if(takeLowTime){
        lowIn = millis();          //save the time of the transition from high to LOW
        takeLowTime = false;       //make sure this is only done at the start of a LOW phase
        }
       if(!lockLow && millis() - lowIn > pause){ 
           //makes sure this block of code is only executed again after
           //a new motion sequence has been detected
           lockLow = true;                       
           Serial.print("motion ended at ");      //output
           Serial.print((millis() - pause)/1000);
           Serial.println(" sec");
           delay(50);
           }
       }
  }

void increaseParameter()
{
  int reading = digitalRead(pirPin);


  if (reading != lastpirState) {
    lastDebounceTime = millis();
  }

  if ((millis() - lastDebounceTime) > debounceDelay) {

    if (reading != pirState) {
      pirState = reading;

      if (pirState == LOW) {
        value++;
      }
    }
  }
  lastpirState = reading;
}