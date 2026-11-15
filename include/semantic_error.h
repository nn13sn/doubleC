#pragma once
#include "AST.h"
#include <string>

/**
 * A semantic error - catched in the analyzer. That is the most popular type
 * of errors, like using undefined variables, undefined functions and etc.
 */
struct SemanticError {
  Location loc;
  std::string err;
  SemanticError(const std::string &err, const Location &loc);
};

inline SemanticError::SemanticError(const std::string &err,
                                    const Location &loc) {
  this->err = err;
  this->loc = loc;
}
