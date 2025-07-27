#include "config.h"
#include <Arduino.h>
#include <ArduinoSTL.h>
#include <LiquidCrystal.h>

// Convert bits to integer
int bits_to_int(bool bits[], int start, int length) {
  int value = 0;
  for (int i = 0; i < length; i++) {
    value |= bits[start + i] << (length - 1 - i);
  }
  return value;
}

// Update baseline voltage if not recording
float update_base_volt(unsigned long curr_t, unsigned long &prev_t) {
  if (!recording && (curr_t - prev_t > T_INTERVAL || prev_t == 0)) {
    prev_t = curr_t;
    float total_voltage = 0;
    for (int i = 0; i < NUM_READINGS; i++) {
      total_voltage += (analogRead(IR_RECEIVER) / 1023.0) * 5.0;
      delay(10);
    }
    base_volt = total_voltage / NUM_READINGS;
  }
  return base_volt;
}

// Decode 12-tuple of bits
char decode_char(float base_volt) {
  bool bits[BITS_PER_MSG];
  int bit_index = 0;
  while (bit_index < BITS_PER_MSG) {
    int sensor_val = analogRead(IR_RECEIVER);
    float volt = (sensor_val / 1023.0) * 5.0;
    bool bit = (volt > base_volt + BIT_THRESHOLD) ? 1 : 0;
    bits[bit_index] = bit;
    bit_index++;
    Serial.print("Bit ");
    Serial.print(bit_index);
    Serial.print(": ");
    Serial.println(bit);
    delay(BAUD_RATE);
  }
  int ascii_val = bits_to_int(bits, 1, 8);
  char decoded_char = static_cast<char>(ascii_val);
  return decoded_char;
}

int decode_id(float base_volt) {
  bool bits[BITS_PER_MSG];
  int bit_index = 0;
  while (bit_index < BITS_PER_MSG) {
    int sensor_val = analogRead(IR_RECEIVER);
    float volt = (sensor_val / 1023.0) * 5.0;
    bool bit = (volt > base_volt + BIT_THRESHOLD) ? 1 : 0;
    bits[bit_index] = bit;
    bit_index++;
    Serial.print("Bit ");
    Serial.print(bit_index);
    Serial.print(": ");
    Serial.println(bit);
    delay(BAUD_RATE);
  }
  int sender_id = bits_to_int(bits, 9, 3);
  return sender_id;
}

void update_message(int id, char character) {
  if (char_index[id] < MAX_CHAR) {
    messages[id][char_index[id]] = String(character);
    char_index[id]++;
  }
}

void eot(char character) {
  if ((int)character == 4) { // EOT detected
    for (int i = 0; i < NUMBER_IDS; i++) {
      lcd.clear();
      lcd.setCursor(0, 0);
      lcd.print("From ");
      lcd.print(i);

      lcd.setCursor(0, 1);
      String msg_line = "";
      for (int j = 0; j < char_index[i]; j++) {
        msg_line += messages[i][j]; // concatenate characters from matrix
      }
      lcd.print(msg_line);

      delay(2000);
    }

    // Clear messages and reset indexes after printing
    for (int i = 0; i < NUMBER_IDS; i++) {
      for (int j = 0; j < MAX_CHAR; j++) {
        messages[i][j] = ""; // clear each character string
      }
      char_index[i] = 0; // reset index
    }
    recording = false; // stop recording after EOT
  }
}
