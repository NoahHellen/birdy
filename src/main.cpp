#include <Arduino.h>
#include <LiquidCrystal.h>

// IR receiver
const int sensor_pin = A0;

// LCD object
LiquidCrystal lcd(2, 3, 4, 5, 6, 7);

// Baseline voltage
unsigned long t_interval = 10000;
unsigned long prev_t = 0;
float base_volt = 0;
const int num_readings = 10;

// Bit threshold
int threshold = 1;

// IR signal recieve rate
float receive_rate = 1000;

// Message settings
const int bits_per_msg = 12;
bool bits[bits_per_msg];
int bit_index = 0;
bool recording = false;
bool message_started = false;

// Message storage
String msgs_by_id[8];
bool has_msg[8] = {false};
int curr_id = -1;

// Convert bit array to int
int bits_to_int(bool bits[], int start, int length) {
  int value = 0;
  for (int i = 0; i < length; i++) {
    value |= bits[start + i] << (length - 1 - i);
  }
  return value;
}

void setup() {
  Serial.begin(9600);
  pinMode(sensor_pin, INPUT);
  lcd.begin(16, 2);
}

void loop() {
  unsigned long curr_t = millis();

  // Only update baseline if not recording
  if (!recording && (curr_t - prev_t > t_interval || prev_t == 0)) {
    prev_t = curr_t;
    float total_voltage = 0;
    for (int i = 0; i < num_readings; i++) {
      total_voltage += (analogRead(sensor_pin) / 1023.0) * 5.0;
      delay(10);
    }
    base_volt = total_voltage / num_readings;
    Serial.print("Updated baseline voltage: ");
    Serial.println(base_volt);
  }

  // Read voltage
  int sensor_val = analogRead(sensor_pin);
  float volt = (sensor_val / 1023.0) * 5.0;

  // Start message if high bit detected
  if (!recording && volt > base_volt + threshold) {
    recording = true;
    message_started = true;
    bit_index = 0;
    curr_id = -1;
    Serial.println("Message started");
  }

  if (recording) {
    bool bit = (volt > base_volt + threshold) ? 1 : 0;
    bits[bit_index] = bit;
    bit_index++;

    Serial.print("Bit ");
    Serial.print(bit_index);
    Serial.print(": ");
    Serial.println(bit);

    delay(receive_rate);

    if (bit_index >= bits_per_msg) {
      bit_index = 0;

      int ascii_val = bits_to_int(bits, 1, 8);
      char decoded_char = static_cast<char>(ascii_val);
      int sender_id = bits_to_int(bits, 9, 3);

      if (curr_id == -1) {
        curr_id = sender_id;
      }

      Serial.print("Character: ");
      Serial.println(decoded_char);
      Serial.print("Sender ID: ");
      Serial.println(sender_id);

      if (ascii_val == 4) { // EOT
        Serial.println("End of Transmission (EOT) detected.");

        Serial.println("----- All Messages -----");
        for (int i = 0; i < 8; i++) {
          if (has_msg[i]) {
            Serial.print("ID ");
            Serial.print(i);
            Serial.print(": ");
            Serial.println(msgs_by_id[i]);
          }
        }

        for (int i = 0; i < 8; i++) {
          if (has_msg[i]) {
            lcd.clear();
            lcd.setCursor(0, 0);
            lcd.print("From ");
            lcd.print(i);

            String msg = msgs_by_id[i];
            int len = msg.length();

            if (len <= 16) {
              lcd.setCursor(0, 1);
              lcd.print(msg);
              delay(3000);
            } else {
              for (int j = 0; j <= len - 16; j++) {
                lcd.setCursor(0, 1);
                lcd.print(msg.substring(j, j + 16));
                delay(400);
              }
              delay(1000);
            }
          }
        }

        recording = false;
        message_started = false;
        curr_id = -1;
      } else {
        msgs_by_id[sender_id] += decoded_char;
        has_msg[sender_id] = true;
      }
    }
  }
}
