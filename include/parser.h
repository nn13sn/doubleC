#pragma once
#include "AST.h"
#include "lexer.h"
#include "parser_error.h"
#include <algorithm>
#include <cstddef>
#include <iostream>
#include <stdexcept>
#define OPENPARENTHESIS "Expected \"(\""
#define CLOSEPARENTHESIS "Expected \")\""
#define OPENCURLYBRACKET "Expected \"{\""
#define PARSER_OK 1
#define PARSER_ERROR -1

class Parser {
  // a giant class that converts tokens into AST
private:
  size_t line = 0;
  size_t pos = 0;
  /**
   * error throw, the function wraps the error with the data about the error's
   * original location
   */
  void SyntaxErr(const std::string &err);
  /// a short function that returns the current token
  const Token &peek() const;
  /// returns the next token, and increments the position
  const Token &advance();
  /// return the next token, but without incrementing the position
  const Token &peekNext();
  /**
   * getting to the bracket, the method is used by the statements that contain
   * blocks
   */
  bool getToBracket();
  std::vector<std::vector<Token>> &tokens;
  std::vector<ParserError> errors = {};
  /// matches every line with it's possible logical meaning(AST)
  std::unique_ptr<Statement> MakeStatement();
  /// parsing the input made by the user
  std::unique_ptr<Statement> ParseInput();
  /// parsing output statement
  std::unique_ptr<Statement> ParseOutput();
  /// parsing independent expressions, like function calls for example
  std::unique_ptr<Statement> ParseExpression();
  /// parsing a block of instructions
  std::unique_ptr<Statement> ParseBlock();
  /// parsing if and else statement
  std::unique_ptr<Statement> ParseIfStatement();
  /// parsing while loop
  std::unique_ptr<Statement> ParseWhile();
  /// parsing for loop
  std::unique_ptr<Statement> ParseFor();
  /// parsing break statement (used to exit the loop)
  std::unique_ptr<Statement> ParseBreak();
  /**
   * parsing continue statement (used the skip the rest of the instructions in
   * the current iteration)
   */
  std::unique_ptr<Statement> ParseContinue();
  /// parsing the function definition
  std::unique_ptr<Statement> ParseFunction();
  /// parsing return statement, which returns the value from the function
  std::unique_ptr<Statement> ParseReturn();
  /// parsing modifiers, which would then be applied the following statement
  std::unique_ptr<Statement> ParseModifiers();
  /**
   * parsing the expression, the define operator is the last one to execute,
   * thats why it is the top one
   */
  std::unique_ptr<Expression> MakeExpression();
  /// the second last element of the expression to execute
  std::unique_ptr<Expression> OrParse();
  /**
   * functions below - different layers of the expression, the single one is
   * the last one, or the first one to execute
   */
  std::unique_ptr<Expression> AndParse();
  std::unique_ptr<Expression> UnaryParse();
  std::unique_ptr<Expression> ParseTerm();
  std::unique_ptr<Expression> ParseMidTerm();
  std::unique_ptr<Expression> LogicalParse();
  /**
   * I made this method to avoid the code repetion
   * when parsing each level
   */
  template <typename LowFunc>
  std::unique_ptr<Expression> ParseBinary(LowFunc ParseLower,
                                          const std::vector<Operator> &ops);
  /**
   * parses single elements of the expression, such as constants, variables,
   * function calls and casts
   */
  std::unique_ptr<Expression> SingleParse();

  /// making a block of statements
  std::unique_ptr<Program> MakeBody();
  /**
   * a set of overloaded check functions, they are used to match a specific
   * token type, or a specific value of the token
   */
  bool Check(TokenType type);
  bool Check(std::string lexeme);
  bool Check(Keyword keyword);
  bool Check(Operator op);
  bool Check(Separator sep);
  /// simple function to identify the end
  bool isEnd();
  /// moving to the next line
  bool eatEnd();
  /// simple function that returns the datatype of a constant
  Datatype getDatatype();
  // used to identify the data type in casts from one datatype to another
  Datatype getDatatype(const Keyword &keyword);
  /**
   * returns the data itself, because the language has a dynamic typization,
   * the values are stored in std::variant
   */
  std::variant<dc_int, dc_double, dc_char, dc_bool, dc_string,
               std::vector<Literal>>
  getData();
  /**
   * adding the modifiers, the mods is a bitwise representation of which
   * modificators are applied
   */
  void addModifier(int32_t &mods, const Keyword &keywrd);

public:
  /// basic constructor
  Parser(std::vector<std::vector<Token>> &T);
  /// the main method, the loop goes over the tokens vector
  signed char Parse(Program &program);
  /**
   * prints all the errors which were catched by the parser, the parser was
   * supposed to return Parser_error in that case
   */
  void printErrors();
};
