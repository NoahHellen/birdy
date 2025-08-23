#include "utils.h"
#include <Arduino.h>

namespace nest {

// Converts a range of bits in an array to an integer.
// The conversion starts at index `start` and uses `length` bits.
int BitsToInt(bool bits[], int start, int length) {
  int integer = 0;
  for (int i = 0; i < length; i++) {
    integer |= bits[start + i] << (length - 1 - i);
  }
  return integer;
}

// Computes the voltage of infrared receiver.
float ComputeVoltage() { return (analogRead(kIrReceiver) / 1023.0) * 5.0; }

// Updates the baseline voltage every ten seconds.
// The `current_time` and `previous_time` are tracked to trigger update.
void UpdateBaseVoltage(unsigned long current_time, unsigned long &previous_time,
                       float &base_voltage) {
  if (current_time - previous_time > kTimeInterval || previous_time == 0) {
    previous_time = current_time;
    float voltage_sum = 0;
    for (int i = 0; i < kNumReadings; i++) {
      voltage_sum += ComputeVoltage();
      delay(10);
    }
    base_voltage = voltage_sum / kNumReadings;
    Serial.print("Base voltage:");
    Serial.println(base_voltage);
  }
}

// Starts process of decoding message_per_id by changing `recording` to true
// if `current_voltage` breaks threshold.
void RecordingStarted(bool &recording, float current_voltage,
                      float base_voltage) {
  if (!recording && current_voltage > base_voltage + kVoltageMargin) {
    recording = true;
  }
}

// Decodes sequence of bits into ascii characters and sender id,
// with bit values determined by `current_voltage` crossing voltage
// bit threshold or not.
DecodedSequence DecodeSequence(float current_voltage, float base_voltage) {
  DecodedSequence ds;
  bool bits[kBitsPerMessage];
  int bit_index = 0;
  while (bit_index < kBitsPerMessage) {
    float current_voltage = ComputeVoltage();
    bool bit = (current_voltage > base_voltage + kVoltageMargin) ? 1 : 0;
    Serial.print("Bit: ");
    Serial.println(bit);
    bits[bit_index] = bit;
    bit_index++;
    delay(kBitRate);
  }
  ds.character = static_cast<char>(BitsToInt(bits, 1, 8));
  ds.sender_id = BitsToInt(bits, 9, 3);
  Serial.print("Character: ");
  Serial.println(ds.character);
  Serial.print("Sender: ");
  Serial.print(ds.sender_id);
  return ds;
}

// Stores decoded `character` in the relevant `sender_id` row in message
// storage matrix, with column dependent on number of characters already in row.
void StoreDecodedSequence(
    int sender_id, char character, int *character_index,
    char message_storage[kNumberOfUsers][kMaxCharacters]) {
  if (character_index[sender_id] < kMaxCharacters) {
    message_storage[sender_id][character_index[sender_id]] = character;
    character_index[sender_id]++;
  }
}

// Once end of transmission ascii `character` received, prints each row
// in message storage matrix to LCD screen.
void EndOfTransmission(char character, int *character_index, bool &recording,
                       LiquidCrystal &lcd,
                       char message_storage[kNumberOfUsers][kMaxCharacters]) {
  if ((int)character == 4) {
    Serial.println("EOT detected");
    for (int i = 0; i < kNumberOfUsers; i++) {
      lcd.clear();
      lcd.setCursor(0, 0);
      lcd.print("From ");
      lcd.print(i);
      lcd.setCursor(0, 1);
      String message_per_id = "";
      for (int j = 0; j < character_index[i]; j++) {
        message_per_id += message_storage[i][j];
      }
      lcd.print(message_per_id);
      delay(2000);
    }
    for (int i = 0; i < kNumberOfUsers; i++) {
      memset(message_storage[i], 0, kMaxCharacters);
      character_index[i] = 0;
    }
    recording = false;
  }
}
} // namespace nest
