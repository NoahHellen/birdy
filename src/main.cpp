#include "utils.h"
#include <Arduino.h>
using namespace birdy;

unsigned long previous_time = 0;
float base_voltage = ComputeVoltage();
bool recording = false;
LiquidCrystal lcd(2, 3, 4, 5, 6, 7);
int character_index[kNumberOfUsers] = {0};
char message_storage[kNumberOfUsers][kMaxCharacters] = {{0}};

void setup() {
  Serial.begin(9600);
  Serial.println("Initialising birdy!");
  pinMode(kIrReceiver, INPUT);
  lcd.begin(16, 2);
}

void loop() {
  unsigned long current_time = millis();
  UpdateBaseVoltage(current_time, previous_time, base_voltage);
  float current_voltage = ComputeVoltage();
  Serial.print("Current volage: ");
  Serial.println(current_voltage);
  RecordingStarted(recording, current_voltage, base_voltage);
  if (recording) {
    DecodedSequence ds = DecodeSequence(current_voltage, base_voltage);
    Serial.print("New character");
    Serial.println(ds.character);
    Serial.println("ID: ");
    Serial.println(ds.sender_id);
    StoreDecodedSequence(ds.sender_id, ds.character, character_index,
                         message_storage);
    EndOfTransmission(ds.character, character_index, recording, lcd,
                      message_storage);
  }
}
