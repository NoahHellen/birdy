#include <Arduino.h>
#include <Vector.h>

#pragma once

namespace bird {

// Button for bit one.
constexpr int kBitOnePin = 18;

// Button for bit zero.
constexpr int kBitZeroPin = 15;

// Piezo buzzer to signify end of each bit sequence.
constexpr int kPiezo = 2;

// LED pin send transmission via pulses after EOT.
constexpr int kLight = 17;

// Duration between each bit received.
constexpr int kBitRate = 500;

// Number of 12-bit sequences that can be sent.
constexpr int kMaxSequences = 500;

// Debounce delay so one button click not registered as two.
constexpr int kDebounceDelay = 50;

// Function declarations.
bool DetectedEOT(const bool bit_sequence[12]);

} // namespace bird
