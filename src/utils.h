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

// Function declarations.
float UpdateBaseVolt(unsigned long current_time, unsigned long &previous_time);

// const char *arr[2][2] = {{"hello", "again"}, {"hello", "again"}};
} // namespace birdy
