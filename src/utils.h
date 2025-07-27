#include <Arduino.h>
#include <LiquidCrystal.h>

#pragma once

namespace birdy {

// Arduino analog pin for infrared receiver.
constexpr int kIrReceiver = A0;

// Time interval for updating baseline voltage.
constexpr int kTimeInterval = 10000;

// Sample size for baseline voltage average.
constexpr int kNumReadings = 10;

// Voltage margin added to baseline to reliably detect message start, filtering
// out noise.
constexpr float kVoltageMargin = 0.01f;

// Length of each bit sequence;
constexpr int kBitsPerMessage = 12;

// Duration between each bit received.
constexpr int kBitRate = 1000;

// Number of users of communication system.
constexpr int kNumberOfUsers = 8;

// Maximum number of characters stored per user in single transmission.
constexpr int kMaxCharacters = 128;

// Contains decoded character and sender ID of bit sequence.
// Example:
//
struct DecodedSequence {
  char character;
  int sender_id;
};

// Function declarations.
void UpdateBaseVoltage(unsigned long current_time, unsigned long &previous_time,
                       float &base_voltage);
float ComputeVoltage();
void RecordingStarted(bool &recording, float current_voltage,
                      float base_voltage);
DecodedSequence DecodeSequence(float current_voltage, float base_voltage);
void StoreDecodedSequence(int sender_id, char character, int *character_index,
                          char message_storage[kNumberOfUsers][kMaxCharacters]);
void EndOfTransmission(char character, int *character_index, bool &recording,
                       LiquidCrystal &lcd,
                       char message_storage[kNumberOfUsers][kMaxCharacters]);

// const char *arr[2][2] = {{"hello", "again"}, {"hello", "again"}};
} // namespace birdy
