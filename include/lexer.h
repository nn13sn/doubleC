#pragma once
#include <iostream>
#include <string>
#include <vector>
#include <array>
#include <string_view>
#include <cctype>
#include <cstdint>
#include <stdexcept>
#include <fstream>
#include "AST.h"

enum class TokenType : uint8_t {
    Identifier,
    Keyword,
    Number,
    Double,
    Symbol, 
    Boolean,
    String,
    Operator,
    Separator,
    End,
    Invalid
};

enum class Keyword : uint8_t {
    If,
    Else, 
    True,
    False,
    In,
    Out,
    Double,
    Int, 
    Char,
    Bool,
    String,
    While,
    For,
    Invalid
};

struct Token{
    TokenType type;
    uint8_t value;
    std::string lexeme;
    size_t lineID;
    size_t columnID;
    Token(TokenType type, const uint8_t& value, std::string lexeme,size_t lineID, size_t columnID);
};

class Lexer{
  static constexpr std::array <std::string_view, static_cast <size_t> (Keyword::Invalid)> keywords {
        "if", "else", "true", "false", "in", "out","double", "int", "char", "bool", "string", "while", "for"
  };
  uint8_t IsKeyword(const std::string_view lexeme);
  std::vector <std::string> Initialcode;
  std::vector <std::vector <Token>> tokens;
  size_t i;
  size_t pos;
  bool isLetter();
  bool isDigit();
  bool isText();
  bool isOperator();
  bool isSeparator();
  char getEscapes(const char& c);
  void unexEnd();
  public:
  std::vector <std::vector <Token>> Tokenize();
  void readFile(std::string name);
};

