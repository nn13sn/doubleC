#pragma once
#include "runtime_value.h"
#include "vm_error.h"
#include <vector>
class VM;
/// the stack class, the stack is the array used for temporary values. All of
/// the operation use stack to push and delete values, and pass them to the next
/// instruction
class Stack {
private:
  /// stack itself
  std::vector<RuntimeValue> data = {};

public:
  /// pushes the value to the top of the stack
  inline void Push(RuntimeValue value) { data.push_back(std::move(value)); }
  /// pops and returns the value from the top of the stack
  inline RuntimeValue Pop() {
    if (data.size() == 0)
      throw VM_error("The operation cannot be completed : stack is empty");
    RuntimeValue value = std::move(data.back());
    data.pop_back();
    return value;
  }
  /// returns the value from the top of the stack
  inline RuntimeValue &Top() {
    if (data.size() == 0)
      throw VM_error("The operation cannot be completed : stack is empty");
    return data.back();
  }
  /// returns the size of the stack
  inline size_t Size() { return data.size(); }
  /// peeks at a specific value of the stack
  inline const RuntimeValue &peek(const size_t &pos) { return data[pos]; }
  friend VM;
};
