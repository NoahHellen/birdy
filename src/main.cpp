#include <Arduino.h>
#include <LiquidCrystal.h>

// IR receiver
const int ir_pin = A0;

// LCD object
LiquidCrystal lcd(2, 3, 4, 5, 6, 7);

// Baseline voltage update variables
unsigned long time_interval = 10000;
unsigned long previous_time = 0;
float baseline_voltage = 0;
const int num_readings = 10;

// Packet length
const int bits_per_packet = 12;

// Bit sequence
bool bits[bits_per_packet];
int bit_index = 0;

// Bools for starting and ending message
bool recording = false;
bool message_started = false;
bool transmission_complete = false;

// Store messages per sender ID (IDs 0–7)
String messages_by_id[8];

// Current sender ID for the active message
int current_sender_id = -1;

// Bits to integer
int bits_to_int(bool bits[], int start, int length) {
  int value = 0;
  for (int i = 0; i < length; i++) {
    value |= bits[start + i] << (length - 1 - i);
  }
  return value;
}

void setup() {
  // Set up communication with computer
  Serial.begin(9600);

  // Analog reading of IR receiver
  pinMode(ir_pin, INPUT);

  // Initialise LCD display
  lcd.begin(16, 2);
}

void loop() {
  // Stop loop if EOT
  if (transmission_complete) return;

  // Track time for baseline voltage
  unsigned long current_time = millis();

  // Update baseline voltage ONLY if not recording
  if (!recording && (current_time - previous_time > time_interval || previous_time == 0)) {
    previous_time = current_time;
    float total_voltage = 0;
    for (int i = 0; i < num_readings; i++) {
      total_voltage += (analogRead(ir_pin) / 1023.0) * 5.0;
      delay(10);
    }
    baseline_voltage = total_voltage / num_readings;
    Serial.print("Updated baseline voltage: ");
    Serial.println(baseline_voltage);
  }

  // Read voltage
  int sensor_val = analogRead(ir_pin);
  float voltage = (sensor_val / 1023.0) * 5.0;

  // Start message if high bit detected
  if (!recording && voltage > baseline_voltage + 0.5) { // 0.5 is for threshold for bit 1 - change once know average IR sensor readings (e.g. if IR gives 10V and baseline is 1.5V, set 0.5 to 8.5 or thereabouts)
    recording = true;
    message_started = true;
    bit_index = 0;
    current_sender_id = -1;
    Serial.println("Message started");
  }

  if (recording) {
    bool bit = (voltage > baseline_voltage) ? 1 : 0;
    bits[bit_index] = bit;
    bit_index++;

    Serial.print("Bit ");
    Serial.print(bit_index);
    Serial.print(": ");
    Serial.println(bit);

    delay(1000);  // avoid reading too fast

    if (bit_index >= bits_per_packet) {
      bit_index = 0;

      int ascii_val = bits_to_int(bits, 1, 8);
      char decoded_char = static_cast<char>(ascii_val);
      int sender_id = bits_to_int(bits, 9, 3);

      if (current_sender_id == -1) {
        current_sender_id = sender_id;
      }

      Serial.print("Character: ");
      Serial.println(decoded_char);
      Serial.print("Sender ID: ");
      Serial.println(sender_id);

      if (ascii_val == 4) {  // EOT
        Serial.println("End of Transmission (EOT) detected.");
        Serial.print("Full Message from ID ");
        Serial.print(current_sender_id);
        Serial.print(": ");
        Serial.println(messages_by_id[current_sender_id]);

        lcd.clear();
        lcd.setCursor(0, 0);
        lcd.print("From ");
        lcd.print(current_sender_id);
        lcd.setCursor(0, 1);
        lcd.print(messages_by_id[current_sender_id]);

        // Stop further IR readings
        recording = false;
        message_started = false;
        transmission_complete = true;  // Set flag
      } else {
        messages_by_id[sender_id] += decoded_char;
      }
    }
  }
}