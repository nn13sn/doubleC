#pragma once
#include "AST.h"
#include "bytecode.h"
#include "slot_table.h"
class Generator {
public:
  const Bytecode &StartGeneration(const Program &program);
  void Generate(const Program &program);
  const Slot_Table &indexes;
  Generator(const Slot_Table &table) : indexes(table) {};

private:
  void GenerateExpression(const Expression &expr);
  void GenerateOutput(const Output &stmt);
  void GenerateInput(const Input &stmt);
  void GenerateIf(const IfStatement &stmt);
  void GenerateWhile(const While &stmt);
  void GenerateFor(const For &stmt);
  void GenerateFunction(const FunctionStatement &stmt);
  void GenerateReturn(const ReturnStatement &stmt);
  Bytecode code;
  uint32_t index = 0;
  std::vector<const FunctionStatement *> AllFunctions = {};

  size_t emit(Location location, const Action &action, uint32_t operand = 0);
  void FinishJump(const size_t &instruction, const size_t &target);
  void GenerateBody(const Program &program);
};
