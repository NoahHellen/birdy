#include <Arduino.h>
#include <ArduinoSTL.h>
#include <LiquidCrystal.h>

#pragma once

inline LiquidCrystal lcd(2, 3, 4, 5, 6, 7);

// Constants
constexpr int IR_RECEIVER = A0;
constexpr unsigned long T_INTERVAL = 10000;
constexpr int NUM_READINGS = 10;
constexpr float BAUD_RATE = 1000;
constexpr int BITS_PER_MSG = 12;
constexpr int MAX_CHAR = 128;
constexpr int BIT_THRESHOLD = 1;
constexpr int NUMBER_IDS = 8;
constexpr const int IDS[NUMBER_IDS] = {0, 1, 2, 3, 4, 5, 6, 7};

// Variables
inline unsigned long prev_t = 0;
inline bool recording = false;
inline float base_volt = 0;


// Function declarations
int bits_to_int(bool bits[], int start, int length);
float update_base_volt(unsigned long curr_t, unsigned long &prev_t);
char decode_char(float base_volt);
int decode_id(float base_volt);
void update_message(int id, char character);
void eot(char character);

// Message matrix
inline int char_index[NUMBER_IDS] = {0};
inline String messages[NUMBER_IDS][MAX_CHAR];