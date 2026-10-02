#pragma once
#include "function_info.h"
#include "runtime_value.h"
#include "runtime_variable.h"
struct Environment {
  std::vector<RuntimeVariable> locals = {};
  std::vector<FunctionInfo> functions = {};
  inline static std::vector<RuntimeVariable> globals = {};
  Environment(const uint32_t &size, const size_t &funcsize);
  RuntimeVariable get(const uint32_t &index);
  RuntimeVariable *getPointer(const uint32_t &index);
  void set(const uint32_t &index, const RuntimeValue &value,
           const int32_t &mods);
  bool newGlobal(const uint32_t &index, const RuntimeValue &value);
};
