#include "jit_integration.h"
#include "runtime_streams.h"
#include <bit>

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

void JITPrint(int64_t datatype, int64_t value) {
  switch (datatype) {
  case 0:
    RuntimeStreams::PrintValue(value);
    return;
  case 1:
    RuntimeStreams::PrintValue(std::bit_cast<dc_double>(value));
    return;
  case 2:
    RuntimeStreams::PrintValue(static_cast<dc_char>(value));
    return;
  case 3:
    RuntimeStreams::PrintValue(static_cast<dc_bool>(value));
    return;
  case 4:
    RuntimeStreams::PrintValue(*std::bit_cast<dc_string *>(value));
    return;
  }
}

JITvalue JITInput(int64_t datatype) {
  auto a = RuntimeStreams::ReadValue(static_cast<Datatype>(datatype));
  a.convertToBits();
  JITvalue raw{static_cast<int64_t>(a.getType()),
               std::get<int64_t>(a.getData())};
  return raw;
}
}
