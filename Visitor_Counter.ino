/*
Change baud rate according to computer to allow proper communication
*/
#include <OLED_I2C.h>

OLED  myOLED(SDA, SCL, 8);

const int pirPin = 2;
const int LEDPin = 4;
const int buzPin = 6;

int pirState;       
int lastpirState = LOW;

unsigned long lastDebounceTime = 0;  // the last time the output pin was toggled
unsigned long debounceDelay = 10;

extern uint8_t BigNumbers[];

int value = 0;

void setup()
{
  myOLED.begin();
  myOLED.setFont(BigNumbers);
  pinMode(pirPin,INPUT_PULLUP);
  pinMode(LEDPin,OUTPUT);
  pinMode(buzPin,OUTPUT);
}

void loop()
{
  increaseParameter();
  myOLED.printNumI(value,CENTER,24);
  myOLED.update();
  myOLED.clrScr();
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
        digitalWrite(LEDPin,HIGH);
        digitalWrite(buzPin,HIGH);
        delay(2000);
        digitalWrite(LEDPin,LOW);
        digitalWrite(buzPin,LOW);
      }
    }
  }
  lastpirState = reading;
}
