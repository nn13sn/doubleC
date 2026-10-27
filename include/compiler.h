#pragma once
#include "JITfunction.h"
#include "asmjit/x86/x86_assembler.h"
#include "bytecode.h"
#include "runtime_value.h"
#include <asmjit/x86.h>
extern "C" size_t current_instruction;
inline auto instruction_address =
    reinterpret_cast<uintptr_t>(&current_instruction);
extern "C" void ReturnValue();
inline auto return_address = reinterpret_cast<uintptr_t>(&ReturnValue);

class Compiler {
public:
  Compiler(const Bytecode &bytecode) : bytecode(bytecode) {};
  JITfunction CompileFunc(const size_t &from, const size_t &to);

private:
  const Bytecode &bytecode;
  const Instruction *instruction;
  void ParseBytecode(asmjit::x86::Assembler &myasm, const size_t &from,
                     const size_t &to);
  void Push(asmjit::x86::Assembler &myasm);
  void Pop(asmjit::x86::Assembler &myasm);
  void Return(asmjit::x86::Assembler &myasm);
  asmjit::JitRuntime runtime;
};
