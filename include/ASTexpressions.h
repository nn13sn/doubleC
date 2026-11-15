#pragma once
#include "literal.h"
#include "location.h"
#include "operators.h"
#include <memory>
#include <string>

/**
 * AST structure, the parent structure for all AST branches - statements and
 * expressions
 */
struct AST {
  Location location;
  virtual ~AST() = default;
};

/// the type of expression, it is used to determine the type of expression later
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

/// the expression structure, the parent for all expressions.
struct Expression : AST {
  ExprType ExpressionType;
};

/// variable, a part of expressions
struct Variable : Expression {
  std::string name;
  Variable() : id(amount++) {}
  inline static uint32_t amount = 0;
  uint32_t id;
};

/// constant values in expressions
struct exprValue : Expression {
  Literal value;
};

/// function call
struct FunctionCall : Expression {
  FunctionCall() : id(amount++) {}
  inline static uint32_t amount = 0;
  uint32_t id;

  std::string name;
  std::vector<std::unique_ptr<Expression>> parameters = {};
};

/// binary expressions - two operands and the operator
struct Binary : Expression {
  Operator op;
  std::unique_ptr<Expression> right;
  std::unique_ptr<Expression> left;
};

/// Assignment expression
struct Assignment : Expression {
  std::unique_ptr<Expression> right;
  std::unique_ptr<Expression> left;
};

/// Unary expression
struct Unary : Expression {
  Operator op;
  std::unique_ptr<Expression> expr;
};

/// cast expression
struct Cast : Expression {
  Datatype castTo;
  std::unique_ptr<Expression> expr;
};
