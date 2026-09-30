#pragma once
#include "function_info.h"
#include <vector>
struct FunctionEnv {
  FunctionEnv() {};
  size_t GeneralSize;
  std::vector<FunctionInfo> functions = {};
  std::vector<size_t> scopesizes = {};
  void EnterScope() { scopesizes.push_back(functions.size()); };
  void ExitScope() {
    functions.resize(scopesizes.back());
    functions.resize(GeneralSize);
    scopesizes.pop_back();
  }
};
