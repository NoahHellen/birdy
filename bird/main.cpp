#include "utils.h"
#include <Arduino.h>
#include <Vector.h>

using namespace bird;

// Store message in nested array of 12-bit sequences.
bool message[kMaxSequences][12];

// Keep track of number of 12-bit sequences.
int sequence_count = 0;

// Initialises hardware.
void setup() {
  Serial.begin(115200);
  pinMode(kBitOnePin, INPUT);
  pinMode(kBitZeroPin, INPUT);
  pinMode(kPiezo, OUTPUT);
  pinMode(kLight, OUTPUT);
}

// Store 12-bit sequences in message array and transmit if EOT.
void loop() {
  bool bit_sequence[12] = {0};

  // Store each button press in array above.
  for (int i = 0; i < 12; i++) {

    bool bit_recorded = false;

    while (!bit_recorded) {
      bool bit_one = digitalRead(kBitOnePin);
      bool bit_zero = digitalRead(kBitZeroPin);

      if (bit_one == HIGH) {
        bit_sequence[i] = true;
        Serial.print(1);
        bit_recorded = true;
        delay(kDebounceDelay);
        while (digitalRead(kBitOnePin) == HIGH)
          ;
      } else if (bit_zero == HIGH) {
        bit_sequence[i] = false;
        Serial.print(0);
        bit_recorded = true;
        delay(kDebounceDelay);
        while (digitalRead(kBitZeroPin) == HIGH)
          ;
      }
      delay(1);
    }
  }

  // Piezo buzz to indicate 12-bit sequence complete.
  tone(kPiezo, 1000, 10);

  // Store 12-bit sequence in message array.
  if (sequence_count < kMaxSequences) {
    for (int i = 0; i < 12; i++) {
      message[sequence_count][i] = bit_sequence[i];
    }
    sequence_count++;
  }

  // Send message once EOT.
  if (DetectedEOT(bit_sequence)) {
    Serial.println("EOT detected");
    for (int seq = 0; seq < sequence_count; seq++) {
      for (int bit = 0; bit < 12; bit++) {
        digitalWrite(kLight, message[seq][bit] ? HIGH : LOW);
        delay(kBitRate);
      }
    }
  }
  digitalWrite(kLight, LOW);
}
