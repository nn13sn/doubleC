#pragma once
#include "runtime_value.h"
#include "runtime_variable.h"

namespace RuntimeOperations {
/// arithmetical addition
RuntimeValue Add(const RuntimeValue &left, const RuntimeValue &right);
/// arithmetical subtraction
RuntimeValue Sub(const RuntimeValue &left, const RuntimeValue &right);
/// arithmetical multiplication
RuntimeValue Mul(const RuntimeValue &left, const RuntimeValue &right);
/// arithmetical division
RuntimeValue Div(const RuntimeValue &left, const RuntimeValue &right);
/// arithmetical mod operation
RuntimeValue Mod(const RuntimeValue &left, const RuntimeValue &right);

/// logical equal(==)
RuntimeValue Equal(const RuntimeValue &left, const RuntimeValue &right);
/// logical not equal(!=)
RuntimeValue NotEqual(const RuntimeValue &left, const RuntimeValue &right);
/// logical greater(>)
RuntimeValue Greater(const RuntimeValue &left, const RuntimeValue &right);
/// logical less(<)
RuntimeValue Less(const RuntimeValue &left, const RuntimeValue &right);
/// logical greater or equal (>=)
RuntimeValue GreaterEq(const RuntimeValue &left, const RuntimeValue &right);
/// logical less or equal (<=)
RuntimeValue LessEq(const RuntimeValue &left, const RuntimeValue &right);

/// unary minus (-)
RuntimeValue Neg(RuntimeValue value);
/// unary not (!)
RuntimeValue Not(const RuntimeValue &value);
/// unary pre-increment
RuntimeValue PreIncr(RuntimeVariable &value);
/// unary post-increment
RuntimeValue PostIncr(RuntimeVariable &value);
/// unary pre-decrement
RuntimeValue PreDecr(RuntimeVariable &value);
/// unary post-decrement
RuntimeValue PostDecr(RuntimeVariable &value);
} // namespace RuntimeOperations
