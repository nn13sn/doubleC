#include "error_handle.h"

extern "C" void PrintError(size_t errorNum, size_t bytecodenum) {
  std::cerr << "\nRuntimeError : ";
  switch (errorNum) { // handling error number to the error
  case 1:
    std::cerr << "The string cannot be used with arithmetical, logical or "
                 "unary operations";
    break;
  case 2:
    std::cerr << "The mod operator cannot be used with double";
    break;
  case 3:
    std::cerr << "Division by zero is not allowed";
    break;
  case 4:
    std::cerr << "Such data type cannot be casted to double";
    break;
  case 5:
    std::cerr << "Such data type cannot be casted to int";
    break;
  case 6:
    std::cerr << "Such data type cannot be casted to char";
    break;
  case 7:
    std::cerr << "Such data type cannot be casted to bool";
    break;
  case 8:
    std::cerr << "Cast cannot be completed - the value is out of range";
    break;
  case 9:
    std::cerr << "Negative operator cannot be used with unsigned or "
                 "non-numercal data types";
    break;
  case 10:
    std::cerr
        << "The increment/decrement operator cannot be used with bool datatype";
    break;
  }
  std::cerr << " at line: " +
                   std::to_string(bytecode->locations[bytecodenum].line);
  std::cerr << "; column: " +
                   std::to_string(bytecode->locations[bytecodenum].column)
            << '\n';
}
