#pragma once

#include <cstdint>
#include <string>
#include <vector>
/// jit value - stores datatype and bit representation of the value
struct JITvalue {
  int64_t datatype;
  int64_t value;
};

/**
 * the vector of the values, which is used by both compiled and interpreted
 * versions of the functions.
 */
inline std::vector<JITvalue> realvalues;

inline std::vector<std::string> realstrings{};
