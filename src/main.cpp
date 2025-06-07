#include <Arduino.h>
#include <LiquidCrystal.h>

// IR receiver
const int sensor_pin = A0;

// LCD object
LiquidCrystal lcd(2, 3, 4, 5, 6, 7);

// Variables for baseline voltage update
unsigned long time_interval = 10000;
unsigned long previous_time = 0;
float baseline_voltage = 0;
const int num_readings = 10;

// Message buffer
const int bits_per_message = 12;
bool bits[bits_per_message];
int bit_index = 0;
bool recording = false;

// IDs
int 

// Function to convert bit sequence to ASCII character
char bits_to_char(bool bits[], int length) {
  char result = 0;
  for (int i = 0; i < length; i++) {
    result |= bits[i] << (length - 1 - i);
  }
  return result;
}

void setup(){
  // Set up connection to computer
  Serial.begin(9600);

  // IR receiver connection
  pinMode(sensor_pin, INPUT);
}


void loop(){
  // Update baseline voltage every 10 seconds
  unsigned long current_time = millis();
  if (current_time - previous_time > time_interval || previous_time == 0) {
    previous_time = current_time;
    float total_voltage = 0;
    for (int i = 0; i < num_readings; i++) {
      int sensor_val = analogRead(sensor_pin);
      float voltage_sample = (sensor_val / 1023.0) * 5.0;
      total_voltage += voltage_sample;
      delay(10);
    }
    baseline_voltage = total_voltage / num_readings;
    Serial.print("Updated baseline voltage: ");
    Serial.println(baseline_voltage);
  }

  // Read current voltage
  int sensor_val = analogRead(sensor_pin);
  float voltage = (sensor_val / 1023.0) * 5.0;

  if (!recording) {
    // Detect start of message (bit 1)
    if (voltage > baseline_voltage) {
      recording = true;
      bit_index = 0;
      Serial.println("Message started");
    }
  }

  if (recording) {
    // Read bit: 1 if voltage > baseline, else 0
    bool bit = (voltage > baseline_voltage) ? 1 : 0;

    // Store bit
    bits[bit_index] = bit;
    bit_index++;

    Serial.print("Bit ");
    Serial.print(bit_index);
    Serial.print(": ");
    Serial.println(bit);

    delay(200); // Small delay between bits to avoid reading too fast

    if (bit_index >= bits_per_message) {
      // Decode and print character
      char decoded_char = bits_to_char(bits, bits_per_message);
      Serial.print("Decoded char: ");
      Serial.println(decoded_char);

      // Reset for next message
      recording = false;
    }
  }
}

// void loop(){
//   // IR receiver value
//   int sensor_val = analogRead(sensor_pin);
//   float voltage = (sensor_val /1023.0) * 5.0;

//   // Update baseline voltage every 10 seconds
//   unsigned long current_time = millis();
//   float total_voltage = 0;
//   if (current_time - previous_time > time_interval || previous_time == 0) {
//     previous_time = current_time;
//     for (int i = 0; i < num_readings; i++) {
//       total_voltage += voltage;
//       delay(10);
//     }
//     baseline_voltage = total_voltage / num_readings;
//   }
  
//   // Receives first bit then records message
//   bool bit = 0;
//   if (voltage > baseline_voltage) {
//     bit = 1;
//     Serial.println(bit);
//   }
// }