#pragma once
#include "runtime_value.h"
#include "utils.h"
namespace RuntimeStreams {
/**
 * main print function, determines the data type and calls the correct
 * overloaded function for it
 */
void Print(const RuntimeValue &value);
/// set of the functions below is an overload for each data type to be printed
void PrintValue(const dc_int &value);
void PrintValue(const dc_double &value);
void PrintValue(const dc_char &value);
void PrintValue(const dc_bool &value);
void PrintValue(const dc_string &value);
void PrintValue(const auto &);
/// reading the input and converting to the specified data type
RuntimeValue ReadValue(const Datatype &type);
/**
 * function to read char32_t which is not supported by std::cin directly, so
 * std::string has to be applied and then converted to do so
 */
template <typename Type> Type Read() {
  Type value;
  std::cin >> value;
  return value;
}
template <> dc_char Read<dc_char>();
} // namespace RuntimeStreams
