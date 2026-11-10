#include "environment.h"
#include "JIT_value.h"
#include "runtime_value.h"
#include "vm_error.h"

Environment::Environment(const uint32_t &size, const size_t &funcsize) {
  locals.resize(size);
  realvalues.resize(size);
  functions.resize(funcsize);
}

RuntimeVariable Environment::get(const uint32_t &index) {
  if (index < locals.size()) {

    auto copy = locals[index];
    copy.get().convertToValue();
    return copy;
  }

  return RuntimeValue(Datatype::Invalid, NULL);
}

RuntimeVariable *Environment::getPointer(const uint32_t &index) {
  if (index < locals.size())
    return &locals[index];
  return nullptr;
}

void Environment::set(const uint32_t &index, const RuntimeValue &value,
                      const int32_t &mods) {
  auto cpvalue = value;
  realvalues[index].datatype = static_cast<int64_t>(cpvalue.getType());
  cpvalue.convertToBits();
  realvalues[index].value = std::move(std::get<dc_int>(cpvalue.getData()));
  cpvalue.setData(std::bit_cast<int64_t>(&realvalues[index]));
  if (auto ptr = getPointer(index)) {
    if (mods != 0)
      throw VM_error("Modifiers can be used only with the first declaration");

    ptr->set(cpvalue);

    return;
  }

  RuntimeVariable var{cpvalue, utils::isConst(mods)};

  locals[index] = var;
}

bool Environment::newGlobal(const uint32_t &index, const RuntimeValue &value) {
  if (!getPointer(index)) {
    globals[index] = value;
    return true;
  }
  return false;
}
