#pragma once

#include "asmjit/x86/x86_assembler.h"
namespace RuntimeJITOperations {
extern "C" {
void sumInt();
void subInt();
void mulInt();
void divInt();
void modInt();
void LogicLess();
void LogicGreater();
void LogicLessEQ();
void LogicGreaterEQ();
void LogicEqual();
void LogicNotEqual();
}
inline auto sum_address = reinterpret_cast<uintptr_t>(&sumInt);
inline auto sub_address = reinterpret_cast<uintptr_t>(&subInt);
inline auto mul_address = reinterpret_cast<uintptr_t>(&mulInt);
inline auto div_address = reinterpret_cast<uintptr_t>(&divInt);
inline auto mod_address = reinterpret_cast<uintptr_t>(&modInt);
inline auto less_address = reinterpret_cast<uintptr_t>(&LogicLess);
inline auto greater_address = reinterpret_cast<uintptr_t>(&LogicGreater);
inline auto lesseq_address = reinterpret_cast<uintptr_t>(&LogicLessEQ);
inline auto greatereq_address = reinterpret_cast<uintptr_t>(&LogicGreaterEQ);
inline auto equal_address = reinterpret_cast<uintptr_t>(&LogicEqual);
inline auto notequal_address = reinterpret_cast<uintptr_t>(&LogicNotEqual);

void add(asmjit::x86::Assembler &myasm);
void sub(asmjit::x86::Assembler &myasm);
void mul(asmjit::x86::Assembler &myasm);
void div(asmjit::x86::Assembler &myasm);
void mod(asmjit::x86::Assembler &myasm);
void less(asmjit::x86::Assembler &myasm);
void greater(asmjit::x86::Assembler &myasm);
void lesseq(asmjit::x86::Assembler &myasm);
void greatereq(asmjit::x86::Assembler &myasm);
void equal(asmjit::x86::Assembler &myasm);
void notequal(asmjit::x86::Assembler &myasm);
} // namespace RuntimeJITOperations
