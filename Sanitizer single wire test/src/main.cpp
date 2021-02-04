#include <Arduino.h>

int sig = 12;
unsigned long timestart;
unsigned long timepassed;

void setup()
{
  // put your setup code here, to run once:
  Serial.begin(9600);
  pinMode(sig, INPUT_PULLUP);
}

void loop()
{
  // put your main code here, to run repeatedly:
  int sigstate = digitalRead(sig);
  Serial.println(sigstate);
  timestart = millis();
  while (sigstate == 0)
  {
    sigstate = digitalRead(sig);
    Serial.println(sigstate);
    timepassed = millis() - timestart;
    Serial.println(timepassed);
    if (timepassed > 1000)
    {
      Serial.println("turning on LED");
      timepassed = 0;
      pinMode(sig, OUTPUT);
      digitalWrite(sig, HIGH);
      delay(1000);
      pinMode(sig, INPUT_PULLUP);
      while(sigstate == 0){
        sigstate = digitalRead(sig);
      }
    }
  }
  delay(500);
}