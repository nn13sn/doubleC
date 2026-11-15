#pragma once
#include "AST.h"
#include <string>

/// parser error, holds an error and location
struct ParserError {
  std::string err;
  Location loc;
  ParserError(const std::string &error, const Location &location)
      : err(error), loc(location) {};
};
