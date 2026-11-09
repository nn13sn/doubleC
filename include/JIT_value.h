#pragma once

#include <cstdint>
#include <vector>
struct JITvalue {
  int64_t datatype;
  int64_t value;
};

inline std::vector<JITvalue> realvalues;
