#pragma once

#include "JIT_value.h"
#include <cstddef>
struct JITForState {
  JITvalue end;
  uint64_t iterator;
  uint8_t direction;
  bool inclusive;
  JITvalue start;
};

static_assert(offsetof(JITForState, end) == 0);
static_assert(offsetof(JITForState, iterator) == 16);
static_assert(offsetof(JITForState, direction) == 24);
static_assert(offsetof(JITForState, inclusive) == 25);
static_assert(offsetof(JITForState, start) == 32);

inline std::vector<JITForState> jitforstates;
