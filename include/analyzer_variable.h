#pragma once
#include "location.h"
#include "semantic_error.h"
#include "slot_table.h"
#include <cstdint>
#include <vector>

/// analyzer variable, only stores the id without the value
struct AnalyzerVariable {
  uint32_t mods = MOD_NONE;
  uint32_t locked = 0;
  VariableID ID;
  /// locks the variable(no longer used)
  inline void lock() { locked++; }
  /// unlocks the variable (no longer used)
  inline void unlock() { locked--; }
  /// checks if the variable is allowed to change
  inline bool isallowed(const uint32_t &mods,
                        std::vector<SemanticError> &errors,
                        const Location &loc) {
    if (this->mods & MOD_CONST)
      errors.emplace_back("A constant variable (or function) cannot be changed",
                          loc);
    if (locked)
      errors.emplace_back("An iterator cannot be changed", loc);
    if (mods)
      errors.emplace_back("Modifiers can only be used with the first "
                          "declaration of the variable",
                          loc);
    return false;
  }
  /// main constructor
  AnalyzerVariable(const uint32_t &modifiers, const VariableID &ID)
      : mods(modifiers), ID(ID) {};
};
