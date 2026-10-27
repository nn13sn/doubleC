#include "error_handle.h"

extern "C" void PrintError(size_t errorNum, size_t bytecodenum) {
  std::cerr << "\nRuntimeError : ";
  switch (errorNum) { // handling error number to the error
  case 1:
    std::cerr << "The string cannot be used with arithmetical operations";
    break;
  case 2:
    std::cerr << "The mod operator cannot be used with double";
    break;
  case 3:
    std::cerr << "Division by zero is not allowed";
    break;
  }
  std::cerr << " at line: " +
                   std::to_string(bytecode->locations[bytecodenum].line);
  std::cerr << "; column: " +
                   std::to_string(bytecode->locations[bytecodenum].column)
            << '\n';
}
