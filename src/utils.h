#include <LiquidCrystal.h>

#pragma once

namespace birdy{

// Initialise lcd display with numbered Arduino digital pins.
LiquidCrystal lcd(2, 3, 4, 5, 6, 7);

// Arduino analog pin for infrared receiver.
constexpr int kIrReceiver = A0;



const char* arr[2][2] = {{"hello", "again"},{"hello", "again"}};
}
