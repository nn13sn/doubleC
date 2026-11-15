#pragma once
#include "AST.h"
#include "analyzer_env.h"
#include "semantic_error.h"
#include "slot_table.h"
#define AnalyzerOK 0
#define AnalyzerError -1

class Analyzer {
  // an analyzer classes, which most of errors catched here
  std::vector<SemanticError> errors = {};
  std::vector<uint32_t> Scopestats;
  AnalyzerEnv *env = new AnalyzerEnv(&table);
  bool insidefunction = false;
  bool insideLoop = false;
  int32_t currentmodifiers = 0;
  bool ignoreVariables = false;
  /// this method analyzes every expression type
  void AnalyzeExpression(const Expression &expr);
  /// Analyzing binary expressions, left side is always first
  void AnalyzeBinary(const Binary &expr);
  /**
   * analyzing variable assignment, right side should be analyzed first, but I
   * do the other way to make sure variable slots will get assigned correctly
   */
  void AnalyzeAssignment(const Assignment &expr);
  /// analyzing unary operators
  void AnalyzeUnary(const Unary &expr);
  /// analyzing input
  void AnalyzeInput(const Input &stmt);
  /// analyzing output
  void AnalyzeOutput(const Output &stmt);
  /// analyzing independent expressions
  void AnalyzeExpressionStmt(const ExpressionStmt &stmt);
  /// analyzing while loop
  void AnalyzeWhile(const While &stmt);
  /// analyzing for loop
  void AnalyzeFor(const For &stmt);
  /// analyzing a block of instructions
  void AnalyzeBreak(const BreakStmt &stmt);
  /// analyzing continue statement
  void AnalyzeContinue(const ContinueStmt &stmt);
  /// analyzing if/else statement
  void AnalyzeIf(const IfStatement &stmt);
  /// analyzing function declaration
  void AnalyzeFunction(const FunctionStatement &stmt);
  /// analyzing return statement
  void AnalyzeReturn(const ReturnStatement &stmt);
  /// analyzing block statement
  void AnalyzeBlock(const BlockStatement &stmt);
  /**
   * adding new scope to know which variables are visible and etc. Also used to
   * optimize the variables slot usage
   */
  void newScope();
  /// removes the scope
  void removeScope();

public:
  /// print Semantic errors
  void printErrors();
  /// the main method, goes through the whole ast and matches each statement
  signed char analyze(const Program &program);
  Slot_Table table;
  ~Analyzer() { delete env; }
};
