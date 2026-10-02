#pragma once
#include "bytecode.h"
#include "callstate.h"
#include "environment.h"
#include "forstate.h"
#include "functionenv.h"
#include "runtime_variable.h"
#include "stack.h"
#define VM_ERROR -1
#define VM_OK 1
class VM {
public:
  VM(const uint32_t &size, const size_t &funcsize);
  signed char evaluate(const Bytecode &code);

private:
  Stack stack;
  std::shared_ptr<Environment> env;
  std::vector<ForState> ForStates = {};
  std::vector<CallState> CallStates = {};
  size_t instruction_number = 0;
  void Add();
  void Sub();
  void Mul();
  void Div();
  template <typename OperationFunc> void BinaryOperation(OperationFunc func);
  template <typename OperationFunc> void UnaryOperation(OperationFunc func);
};
