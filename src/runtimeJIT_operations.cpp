#include "runtimeJIT_operations.h"
#include "asmjit/x86/x86_assembler.h"
#include "asmjit/x86/x86_operand.h"

void RuntimeJITOperations::add(asmjit::x86::Assembler &myasm) {
  myasm.mov(asmjit::x86::rax, sum_address);
  myasm.call(asmjit::x86::rax);
}

void RuntimeJITOperations::sub(asmjit::x86::Assembler &myasm) {
  myasm.mov(asmjit::x86::rax, sub_address);
  myasm.call(asmjit::x86::rax);
}

void RuntimeJITOperations::mul(asmjit::x86::Assembler &myasm) {
  myasm.mov(asmjit::x86::rax, mul_address);
  myasm.call(asmjit::x86::rax);
}

void RuntimeJITOperations::div(asmjit::x86::Assembler &myasm) {
  myasm.mov(asmjit::x86::rax, div_address);
  myasm.call(asmjit::x86::rax);
}

void RuntimeJITOperations::mod(asmjit::x86::Assembler &myasm) {
  myasm.mov(asmjit::x86::rax, mod_address);
  myasm.call(asmjit::x86::rax);
}

void RuntimeJITOperations::less(asmjit::x86::Assembler &myasm) {
  myasm.mov(asmjit::x86::rax, less_address);
  myasm.call(asmjit::x86::rax);
}

void RuntimeJITOperations::greater(asmjit::x86::Assembler &myasm) {
  myasm.mov(asmjit::x86::rax, greater_address);
  myasm.call(asmjit::x86::rax);
}

void RuntimeJITOperations::lesseq(asmjit::x86::Assembler &myasm) {
  myasm.mov(asmjit::x86::rax, lesseq_address);
  myasm.call(asmjit::x86::rax);
}

void RuntimeJITOperations::greatereq(asmjit::x86::Assembler &myasm) {
  myasm.mov(asmjit::x86::rax, greatereq_address);
  myasm.call(asmjit::x86::rax);
}

void RuntimeJITOperations::equal(asmjit::x86::Assembler &myasm) {
  myasm.mov(asmjit::x86::rax, equal_address);
  myasm.call(asmjit::x86::rax);
}

void RuntimeJITOperations::notequal(asmjit::x86::Assembler &myasm) {
  myasm.mov(asmjit::x86::rax, notequal_address);
  myasm.call(asmjit::x86::rax);
}

void RuntimeJITOperations::cast(asmjit::x86::Assembler &myasm) {
  myasm.mov(asmjit::x86::rax, generalcast_address);
  myasm.call(asmjit::x86::rax);
}

void RuntimeJITOperations::logicnot(asmjit::x86::Assembler &myasm) {
  myasm.mov(asmjit::x86::rax, not_address);
  myasm.call(asmjit::x86::rax);
}

void RuntimeJITOperations::neg(asmjit::x86::Assembler &myasm) {
  myasm.mov(asmjit::x86::rax, neg_address);
  myasm.call(asmjit::x86::rax);
}

void RuntimeJITOperations::preincr(asmjit::x86::Assembler &myasm) {
  myasm.mov(asmjit::x86::rax, preincr_address);
  myasm.call(asmjit::x86::rax);
}

void RuntimeJITOperations::postincr(asmjit::x86::Assembler &myasm) {
  myasm.mov(asmjit::x86::rax, postincr_address);
  myasm.call(asmjit::x86::rax);
}

void RuntimeJITOperations::predecr(asmjit::x86::Assembler &myasm) {
  myasm.mov(asmjit::x86::rax, predecr_address);
  myasm.call(asmjit::x86::rax);
}

void RuntimeJITOperations::postdecr(asmjit::x86::Assembler &myasm) {
  myasm.mov(asmjit::x86::rax, postdecr_address);
  myasm.call(asmjit::x86::rax);
}
