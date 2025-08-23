#include "utils.h"
#include <Arduino.h>
using namespace nest;

// Timestamp of last base voltage update.
unsigned long previous_time = 0;

// Initial base voltage reading.
float base_voltage = ComputeVoltage();

// True when message decoding should begin.s
bool recording = false;

// Initialise LCD display.
LiquidCrystal lcd(2, 3, 4, 5, 6, 7);

// Index of next character to store for each user.
int character_index[kNumberOfUsers] = {0};

// Matrix containing message for each user.
char message_storage[kNumberOfUsers][kMaxCharacters] = {{0}};

// Initialises hardware.
void setup() {
  Serial.begin(9600);
  pinMode(kIrReceiver, INPUT);
  lcd.begin(16, 2);
}

// Decodes transmission and stores in message matrix.
void loop() {
  unsigned long current_time = millis();
  if (!recording) {
    UpdateBaseVoltage(current_time, previous_time, base_voltage);
  }
  float current_voltage = ComputeVoltage();
  RecordingStarted(recording, current_voltage, base_voltage);
  if (recording) {
    DecodedSequence ds = DecodeSequence(current_voltage, base_voltage);
    StoreDecodedSequence(ds.sender_id, ds.character, character_index,
                         message_storage);
    EndOfTransmission(ds.character, character_index, recording, lcd,
                      message_storage);
  }
}
