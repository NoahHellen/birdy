#include <Arduino.h>

const int optoPin = 12;

void setup(){
  pinMode(optoPin, OUTPUT);
}

void loop(){
  digitalWrite(optoPin, HIGH);
  delay(15);
  digitalWrite(optoPin, LOW);
  delay(1000);
}