#include "utils.h"
#include <Arduino.h>

namespace birdy {

// Converts a range of bits in an array to an integer.
// The conversion starts at index `start` and uses `length` bits.
int BitsToInt(bool bits[], int start, int length) {
  int integer = 0;
  for (int i = 0; i < length; i++) {
    integer |= bits[start + i] << (length - 1 - i);
  }
  return integer;
}

// Returns the average voltage of infrared sensor every ten seconds.
// The `current_time` compares itself to `previous_time`, the time of
// last update, to determine when update should trigger.
float UpdateBaseVolt(unsigned long current_time, unsigned long &previous_time) {
  float avg_volt = -1;
  if (current_time - previous_time > kTimeInterval) {
    previous_time = current_time;
    float voltage_sum = 0;
    for (int i = 0; i < kNumReadings; i++) {
      voltage_sum += (analogRead(kIrReceiver) / 1023.0) * 5.0;
      delay(10);
    }
    avg_volt = voltage_sum / kNumReadings;
  }
  return avg_volt;
}
} // namespace birdy
