#include "runtime_value.h"
struct ForState {
  RuntimeValue end;
  uint32_t iterator;
  char8_t direction = 2;
  bool inclusive;
};
