#pragma once

#include "JIT_value.h"
#include "jitforstate.h"
#include "runtime_value.h"
#include <cstddef>
struct JITFrame {
  RuntimeValue *return_value;
  JITvalue *envptr;
  JITvalue *stackptr;
  JITForState *forptr = nullptr;
  bool interpretatorcall = true;
};

inline JITFrame emptyframe{nullptr, nullptr, nullptr, nullptr, false};

static_assert(offsetof(JITFrame, return_value) == 0);
static_assert(offsetof(JITFrame, envptr) == 8);
static_assert(offsetof(JITFrame, stackptr) == 16);
static_assert(offsetof(JITFrame, forptr) == 24);
static_assert(offsetof(JITFrame, interpretatorcall) == 32);
