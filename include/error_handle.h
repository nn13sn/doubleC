#pragma once
#include "bytecode.h"
#include <iostream>

extern const Bytecode *bytecode;

extern "C" void PrintError(size_t errorNum, size_t bytecodenum);
