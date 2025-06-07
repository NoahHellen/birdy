#include <Arduino.h>
#include <LiquidCrystal.h>

const int sensor_pin = A0;
LiquidCrystal lcd(2, 3, 4, 5, 6, 7);


void setup(){
  Serial.begin(9600);
  pinMode(sensor_pin, INPUT);
  lcd.print("Hello Ferros!");
}

void loop(){
  int sensor_val = analogRead(sensor_pin);
  float voltage = (sensor_val / 1023.0) * 5.0;
  Serial.print("Sensor Value: ");
  Serial.print(sensor_val);
  Serial.print(", Volts: ");
  Serial.println(voltage);
  delay(500);
}