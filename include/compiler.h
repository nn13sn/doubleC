#pragma once
#include "JITfunction.h"
#include "asmjit/x86/x86_assembler.h"
#include "bytecode.h"
#include "environment.h"
#include "runtime_value.h"
#include "stack.h"
#include <asmjit/x86.h>
extern "C" size_t current_instruction;
inline auto instruction_address =
    reinterpret_cast<uintptr_t>(&current_instruction);

extern "C" void ReturnValue();
extern "C" void asmPrint();
extern "C" void asmInput();
extern "C" void asmForInit();
extern "C" void asmForEnd();
inline auto return_address = reinterpret_cast<uintptr_t>(&ReturnValue);
inline auto input_address = reinterpret_cast<uintptr_t>(&asmInput);
inline auto print_address = reinterpret_cast<uintptr_t>(&asmPrint);
inline auto forinit_address = reinterpret_cast<uintptr_t>(&asmForInit);
inline auto forend_address = reinterpret_cast<uintptr_t>(&asmForEnd);

struct CompilationContext {
  const Instruction *instruction = nullptr;
  std::vector<bool> notsmart{};
};

class Compiler {
public:
  Compiler(const Bytecode &bytecode, std::shared_ptr<Environment> envptr)
      : bytecode(bytecode) {
    this->envptr = envptr;
  };
  /**
   * the main compilation function, it copies the values into the current stack,
   * saves r12 and translates every instruction into asm(nasm)
   */
  JITfunction CompileFunc(const size_t &from, const size_t &to,
                          const size_t &numberOfParams);

private:
  std::shared_ptr<Environment> envptr;
  const Bytecode &bytecode;
  /// the main method, matches the instruction and translates into assembly
  void ParseBytecode(std::vector<asmjit::Label> &labels,
                     asmjit::x86::Assembler &myasm, const size_t &from,
                     const size_t &to, CompilationContext &context);
  /**
   * translates the push operation in bytecode into push operation in assembly.
   * Every datatype requires a different set of operation, unlike in vm.
   */
  void Push(asmjit::x86::Assembler &myasm, CompilationContext &context);
  /**
   * poping the last two values from the stack(datatype and value itself, 16
   * bytes in total)
   */
  void Pop(asmjit::x86::Assembler &myasm);

  /**
   * the return instruction, it differs if the compiled version is calling the
   * function, or the vm is calling the function. The r12 register needs to be
   * restored after the functions ends as well. (its callee saved)
   */
  void Return(asmjit::x86::Assembler &myasm, CompilationContext &context);
  std::mutex runtime_mutex;
  asmjit::JitRuntime runtime;
};
