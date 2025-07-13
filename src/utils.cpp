namespace birdy { 
namespace {

// Converts a range of bits in an array to an integer.
// The conversion starts at index `start` and uses `length` bits.
int BitsToInt(bool bits[], int start, int length) {
  int integer = 0;
  for (int i = 0; i < length; i++) {
    integer |= bits[start + i] << (length - 1 - i);
  }
  return integer;
}
} // namespace
} // namesapce birdy