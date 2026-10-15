#pragma once
#include "asmjit/x86/x86_assembler.h"
#include "bytecode.h"
#include "runtime_value.h"
#include <asmjit/x86.h>
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
  void Add(asmjit::x86::Assembler &myasm);
  void Sub(asmjit::x86::Assembler &myasm);
  void Mul(asmjit::x86::Assembler &myasm);
  asmjit::JitRuntime runtime;
};
