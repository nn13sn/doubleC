#pragma once
#include "datatype.h"
#include "runtime_value.h"

namespace RuntimeCast {
/// helper function to create bitmasks
constexpr uint32_t getBit(const Datatype &type) {
  return 1u << static_cast<uint32_t>(type);
}
/// all the bitmasks
constexpr uint32_t AllowedCasts[] = {
    // int
    getBit(Datatype::Int) | getBit(Datatype::Double) | getBit(Datatype::Char) |
        getBit(Datatype::Bool),

    // double
    getBit(Datatype::Int) | getBit(Datatype::Double) | getBit(Datatype::Char) |
        getBit(Datatype::Bool),

    // char
    getBit(Datatype::Int) | getBit(Datatype::Double) | getBit(Datatype::Char) |
        getBit(Datatype::Bool),

    // bool
    getBit(Datatype::Int) | getBit(Datatype::Double) | getBit(Datatype::Char) |
        getBit(Datatype::Bool),

    // string
    getBit(Datatype::String),

    // array
    getBit(Datatype::Array),

    // function
    getBit(Datatype::Function)};

/// helper function that determines if one value can be casted to another
bool CanCast(const Datatype &from, const Datatype &to);
/**
 * for arithmetical operations common type is required, it is always integer
 * unless the expression has at least one double value
 */
Datatype CommonType(const Datatype &left, const Datatype &right);
/**
 * main function, applies a template function that is overloaded for every need
 * cast type
 */
RuntimeValue Cast(const RuntimeValue &value, const Datatype &to);
// template
template <typename ReturnType> ReturnType As(const RuntimeValue &value);
/// overload for int cast
template <> dc_int As<dc_int>(const RuntimeValue &value);
/// overload for double cast
template <> dc_double As<dc_double>(const RuntimeValue &value);
/// overloaad for bool cast
template <> dc_bool As<dc_bool>(const RuntimeValue &value);
/// overload for chat cast
template <> dc_char As<dc_char>(const RuntimeValue &value);
/// overload for string cast
template <> dc_string As<dc_string>(const RuntimeValue &value);
} // namespace RuntimeCast
