#pragma once
#include "bytecode.h"
#include "callstate.h"
#include "compiler.h"
#include "environment.h"
#include "forstate.h"
#include "runtime_variable.h"
#include "stack.h"
#define VM_ERROR -1
#define VM_OK 1

inline std::shared_ptr<Environment> envptr;

class VM {
  // VM class, the main execution happens here
public:
  /// constructor
  VM(const uint32_t &size, const size_t &funcsize);
  /// the main method, matches the bytecode instruction and executes it
  signed char evaluate(Bytecode &code);
  /**
   * destructor, it waits until every compilation thread ends, or otherwise a
   * program would end with abort(). It would be nice to finish all other
   * compilation threads as they are no longer needed, but std::thread doesn't
   * really provide any possible kill method. So it is the best way to
   * succesfully finish the program.
   */
  ~VM();

private:
  std::unique_ptr<Compiler> compiler;
  /**
   * a compilation in a different thread starts here, also checks if the
   * compilation is not already going.
   */
  void getCompiled(const size_t &funcid);

  Stack stack;
  std::shared_ptr<Environment> env;
  std::vector<ForState> ForStates = {};
  std::vector<CallState> CallStates = {};
  size_t instruction_number = 0;
  /**
   * template function for all binary operations, as they all have the same
   * input values
   */
  template <typename OperationFunc> void BinaryOperation(OperationFunc func);
  /// template function for unary operations
  template <typename OperationFunc> void UnaryOperation(OperationFunc func);
};
