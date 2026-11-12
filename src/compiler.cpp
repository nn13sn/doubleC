#include "compiler.h"
#include "asmjit/core/code_holder.h"
#include "asmjit/x86/x86_assembler.h"
#include "asmjit/x86/x86_operand.h"
#include "runtimeJIT_operations.h"
#include "runtime_cast.h"
#include "stack.h"
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

void Compiler::ParseBytecode(std::vector<asmjit::Label> &labels,
                             asmjit::x86::Assembler &myasm, const size_t &from,
                             const size_t &to) {
  for (size_t i = from; i < to; i++) {
    myasm.bind(labels[i - from]);
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
    case Action::Not:
      RuntimeJITOperations::logicnot(myasm);
      break;
    case Action::Neg:
      RuntimeJITOperations::neg(myasm);
      break;
    case Action::PreIncr:
      myasm.mov(asmjit::x86::rdi, instruction->operand);
      RuntimeJITOperations::preincr(myasm);
      break;
    case Action::PostIncr:
      myasm.mov(asmjit::x86::rdi, instruction->operand);
      RuntimeJITOperations::postincr(myasm);
      break;
    case Action::PreDecr:
      myasm.mov(asmjit::x86::rdi, instruction->operand);
      RuntimeJITOperations::predecr(myasm);
      break;
    case Action::PostDecr:
      myasm.mov(asmjit::x86::rdi, instruction->operand);
      RuntimeJITOperations::postdecr(myasm);
      break;
    case Action::Return:
      Return(myasm);
      break;
    case Action::Cast:
      myasm.mov(asmjit::x86::rdi, instruction->operand);
      RuntimeJITOperations::cast(myasm);
      break;
    case Action::Load_Local:
      myasm.mov(asmjit::x86::rax, instruction->operand);
      myasm.shl(asmjit::x86::rax, 4);
      myasm.add(asmjit::x86::rax, asmjit::x86::qword_ptr(asmjit::x86::r12, 8));
      myasm.push(asmjit::x86::qword_ptr(asmjit::x86::rax));
      myasm.push(asmjit::x86::qword_ptr(asmjit::x86::rax, 8));
      break;
    case Action::Store_Local:
      myasm.mov(asmjit::x86::rax, instruction->operand);
      myasm.shl(asmjit::x86::rax, 4);
      myasm.add(asmjit::x86::rax, asmjit::x86::qword_ptr(asmjit::x86::r12, 8));
      myasm.mov(asmjit::x86::rdi, asmjit::x86::qword_ptr(asmjit::x86::rsp));
      myasm.mov(asmjit::x86::qword_ptr(asmjit::x86::rax, 8), asmjit::x86::rdi);
      myasm.mov(asmjit::x86::rdi, asmjit::x86::qword_ptr(asmjit::x86::rsp, 8));
      myasm.mov(asmjit::x86::qword_ptr(asmjit::x86::rax), asmjit::x86::rdi);
      break;
    case Action::DefaultInit:
      myasm.mov(asmjit::x86::rax, instruction->operand);
      myasm.shl(asmjit::x86::rax, 4);
      myasm.add(asmjit::x86::rax, asmjit::x86::qword_ptr(asmjit::x86::r12, 8));
      myasm.cmp(asmjit::x86::qword_ptr(asmjit::x86::rax), 7); // invalid
                                                              // datatype
      myasm.jne(labels[i - from + 1]);
      myasm.mov(asmjit::x86::qword_ptr(asmjit::x86::rax), 0);
      myasm.mov(asmjit::x86::qword_ptr(asmjit::x86::rax, 8), 0);
      break;
    case Action::Jump:
      myasm.jmp(labels[instruction->operand - from]);
      break;
    case Action::JumpIfFalse:
      Pop(myasm);
      myasm.test(asmjit::x86::rax, asmjit::x86::rax);
      myasm.jz(labels[instruction->operand - from]);
      break;
    case Action::Read:
      myasm.mov(asmjit::x86::rdi, instruction->operand);
      myasm.mov(asmjit::x86::rax, input_address);
      myasm.call(input_address);
      break;
    case Action::Print:
      myasm.pop(asmjit::x86::rsi);
      myasm.pop(asmjit::x86::rdi);
      myasm.mov(asmjit::x86::rax, print_address);
      myasm.call(asmjit::x86::rax);
      break;
    case Action::ForInit:
      myasm.mov(asmjit::x86::rax, forinit_address);
      myasm.call(asmjit::x86::rax);
      myasm.mov(asmjit::x86::rax, asmjit::x86::qword_ptr(asmjit::x86::r12, 24));
      myasm.mov(asmjit::x86::qword_ptr(asmjit::x86::rax, 16),
                instruction->operand >> 2);
      myasm.mov(asmjit::x86::rcx, instruction->operand >> 2);
      myasm.shl(asmjit::x86::rcx, 4);
      myasm.add(asmjit::x86::rcx, asmjit::x86::qword_ptr(asmjit::x86::r12, 8));
      myasm.mov(asmjit::x86::rdx, asmjit::x86::qword_ptr(asmjit::x86::rcx));
      myasm.mov(asmjit::x86::qword_ptr(asmjit::x86::rax, 32), asmjit::x86::rdx);
      myasm.mov(asmjit::x86::rdx, asmjit::x86::qword_ptr(asmjit::x86::rcx, 8));
      myasm.mov(asmjit::x86::qword_ptr(asmjit::x86::rax, 40), asmjit::x86::rdx);
      myasm.mov(asmjit::x86::rcx, asmjit::x86::qword_ptr(asmjit::x86::rsp, 8));
      myasm.mov(asmjit::x86::qword_ptr(asmjit::x86::rax), asmjit::x86::rcx);
      myasm.mov(asmjit::x86::rcx, asmjit::x86::qword_ptr(asmjit::x86::rsp));
      myasm.mov(asmjit::x86::qword_ptr(asmjit::x86::rax, 8), asmjit::x86::rcx);
      notsmart.push_back(false);
      if ((instruction->operand & 0b11) == 1)
        myasm.mov(asmjit::x86::byte_ptr(asmjit::x86::rax, 25), 0);
      else if ((instruction->operand & 0b11) == 2)
        myasm.mov(asmjit::x86::byte_ptr(asmjit::x86::rax, 25), 1);
      else {
        notsmart.back() = true;
        Pop(myasm); // deleting start and the end because forcheck will push
                    // them again
        Pop(myasm);
        break;
      }
      RuntimeJITOperations::lesseq(myasm);
      myasm.pop(asmjit::x86::rcx);
      myasm.add(asmjit::x86::rsp, 8);
      myasm.mov(asmjit::x86::rax, asmjit::x86::qword_ptr(asmjit::x86::r12, 24));
      myasm.mov(asmjit::x86::byte_ptr(asmjit::x86::rax, 24), asmjit::x86::cl);
      break;
    case Action::ForCheck: {
      myasm.mov(asmjit::x86::rax, asmjit::x86::qword_ptr(asmjit::x86::r12, 24));
      myasm.mov(asmjit::x86::rax, asmjit::x86::qword_ptr(asmjit::x86::rax, 16));
      myasm.shl(asmjit::x86::rax, 4);
      myasm.add(asmjit::x86::rax, asmjit::x86::qword_ptr(asmjit::x86::r12, 8));
      myasm.push(asmjit::x86::qword_ptr(asmjit::x86::rax));
      myasm.push(asmjit::x86::qword_ptr(asmjit::x86::rax, 8));
      myasm.mov(asmjit::x86::rax, asmjit::x86::qword_ptr(asmjit::x86::r12, 24));
      myasm.push(asmjit::x86::qword_ptr((asmjit::x86::rax)));
      myasm.push(asmjit::x86::qword_ptr(asmjit::x86::rax, 8));
      if (notsmart.back())
        break;
      myasm.xor_(asmjit::x86::rcx, asmjit::x86::rcx);
      myasm.mov(asmjit::x86::cl, asmjit::x86::byte_ptr(asmjit::x86::rax, 24));
      myasm.test(asmjit::x86::cl, asmjit::x86::cl);
      auto negdirection = myasm.new_label();
      myasm.jz(negdirection);
      myasm.mov(asmjit::x86::cl, asmjit::x86::byte_ptr(asmjit::x86::rax, 25));
      myasm.test(asmjit::x86::cl, asmjit::x86::cl);
      auto posdirectionNotinclusive = myasm.new_label();
      myasm.jz(posdirectionNotinclusive);
      RuntimeJITOperations::lesseq(myasm);
      myasm.jmp(labels[i - from + 1]);
      myasm.bind(posdirectionNotinclusive);
      RuntimeJITOperations::less(myasm);
      myasm.jmp(labels[i - from + 1]);
      myasm.bind(negdirection);
      myasm.mov(asmjit::x86::rax, asmjit::x86::qword_ptr(asmjit::x86::r12, 24));
      myasm.mov(asmjit::x86::cl, asmjit::x86::byte_ptr(asmjit::x86::rax, 25));
      myasm.test(asmjit::x86::cl, asmjit::x86::cl);
      auto negdirectionNotinclusive = myasm.new_label();
      myasm.jz(negdirectionNotinclusive);
      RuntimeJITOperations::greatereq(myasm);
      myasm.jmp(labels[i - from + 1]);
      myasm.bind(negdirectionNotinclusive);
      RuntimeJITOperations::greater(myasm);
      break;
    }
    case Action::ForStep: {
      myasm.mov(asmjit::x86::rax, asmjit::x86::qword_ptr(asmjit::x86::r12, 24));
      myasm.mov(asmjit::x86::cl, asmjit::x86::byte_ptr(asmjit::x86::rax, 24));
      myasm.test(asmjit::x86::cl, asmjit::x86::cl);
      auto negdirection = myasm.new_label();
      myasm.jz(negdirection);
      myasm.mov(asmjit::x86::rdi, asmjit::x86::qword_ptr(asmjit::x86::rax, 16));
      RuntimeJITOperations::preincr(myasm);
      myasm.add(asmjit::x86::rsp, 16);
      myasm.jmp(labels[i - from + 1]);
      myasm.bind(negdirection);
      myasm.mov(asmjit::x86::rax, asmjit::x86::qword_ptr(asmjit::x86::r12, 24));
      myasm.mov(asmjit::x86::rdi, asmjit::x86::qword_ptr(asmjit::x86::rax, 16));
      RuntimeJITOperations::predecr(myasm);
      myasm.add(asmjit::x86::rsp, 16);
      break;
    }
    case Action::ForEnd:
      myasm.mov(asmjit::x86::rax, asmjit::x86::qword_ptr(asmjit::x86::r12, 24));
      myasm.mov(asmjit::x86::rcx, asmjit::x86::qword_ptr(asmjit::x86::rax, 16));
      myasm.shl(asmjit::x86::rcx, 4);
      myasm.add(asmjit::x86::rcx, asmjit::x86::qword_ptr(asmjit::x86::r12, 8));
      myasm.mov(asmjit::x86::rdx, asmjit::x86::qword_ptr(asmjit::x86::rax, 32));
      myasm.mov(asmjit::x86::qword_ptr(asmjit::x86::rcx), asmjit::x86::rdx);
      myasm.mov(asmjit::x86::rdx, asmjit::x86::qword_ptr(asmjit::x86::rax, 40));
      myasm.mov(asmjit::x86::qword_ptr(asmjit::x86::rcx, 8), asmjit::x86::rdx);
      myasm.mov(asmjit::x86::rax, forend_address);
      myasm.call(asmjit::x86::rax);
      notsmart.pop_back();
      break;
    default:
      break;
    }
  }
}

JITfunction Compiler::CompileFunc(const size_t &from, const size_t &to,
                                  const size_t &numberOfParams) {

  asmjit::CodeHolder code;
  code.init(runtime.environment(), runtime.cpu_features());

  asmjit::x86::Assembler myasm(&code);

  std::vector<asmjit::Label> labels;

  myasm.push(asmjit::x86::r12);
  myasm.mov(asmjit::x86::r12, asmjit::x86::rdi);

  myasm.mov(asmjit::x86::rax, asmjit::x86::qword_ptr(asmjit::x86::r12, 16));
  for (size_t i = numberOfParams; i > 0; i--) {
    myasm.mov(asmjit::x86::rcx, i - 1);
    myasm.shl(asmjit::x86::rcx, 4);
    myasm.add(asmjit::x86::rcx, asmjit::x86::rax);
    myasm.push(asmjit::x86::qword_ptr(asmjit::x86::rcx));
    myasm.push(asmjit::x86::qword_ptr(asmjit::x86::rcx, 8));
  }

  for (size_t i = from; i < to; i++) {
    labels.push_back(myasm.new_label());
  }

  ParseBytecode(labels, myasm, from, to);

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
