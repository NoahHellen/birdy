#include "utils.h"
#include <Arduino.h>
using namespace birdy;

void setup() {
  Serial.begin(9600);
  Serial.println("Initialising birdy!");
  pinMode(kIrReceiver, INPUT);
  lcd.begin(16, 2);
}

void loop() { Serial.println(arr[1][1]); }
