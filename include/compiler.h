#pragma once
#include "JITfunction.h"
#include "asmjit/x86/x86_assembler.h"
#include "bytecode.h"
#include "runtime_value.h"
#include "stack.h"
#include <asmjit/x86.h>
extern "C" size_t current_instruction;
inline auto instruction_address =
    reinterpret_cast<uintptr_t>(&current_instruction);

extern "C" void ReturnValue();
extern "C" void asmPrint();
extern "C" void asmInput();
inline auto return_address = reinterpret_cast<uintptr_t>(&ReturnValue);
inline auto input_address = reinterpret_cast<uintptr_t>(&asmInput);
inline auto print_address = reinterpret_cast<uintptr_t>(&asmPrint);

class Compiler {
public:
  Compiler(const Bytecode &bytecode) : bytecode(bytecode) {};
  JITfunction CompileFunc(const size_t &from, const size_t &to,
                          const size_t &numberOfParams);

private:
  const Bytecode &bytecode;
  const Instruction *instruction;
  void ParseBytecode(std::vector<asmjit::Label> &labels,
                     asmjit::x86::Assembler &myasm, const size_t &from,
                     const size_t &to);
  void Push(asmjit::x86::Assembler &myasm);
  void Pop(asmjit::x86::Assembler &myasm);
  void Return(asmjit::x86::Assembler &myasm);
  asmjit::JitRuntime runtime;
};
