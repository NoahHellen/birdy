#include "utils.h"

namespace bird {
// Starts trasmission of light pulses once EOT detected
// in `bit_sequence`. First bit in sequence must be 1
// (hence [i+1]) since receiver only starts recording when
// a bit 1 is received.
bool DetectedEOT(const bool bit_sequence[12]) {
  const bool EOT[8] = {0, 0, 0, 0, 0, 1, 0, 0};

  for (int i = 0; i < 8; i++) {
    if (bit_sequence[i + 1] != EOT[i])
      return false;
  }
  return true;
}
} // namespace bird
