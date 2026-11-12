#include "runtime_variable.h"
#include "JIT_value.h"
#include "runtime_value.h"
#include "vm_error.h"
bool RuntimeVariable::CheckConst() { return !(isConst || locked); }

void RuntimeVariable::lock() { locked++; }

void RuntimeVariable::unlock() { locked--; }

RuntimeValue &RuntimeVariable::get() { return value; }

void RuntimeVariable::increment() {
  if (isConst)
    throw VM_error("Cannot modify constant");
  if (locked)
    throw VM_error("Cannot modify loop iterator");
  auto adress = std::bit_cast<JITvalue *>(std::get<dc_int>(value.getData()));
  value.convertToValue();
  value.increment();
  value.convertToBits();
  adress->datatype = static_cast<int64_t>(value.getType());
  adress->value = std::move(std::get<dc_int>(value.getData()));
  value.setData(std::bit_cast<int64_t>(adress));
}

void RuntimeVariable::decrement() {
  if (isConst)
    throw VM_error("Cannot modify constant");
  if (locked)
    throw VM_error("Cannot modify loop iterator");
  auto adress = std::bit_cast<JITvalue *>(std::get<dc_int>(value.getData()));
  value.convertToValue();
  value.decrement();
  value.convertToBits();
  adress->datatype = static_cast<int64_t>(value.getType());
  adress->value = std::move(std::get<dc_int>(value.getData()));
  value.setData(std::bit_cast<int64_t>(adress));
}

void RuntimeVariable::set(const RuntimeValue &value) {
  if (isConst)
    throw VM_error("Cannot modify constant");
  if (locked)
    throw VM_error("Cannot modify loop iterator");
  this->value = value;
}

void RuntimeVariable::setData(const Data &data) {
  if (isConst)
    throw VM_error("Cannot modify constant");
  if (locked)
    throw VM_error("Cannot modify loop iterator");
  value.setData(data);
}
