#pragma once

#include "JIT_value.h"
#include "datatype.h"
#include "jitforstate.h"
#include "runtime_value.h"
// all the functions below are the assembly integration with c++
extern "C" {
/// function to call if the return datatype is int
void ReturnInt(RuntimeValue *result, dc_int value);
/// function to call if the return data type is double
void ReturnDouble(RuntimeValue *result, dc_double value);
/// function to call if the return data type is char
void ReturnChar(RuntimeValue *result, dc_char value);
/// function to call if the return data type is bool
void ReturnBool(RuntimeValue *result, dc_bool value);
/// function to call if the return data type is string
void ReturnString(RuntimeValue *result, dc_string *value);
/// output function, std::cout basically
void JITPrint(int64_t datatype, int64_t value);
/// input function, std::cin basically
JITvalue JITInput(int64_t datatype);
/// function that adds a scope to the forstate(used for inclusive loops)
JITForState *PushFor();
/// function that removes a scope from the forstate
JITForState *PopFor();
}
