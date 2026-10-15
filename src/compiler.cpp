#include "compiler.h"
#include "asmjit/core/code_holder.h"
#include "asmjit/x86/x86_assembler.h"
#include "asmjit/x86/x86_operand.h"
#include <iostream>
#include <stdexcept>

void Compiler::Push(asmjit::x86::Assembler &myasm) {
  dc_int a = std::get<dc_int>(bytecode.values[instruction->operand].getData());
  myasm.mov(asmjit::x86::rax, a);
  myasm.push(asmjit::x86::rax);
}

void Compiler::Pop(asmjit::x86::Assembler &myasm) {
  myasm.pop(asmjit::x86::rax);
}

void Compiler::Add(asmjit::x86::Assembler &myasm) {
  Pop(myasm);
  myasm.add(asmjit::x86::qword_ptr(asmjit::x86::rsp),
            asmjit::x86::rax); // no need to pop the stack twice, just rewriting
                               // the first value
}

void Compiler::Sub(asmjit::x86::Assembler &myasm) {
  Pop(myasm);
  myasm.sub(asmjit::x86::qword_ptr(asmjit::x86::rsp), asmjit::x86::rax);
}

void Compiler::Mul(asmjit::x86::Assembler &myasm) {
  Pop(myasm);
  myasm.mov(asmjit::x86::rcx, asmjit::x86::qword_ptr(asmjit::x86::rsp));
  myasm.imul(asmjit::x86::rcx);
  myasm.mov(asmjit::x86::qword_ptr(asmjit::x86::rsp), asmjit::x86::rax);
}

void Compiler::ParseBytecode(asmjit::x86::Assembler &myasm, const size_t &from,
                             const size_t &to) {
  for (size_t i = from; i < to; i++) {
    instruction = &bytecode.code[i];
    switch (instruction->action) {
    case Action::Push_Value:
      Push(myasm);
      break;
    case Action::Pop:
      Pop(myasm);
      break;
    case Action::Add:
      Add(myasm);
      break;
    case Action::Sub:
      Sub(myasm);
      break;
    case Action::Mul:
      Mul(myasm);
      break;
    case Action::Return:
      Pop(myasm);
      myasm.ret();
      break;
    default:
      break;
    }
  }
}

JITfunction Compiler::CompileFunc(const size_t &from, const size_t &to) {
  asmjit::CodeHolder code;
  code.init(runtime.environment(), runtime.cpu_features());

  asmjit::x86::Assembler myasm(&code);

  ParseBytecode(myasm, from, to);

  JITfunction func;
  asmjit::Error err = runtime.add(&func, &code);
  if (err != asmjit::Error::kOk) {
    std::cout << static_cast<int64_t>(err);
    throw std::invalid_argument("\nError");
  }
  return func;
}
