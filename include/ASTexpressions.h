#pragma once
#include "literal.h"
#include "location.h"
#include "operators.h"
#include <memory>
#include <string>
struct AST {
  Location location;
  virtual ~AST() = default;
};

enum class ExprType {
  exprValue,
  Variable,
  FunctionCall,
  Binary,
  Assignment,
  Unary,
  Cast,
  Amount
};

struct Expression : AST {
  ExprType ExpressionType;
};

struct Variable : Expression {
  std::string name;
  Variable() : id(amount++) {}
  inline static uint32_t amount = 0;
  uint32_t id;
};

struct exprValue : Expression {
  Literal value;
};

struct FunctionCall : Expression {
  FunctionCall() : id(amount++) {}
  inline static uint32_t amount = 0;
  uint32_t id;

  std::string name;
  std::vector<std::unique_ptr<Expression>> parameters = {};
};

struct Binary : Expression {
  Operator op;
  std::unique_ptr<Expression> right;
  std::unique_ptr<Expression> left;
};

struct Assignment : Expression {
  std::unique_ptr<Expression> right;
  std::unique_ptr<Expression> left;
};

struct Unary : Expression {
  Operator op;
  std::unique_ptr<Expression> expr;
};

struct Cast : Expression {
  Datatype castTo;
  std::unique_ptr<Expression> expr;
};
