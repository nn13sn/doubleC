#pragma once
#include "datatype.h"
#include <variant>
#include <vector>

/// literal - a constant value inside expressions
struct Literal {
  Datatype type;
  std::variant<dc_int, dc_double, dc_char, dc_bool, dc_string,
               std::vector<Literal>>
      data;
};
