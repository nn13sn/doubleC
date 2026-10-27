#pragma once
#include "JITfunction.h"
#include "runtime_value.h"
#include <cstddef>
#include <cstdint>
struct FunctionInfo {
  uint32_t entry;
  size_t slot;
  size_t exit;
  JITfunction compversion = nullptr;
};
