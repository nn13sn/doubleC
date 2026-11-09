#pragma once

#include "JIT_value.h"
#include "runtime_value.h"
struct JITFrame {
  RuntimeValue *return_value;
  JITvalue *envptr;
  JITvalue *stackptr;
};
