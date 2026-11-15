#pragma once

#include "JIT_value.h"
#include "jitforstate.h"
#include "runtime_value.h"
#include <cstddef>

/**
 * A structure that is passed to the compiled function when its called. It
 * stores the pointers too all essential structures and arrays which are needed
 * for the compiled function to properly work. The pointer to the jitframe is
 * stored in r12 register.
 */
struct JITFrame {
  RuntimeValue *return_value;
  JITvalue *envptr;
  JITvalue *stackptr;
  JITForState *forptr = nullptr;
  bool interpretatorcall = true;
};

static_assert(offsetof(JITFrame, return_value) == 0);
static_assert(offsetof(JITFrame, envptr) == 8);
static_assert(offsetof(JITFrame, stackptr) == 16);
static_assert(offsetof(JITFrame, forptr) == 24);
static_assert(offsetof(JITFrame, interpretatorcall) == 32);
