#pragma once
#include "tokens.h"
#include <array>
#include <cstdint>
#include <string>
#include <string_view>
#include <vector>
class Lexer {
  static constexpr std::array<std::string_view,
                              static_cast<size_t>(Keyword::Invalid)>
      keywords{"if",      "else",  "true",     "false",    "in",     "out",
               "double",  "int",   "char",     "bool",     "string", "while",
               "for",     "break", "continue", "function", "return", "global",
               "dynamic", "const", "jit"}; // the list of all possible keywords
                                           // which are going to be used.
  uint8_t IsKeyword(const std::string_view lexeme);
  std::vector<std::string> Initialcode;
  std::vector<std::vector<Token>> tokens;
  size_t i;
  size_t pos;
  /**
   * reading any element that starts with a letter, later we can identify if
   * its variable name or keyword
   */
  bool isLetter();
  /// reading the constant numbers here
  bool isDigit();
  /// reding constant char and string elements here
  bool isText();
  /// reading operators here, including arithmetic, logical and unary.
  bool isOperator();
  // reading all the separators (mostly brackets) here
  bool isSeparator();
  /// the list of all basic escape sequences here
  char getEscapes(const char &c);
  /**
   * the unexpected end of the line, an example is when a string constant is
   * being defined, but ends unexpectedly without the ending <">
   */
  void unexEnd();
  std::vector<std::string> errors = {};

public:
  /**
   * the main method, the loop goes over every single line and analyzes it,
   * after that an array of tokens is formed
   */
  std::vector<std::vector<Token>> Tokenize();
  /// reading the original file with code and saving it to the array of strings
  void readFile(std::string name);
};
