/**
 * Token structure defined here, the lexer gives a std::vector of these after
 * the it finishes the work
 */
#pragma once
#include <cstdint>
#include <string>

/// all the types of tokens
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

/// all the possible keywords
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
  Break,
  Continue,

  Function,
  Return,

  Global,
  Dynamic,
  Const,
  JIT,
  Invalid
};
/// all the possible separator types
enum class Separator : uint8_t {
  LeftParenthesis,
  RightParenthesis,
  Colon,
  Semicolon,
  LeftSquareBracket,
  RightSquareBracket,
  LeftCurlyBracket,
  RightCurlyBracket,
  Comma,
  Invalid
};

/**
 * the basic token structure here. It stores the type of the token, which are
 * listed above, value stores the index of a specific type to determine the
 * exact element lexeme is the original value, it is also important to store
 * the variable names. LineID and columnID are the original locations of the
 * element, they will be used to print errors in the future.
 */
struct Token {
  TokenType type;
  uint8_t value;
  std::string lexeme;
  size_t lineID;
  size_t columnID;
  Token(TokenType type, const uint8_t &value, std::string lexeme, size_t lineID,
        size_t columnID);
  inline ~Token() = default;
};

/// main constructor of the token
inline Token::Token(TokenType type, const uint8_t &value, std::string lexeme,
                    size_t lineID, size_t columnID) {
  this->type = type;
  this->value = value;
  this->lexeme = lexeme;
  this->lineID = lineID;
  this->columnID = columnID;
}
