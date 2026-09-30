#pragma once
#include "action.h"
#include "function_info.h"
#include "location.h"
#include <runtime_value.h>
#include <vector>

struct Instruction {
  Action action;
  uint32_t operand = 0;
  inline Instruction(const Action &action, const uint32_t &operand = 0)
      : action(action), operand(operand) {};
};

struct Bytecode {
  std::vector<Instruction> code = {};
  std::vector<RuntimeValue> values = {};
  std::vector<FunctionInfo> functionsinfo = {};
  std::vector<Location> locations = {};
};
