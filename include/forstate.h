#include "runtime_value.h"
/// for state, used to store the information about the for loop
struct ForState {
  RuntimeValue end;
  RuntimeValue start;
  uint32_t iterator;
  char8_t direction = 2;
  bool inclusive;
};
