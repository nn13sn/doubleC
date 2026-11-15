#pragma once
#include "AST.h"
#include "bytecode.h"
#include "slot_table.h"
class Generator {
  // generator class, it generates bytecode out of AST
public:
  /// start of generation
  Bytecode &StartGeneration(const Program &program);
  /// the main method, matches the AST statement
  void Generate(const Program &program);
  const Slot_Table &indexes;
  Generator(const Slot_Table &table) : indexes(table) {};

private:
  /// generating all types of expressions
  void GenerateExpression(const Expression &expr);
  /// generaing output instruction
  void GenerateOutput(const Output &stmt);
  /// generating input instruction
  void GenerateInput(const Input &stmt);
  /// generaing if statement instruction (basically jumps)
  void GenerateIf(const IfStatement &stmt);
  /// generaing while loop instructions
  void GenerateWhile(const While &stmt);
  /// generating for loop instructions
  void GenerateFor(const For &stmt);
  /// generaing function instructions
  void GenerateFunction(const FunctionStatement &stmt);
  /// generaing return instructions
  void GenerateReturn(const ReturnStatement &stmt);
  Bytecode code;
  uint32_t index = 0;
  std::vector<const FunctionStatement *> AllFunctions = {};
  /**
   * a helper function, it adds one more bytecode element with provided action
   * and operand. The locations are stored at the same index but in different
   * array.
   */
  size_t emit(Location location, const Action &action, uint32_t operand = 0);
  /**
   * finishing jump - block statements require jumps forward or before,
   * therefore the operands cannot be assigned right away, so this function
   * assigns operands to jump instructions at the time it becomes possible
   */
  void FinishJump(const size_t &instruction, const size_t &target);
  /// generating body(block)
  void GenerateBody(const Program &program);
};
