#pragma once
#include "action.h"
#include "function_info.h"
#include "location.h"
#include "runtime_value.h"
#include <vector>

/**
 * Instruction - main component of the bytecode, it holds the action and the
 * operand. Operand value shows a specific format, index or poiner that is
 * needed in VM to reference
 */
struct Instruction {
  Action action;
  uint32_t operand = 0;
  Instruction(const Action &action, const uint32_t &operand = 0)
      : action(action), operand(operand) {};
  Instruction() {};
};

/**
 * The bytecode structure. It primary holds all of the instructions, but also
 * stores constant values in a different array, and the index of these values is
 * the operand of the instructions like push. Functionsinfo array contains the
 * information about functions, and the index is the operand of callfunction
 * instruction. Locations array is the same size as a code array, so the index
 * of the instruction is also the index of it's location. It is essential for
 * printing runtime errors
 */
struct Bytecode {
  std::vector<Instruction> code = {};
  std::vector<RuntimeValue> values = {};
  std::vector<FunctionInfo> functionsinfo = {};
  std::vector<Location> locations = {};
};
