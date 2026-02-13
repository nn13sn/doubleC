#pragma once
#include "AST.h"
#include <iostream>
#include <string>
#include <map>
#include <unordered_map>
#include <cmath>

struct Interpreter{
  std::vector<std::unordered_map <std::string, Value>> variables;
  Value* findVar(const std::string& name);
  void execute(const Program& program);
  void matchStatement(const Statement& stmt);
  void declaration(const Declaration& stmt);
  void output(const Output& stmt);
  void definition(const Definition& stmt);
  Value eval(const Expression& expr);
  void ifStatement(const IfStatement& stmt);
  double toDouble(const Value& value);
  int64_t toInt(const Value& value);
  bool isNumeric(const Value& value);
  bool isTrue (const Value& value);
  Value evalAdd(const Value& left, const Value& right);
  Value evalSub(const Value& left, const Value& right);
  Value evalMul(const Value& left, const Value& right);
  Value evalDiv(const Value& left, const Value& right);
  Value evalMod(const Value& left, const Value& right);
  Value evalGr(const Value& left, const Value& right);
  Value evalLs(const Value& left, const Value& right);
  Value evalEq(const Value& left, const Value& right);
  Value evalNq(const Value& left, const Value& right);
  Value evalGe(const Value& left, const Value& right);
  Value evalLe(const Value& left, const Value& right);
};
