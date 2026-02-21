#include <iostream>
#include "lexer.h"
#include "AST.h"
#include "parser.h"
#include "interpreter.h"

int main(){
  try{
    Program program;
    std::vector <std::string> code = readFile("code.txt");
    std::vector <std::vector <Token>> tokens = Tokenize(code);
    Parser parser(tokens);
    parser.Parse(program);
    Interpreter interpreter;
    interpreter.execute(program);
  }
  catch(const std::invalid_argument& err){
    std::cerr << "Syntax error: " << err.what() << std::endl;
    return -1;
  }
  catch(const interpreter_error& err){
    std::cerr << "Runtime error: "<< err.what() << " at line: " + std::to_string(err.location.line);
    if(err.location.column != 0) std::cerr<< "; column: " + std::to_string(err.location.column);
    std::cerr<<std::endl;
    return -2;
  }
  catch(const std::runtime_error& err){
    std::cerr << "Runtime error: " << err.what() << std::endl;
    return -3;
  }
  return 0;
}
