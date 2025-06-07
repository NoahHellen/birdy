#include <Arduino.h>
#include <LiquidCrystal.h>

const int sensor_pin = A0;
LiquidCrystal lcd(2, 3, 4, 5, 6, 7);

char bits_to_char(bool bits[], int length) {
  char result = 0;
  for (int i = 0; i < length; i++) {
    result |= bits[i] << (length - 1 - i);
  }
  return result;
}



void setup(){
  Serial.begin(9600);
  pinMode(sensor_pin, INPUT);
  lcd.print("Hello Ferros!");
  bool bits[7] = {1, 0, 0, 0, 0, 0, 1};
  char c = bits_to_char(bits, 7);
  Serial.print("Char is: ");
  Serial.println(c);  
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