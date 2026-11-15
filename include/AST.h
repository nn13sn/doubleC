#pragma once
#include "ASTexpressions.h"
#include "datatype.h"
#include "funcparameter.h"
#include "literal.h"
#include "location.h"
#include "operators.h"
#include <cstdint>
#include <iostream>
#include <memory>
#include <string>
#include <variant>
#include <vector>
#define MAX_ERROR_NUMBER 20

/// modifiers bits (a number represents a different bit in binary)
enum Modifiers {
  MOD_NONE = 0,
  MOD_GLOBAL = 1,
  MOD_DYNAMIC = 2,
  MOD_CONST = 4,
  MOD_JIT = 8
};

/// The type of the statement, used to identify the statement later
enum class StmtType {
  Input,
  Output,
  ExpressionStmt,
  IfStatement,
  While,
  For,
  BreakStmt,
  ContinueStmt,
  FunctionStatement,
  ReturnStatement,
  BlockStatement,
  Amount
};
/**
 * statement struct, the parent for all statements. The whole code consists of
 * the statements, which are a different types of instructions in the code.
 * Statements often include expression pointer inside
 */
struct Statement : AST {
  StmtType StatementType;
  int32_t mods = MOD_NONE;
};

/// program - holds all the statements
struct Program : AST {
  std::vector<std::unique_ptr<Statement>> statements;
};

/// input statement
struct Input : Statement {
  std::unique_ptr<Expression> input;
  Datatype InputType = Datatype::Invalid;
};

/// output statement
struct Output : Statement {
  std::unique_ptr<Expression> output;
};

/// independent expression statement
struct ExpressionStmt : Statement {
  std::unique_ptr<Expression> expr;
};

/// if statement
struct IfStatement : Statement {
  std::unique_ptr<Program> Instructions = std::make_unique<Program>();
  std::unique_ptr<Expression> expr;
  std::unique_ptr<IfStatement> elseStatement = nullptr;
};

/// while statement
struct While : Statement {
  std::unique_ptr<Program> Instructions = std::make_unique<Program>();
  std::unique_ptr<Expression> expr;
};

/// for statement
struct For : Statement {
  Variable iterator;
  Operator op;
  std::unique_ptr<Expression> step = nullptr;
  std::unique_ptr<Expression> Initialvalue;
  std::unique_ptr<Expression> Finalvalue;
  std::unique_ptr<Program> Instructions;
};

/// break statement
struct BreakStmt : Statement {};

/// continue statement
struct ContinueStmt : Statement {};

/// function statement
struct FunctionStatement : Statement {
  FunctionStatement() : id(amount++) {}
  inline static uint32_t amount = 0;
  uint32_t id;
  std::string name;
  std::vector<Parameter> params = {};
  std::unique_ptr<Program> Instructions;
};
/// return statement
struct ReturnStatement : Statement {
  std::unique_ptr<Expression> expr;
};

/// block statement
struct BlockStatement : Statement {
  std::unique_ptr<Program> instructions;
};
