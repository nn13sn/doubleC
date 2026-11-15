#pragma once
#include "function_info.h"
#include "runtime_value.h"
#include "runtime_variable.h"
struct Environment {
  std::vector<RuntimeVariable> locals = {};
  std::vector<FunctionInfo> functions = {};
  inline static std::vector<RuntimeVariable> globals = {};
  /**
   * constructor, resizes the slot array right away, so the pointers would not
   * be affected by vector allocations after that
   */
  Environment(const uint32_t &size, const size_t &funcsize);
  /**
   * getting the copy of the variable, it first needs to be converted into
   * value, as originally it is stored in bit representation.
   */
  RuntimeVariable get(const uint32_t &index);

  /// geting pointer to the variable, to the bit representation in reality.
  RuntimeVariable *getPointer(const uint32_t &index);
  /**
   * setting the value, it first needs to be converted into the bits and only
   * after it can be stored
   */
  void set(const uint32_t &index, const RuntimeValue &value,
           const int32_t &mods);

  /// adding new global variable, not used in current version.
  bool newGlobal(const uint32_t &index, const RuntimeValue &value);
};
