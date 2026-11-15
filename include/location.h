#pragma once
#include <cstddef>
/// the location structure, a basic structure that holds the line and column
struct Location {
  size_t column = 0;
  size_t line;
  Location() {};
  Location(const size_t &col, const size_t &lin) : column(col), line(lin) {};
};
