#pragma once
#include "ASTexpressions.h"
#include <string>
/// a function parameter, holds the name (variable) and the modifiers of it
struct Parameter {
  Variable var;
  int32_t mods = 0;
};
