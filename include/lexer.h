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

enum class TokenType{
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
    amount
};

struct Token{
    TokenType type;
    Keyword keyword;
    std::string lexeme;
    size_t lineID;
    size_t columnID;
    Token(TokenType type, const Keyword& keyword, std::string lexeme,size_t lineID, size_t columnID);
};

class Lexer{
  static constexpr std::array <std::string_view, static_cast <size_t> (Keyword::amount)> keywords {
        "if", "else", "true", "false", "in", "out","double", "int", "char", "bool", "string", "while", "for"
  };
  Keyword IsKeyword(const std::string_view lexeme);
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

