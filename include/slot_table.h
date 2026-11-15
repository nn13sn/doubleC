#pragma once

#include <cstdint>
#include <vector>

using VariableID = uint32_t;
using Slot = uint32_t;
/**
 * structure that holds the slots of the variables and functions. Analyzer
 * gives each variable a different slot and each variable can identify it's slot
 * by the id later in generator
 */
struct Slot_Table {
  uint32_t nextID = 0;
  uint32_t nextSlot = 0;
  uint32_t nextFunctionSlot = 0;
  std::vector<VariableID> IDs = {};
  std::vector<Slot> slots = {};
  std::vector<Slot> FunctionSlots = {};
  std::vector<uint32_t> CallSlots = {};
};
