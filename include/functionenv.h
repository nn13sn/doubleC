#pragma once
#include "function_info.h"
#include <vector>
struct FunctionEnv {
  FunctionEnv() {};
  std::vector<FunctionInfo> functions = {};
};
