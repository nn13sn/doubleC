#include "compiler.h"
#include "asmjit/core/code_holder.h"
#include "asmjit/x86/x86_assembler.h"
#include "asmjit/x86/x86_operand.h"
#include "runtimeJIT_operations.h"
#include "runtime_cast.h"
#include <cstring>
#include <iostream>
#include <stdexcept>

void Compiler::Push(asmjit::x86::Assembler &myasm) {
  auto &data = bytecode.values[instruction->operand];
  switch (data.getType()) {
  case Datatype::Int:
    myasm.mov(asmjit::x86::rax, static_cast<int64_t>(Datatype::Int));
    myasm.push(asmjit::x86::rax);
    myasm.mov(asmjit::x86::rax, RuntimeCast::As<dc_int>(data));
    myasm.push(asmjit::x86::rax);
    break;
  case Datatype::Double: {
    dc_double value = RuntimeCast::As<dc_double>(data);
    uint64_t doublebits;
    static_assert(sizeof(doublebits) == sizeof(value));
    std::memcpy(
        &doublebits, &value,
        sizeof(doublebits)); // basically copying the bits of double value and
                             // saving them to rax later, as it is not allowed
                             // to save double to xmm0 directly
    myasm.mov(asmjit::x86::rax, static_cast<int64_t>(Datatype::Double));
    myasm.push(asmjit::x86::rax);
    myasm.mov(asmjit::x86::rax, doublebits);
    myasm.push(asmjit::x86::rax);
    break;
  }
  case Datatype::Char:
    myasm.mov(asmjit::x86::rax, static_cast<int64_t>(Datatype::Char));
    myasm.push(asmjit::x86::rax);
    myasm.mov(asmjit::x86::rax, RuntimeCast::As<dc_char>(data));
    myasm.push(asmjit::x86::rax);
    break;
  case Datatype::Bool:
    myasm.mov(asmjit::x86::rax, static_cast<int64_t>(Datatype::Bool));
    myasm.push(asmjit::x86::rax);
    myasm.mov(asmjit::x86::rax, RuntimeCast::As<dc_bool>(data));
    myasm.push(asmjit::x86::rax);
    break;
  case Datatype::String:
    myasm.mov(asmjit::x86::rax, static_cast<int64_t>(Datatype::String));
    myasm.push(asmjit::x86::rax);
    myasm.mov(asmjit::x86::rax, &std::get<dc_string>(data.getData()));
    myasm.push(asmjit::x86::rax);
    break;
  default:
    break;
  }
}

void Compiler::Pop(asmjit::x86::Assembler &myasm) {
  myasm.pop(asmjit::x86::rax); // value
  myasm.pop(asmjit::x86::rcx); // data type
}

void Compiler::Return(asmjit::x86::Assembler &myasm) {
  if (instruction->operand) {
    myasm.pop(asmjit::x86::rsi);
    myasm.pop(asmjit::x86::rdi);
    myasm.mov(asmjit::x86::rdx, asmjit::x86::qword_ptr(asmjit::x86::r12));
    myasm.mov(asmjit::x86::rax, return_address);
    myasm.call(asmjit::x86::rax);
  }
  myasm.pop(asmjit::x86::r12);
  myasm.ret();
}

void Compiler::ParseBytecode(asmjit::x86::Assembler &myasm, const size_t &from,
                             const size_t &to) {
  for (size_t i = from; i < to; i++) {
    instruction = &bytecode.code[i];
    myasm.mov(asmjit::x86::rax, instruction_address);
    myasm.mov(asmjit::x86::qword_ptr(asmjit::x86::rax), i);
    switch (instruction->action) {
    case Action::Push_Value:
      Push(myasm);
      break;
    case Action::Pop:
      Pop(myasm);
      break;
    case Action::Add:
      RuntimeJITOperations::add(myasm);
      break;
    case Action::Sub:
      RuntimeJITOperations::sub(myasm);
      break;
    case Action::Mul:
      RuntimeJITOperations::mul(myasm);
      break;
    case Action::Div:
      RuntimeJITOperations::div(myasm);
      break;
    case Action::Mod:
      RuntimeJITOperations::mod(myasm);
      break;
    case Action::Less:
      RuntimeJITOperations::less(myasm);
      break;
    case Action::Greater:
      RuntimeJITOperations::greater(myasm);
      break;
    case Action::LessEq:
      RuntimeJITOperations::lesseq(myasm);
      break;
    case Action::GreaterEq:
      RuntimeJITOperations::greatereq(myasm);
      break;
    case Action::Equal:
      RuntimeJITOperations::equal(myasm);
      break;
    case Action::NotEqual:
      RuntimeJITOperations::notequal(myasm);
      break;
    case Action::Return:
      Return(myasm);
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

  myasm.push(asmjit::x86::r12);
  myasm.mov(asmjit::x86::r12, asmjit::x86::rdi);
  ParseBytecode(myasm, from, to);

  myasm.pop(asmjit::x86::r12);
  myasm.ret();
  JITfunction func;
  asmjit::Error err = runtime.add(&func, &code);
  if (err != asmjit::Error::kOk) {
    std::cout << static_cast<int64_t>(err);
    throw std::invalid_argument("\nError");
  }
  return func;
}
