#pragma once

#include "JIT_value.h"
#include "datatype.h"
#include "runtime_value.h"

extern "C" {
void ReturnInt(RuntimeValue *result, dc_int value);
void ReturnDouble(RuntimeValue *result, dc_double value);
void ReturnChar(RuntimeValue *result, dc_char value);
void ReturnBool(RuntimeValue *result, dc_bool value);
void ReturnString(RuntimeValue *result, dc_string *value);
void JITPrint(int64_t datatype, int64_t value);
JITvalue JITInput(int64_t datatype);
}
