#pragma once
#include "bytecode.h"
#include <iostream>

extern const Bytecode *bytecode;
/**
 * mathcing the error code and priniting it, the function is called from the
 * asm file, that why it is defined in c style
 */
extern "C" void PrintError(size_t errorNum, size_t bytecodenum);
