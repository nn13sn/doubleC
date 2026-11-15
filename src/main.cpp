#define DC_OK 0
#define DC_LEXER_ERROR -1
#define DC_PARSER_ERROR -2
#define DC_ANALYZER_ERROR -3
#define DC_RUNTIME_ERROR -4
#define DC_FILEPATH_ERROR -5

#include "AST.h"
#include "VM.h"
#include "analyzer.h"
#include "compiler.h"
#include "error_handle.h"
#include "generator.h"
#include "lexer.h"
#include "parser.h"
#include <iostream>

const Bytecode *bytecode = nullptr;

int main(int argc, char *argv[]) {
  try {
    if (argc == 1) {
      std::cout << "The path is expected to be provided\n";
      return DC_FILEPATH_ERROR;
    }
    std::string path = argv[1];
    Program program;
    Lexer lexer;
    // 1st stage - lexer, lexer divides every single element of a code into
    // tokens. After that stage we have a vector of tokens, or a tokenized
    // representation of a code.
    lexer.readFile(path);
    auto tokens = lexer.Tokenize();
    Parser parser(tokens);
    // next, 2nd stage - parser, its job is to form a logical representation of
    // code out of tokens (AST). Simpy put, a sequence of tokens now forms a
    // clearer representation of what exactly code is doing. Like loop, variable
    // assign and etc.
    if (parser.Parse(program) == PARSER_ERROR) {
      // if an error happens at parser level, all of them will get printed and
      // program is going to end
      parser.printErrors();
      return DC_PARSER_ERROR;
    }
    tokens.clear();
    // std::cout << "Reached Analyzer!\n";
    Analyzer analyzer;
    // 3rd stage - analyzer, or also known as semantic layer. The main purpose
    // is to catch early recognizible errors, a semantic errors. Analyzer also
    // gives each variable and function a slot, as storing string names and
    // scopes in VM would be unnecessary.
    if (analyzer.analyze(program) == AnalyzerError) {
      analyzer.printErrors();
      return DC_ANALYZER_ERROR;
    }
    // std::cout << "Reached Generator!\n";
    Generator generator(analyzer.table);
    // 4th stage - generator. Generator's puspose is to generate a bytecode out
    // of ast. My old version of interpreter used to work  with ast right away,
    // but that's quite insufficient. The reason - ast requires to always jump
    // from one memory adress to another, and doing so repeatedly in inerpreter
    // would cost a lot of time.
    auto code = std::move(generator.StartGeneration(program));
    program.statements.clear(); // clearing the AST as it is no longer needed
                                // after the generation of bytecode
    // std::cout << "Reached VM!\n";
    bytecode = &code;
    VM vm(analyzer.table.slots.size(), analyzer.table.FunctionSlots.size());
    // final, 5th stage - VM (virtual machine). It executes every instruction
    // that was generated before.
    auto result = vm.evaluate(code);
    if (result == VM_ERROR)
      return DC_RUNTIME_ERROR;
    return DC_OK;
  } catch (const std::invalid_argument &err) {
    std::cerr << "Lexer error: " << err.what() << std::endl;
    return DC_LEXER_ERROR;
  }
  return DC_OK;
}
