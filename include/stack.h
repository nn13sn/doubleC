#pragma once
#include "runtime_value.h"
#include "vm_error.h"
#include <vector>
class Stack {
private:
  std::vector<RuntimeValue> data = {};

public:
  inline void Push(RuntimeValue value) { data.push_back(std::move(value)); }
  inline RuntimeValue Pop() {
    if (data.size() == 0)
      throw VM_error("The operation cannot be completed : stack is empty");
    RuntimeValue value = std::move(data.back());
    data.pop_back();
    return value;
  }
  inline RuntimeValue &Top() {
    if (data.size() == 0)
      throw VM_error("The operation cannot be completed : stack is empty");
    return data.back();
  }
  inline size_t Size() { return data.size(); }
  inline const RuntimeValue &peek(const size_t &pos) { return data[pos]; }
};
