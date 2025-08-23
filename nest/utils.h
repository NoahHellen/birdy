#include <Arduino.h>
#include <LiquidCrystal.h>

#pragma once

namespace nest {

// Arduino analog pin for infrared receiver.
constexpr int kIrReceiver = A0;

// Time interval for updating baseline voltage.
constexpr int kTimeInterval = 10000;

// Sample size for baseline voltage average.
constexpr int kNumReadings = 10;

// Voltage margin added to baseline to detect transmission start.
constexpr float kVoltageMargin = 0.10f;

// Length of each bit sequence;
constexpr int kBitsPerMessage = 12;

// Duration between each bit received - slight offset of 10ms from transmitter
// due to lag.
constexpr int kBitRate = 510;

// Number of users of communication system.
constexpr int kNumberOfUsers = 8;

// Maximum number of characters stored per user in single transmission.
constexpr int kMaxCharacters = 128;

// Contains decoded character and sender ID of bit sequence.
// Example:
// 		DecodedSequence ds;
// 		ds.character = 'a'
//		ds.sender_id = 1
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
} // namespace nest
