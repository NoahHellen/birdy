#include <Arduino.h>
#include "config.h"

void setup() {
  Serial.begin(9600);
  pinMode(IR_RECEIVER, INPUT);
  lcd.begin(16, 2);
  Serial.println("Starting IR Receiver...");
}

void loop() {
  unsigned long curr_t = millis();
  float base_volt = update_base_volt(curr_t, prev_t);
  Serial.print("Updated baseline voltage: ");
  Serial.println(base_volt);

  // Read voltage
  int sensor_val = analogRead(IR_RECEIVER);
  float volt = (sensor_val / 1023.0) * 5.0;

  // Start message if high bit detected
  if (!recording && volt > base_volt + BIT_THRESHOLD) {
    recording = true;
  }

  if (recording) {
    int id = decode_id(base_volt);
    char character = decode_char(base_volt);
    Serial.print("Character: ");
    Serial.println(character);
    Serial.print("Sender ID: ");
    Serial.println(id);
    update_message(id, character);
    eot(character);
  }
}
