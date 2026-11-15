#pragma once

#include "JIT_value.h"
#include <cstddef>

/**
 * jitforstate, and analogy to the forstate in interpreter, used in compiled
 * functions to correctly operate the loops
 */
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

/// array that holds all the forstates
inline std::vector<JITForState> jitforstates;
