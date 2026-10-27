#include "jit_return.h"

extern "C" {
void ReturnInt(RuntimeValue *result, dc_int value) {
  *result = RuntimeValue(Datatype::Int, value);
}

void ReturnDouble(RuntimeValue *result, dc_double value) {
  *result = RuntimeValue(Datatype::Double, value);
}

void ReturnChar(RuntimeValue *result, dc_char value) {
  *result = RuntimeValue(Datatype::Char, value);
}

void ReturnBool(RuntimeValue *result, dc_bool value) {
  *result = RuntimeValue(Datatype::Bool, value);
}

void ReturnString(RuntimeValue *result, dc_string *value) {
  *result = RuntimeValue(Datatype::String, *value);
}
}
