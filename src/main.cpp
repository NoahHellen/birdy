#include "utils.h"
#include <Arduino.h>
using namespace birdy;

unsigned long previous_time = 0;

void setup() {
  Serial.begin(9600);
  Serial.println("Initialising birdy!");
  pinMode(kIrReceiver, INPUT);
  LiquidCrystal lcd(2, 3, 4, 5, 6, 7);
  lcd.begin(16, 2);
}

void loop() {
  unsigned long current_time = millis();
  float base_volt = UpdateBaseVolt(current_time, previous_time);
  if (base_volt > 0) {
    Serial.print("Baseline: ");
    Serial.println(base_volt);
  }
}
