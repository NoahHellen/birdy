#include <Arduino.h>
#include <LiquidCrystal.h>

const int sensorPin = A0;
LiquidCrystal lcd(12, 11, 5, 4, 3, 2);

void setup() {
  Serial.begin(9600);
  pinMode(sensorPin, INPUT);
  lcd.begin(16, 2);
  lcd.print("Hello!");
  delay(2000); // Give time to read
  lcd.clear(); // Clear after welcome message
}

void loop() {
  int sensorVal = analogRead(sensorPin);
  float voltage = (sensorVal / 1023.0) * 5.0;

  Serial.print("Sensor Value: ");
  Serial.print(sensorVal);
  Serial.print(", Volts: ");
  Serial.println(voltage);

  lcd.setCursor(0, 0);
  lcd.print("Sensor: ");
  lcd.print(sensorVal);

  lcd.setCursor(0, 1);
  lcd.print("Volt: ");
  lcd.print(voltage, 2);  // Print voltage with 2 decimal places

  delay(1000);  // Delay before next reading
}