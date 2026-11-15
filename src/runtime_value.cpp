#include "runtime_value.h"
#include "JIT_value.h"
#include "vm_error.h"
RuntimeValue::RuntimeValue(const Literal &value) {
  type = value.type;
  std::visit([this](const auto &data) { this->data = data; }, value.data);
}

RuntimeValue::RuntimeValue(const Datatype &type, const Data &data) {
  this->type = type;
  this->data = data;
}

RuntimeValue::RuntimeValue() {
  type = Datatype::Invalid;
  data = NULL;
}

Datatype RuntimeValue::deduceType(const Data &data) {
  return std::visit(
      [](auto &&a) -> Datatype {
        using T = std::decay_t<decltype(a)>;

        if constexpr (std::is_same_v<T, dc_int>)
          return Datatype::Int;
        else if constexpr (std::is_same_v<T, dc_char>)
          return Datatype::Char;
        else if constexpr (std::is_same_v<T, dc_double>)
          return Datatype::Double;
        else if constexpr (std::is_same_v<T, dc_bool>)
          return Datatype::Bool;
        else if constexpr (std::is_same_v<T, dc_string>)
          return Datatype::String;
        else if constexpr (std::is_same_v<T, std::vector<Literal>>)
          return Datatype::Array;
        else if constexpr (std::is_same_v<T, std::shared_ptr<Function>>)
          return Datatype::Function;
        else
          return Datatype::Invalid;
      },
      data);
}

void RuntimeValue::increment() {
  if (utils::isNumerical(*this)) {
    std::visit(
        [](auto &c) {
          using T = std::decay_t<decltype(c)>;
          if constexpr (std::is_same_v<T, dc_int> ||
                        std::is_same_v<T, dc_double> ||
                        std::is_same_v<T, dc_char>) {
            c = c + 1;
          }
        },
        this->data);
  } else
    throw VM_error("The increment operator cannot be used to such value type");
}

void RuntimeValue::decrement() {
  if (utils::isNumerical(*this)) {
    std::visit(
        [](auto &c) {
          using T = std::decay_t<decltype(c)>;
          if constexpr (std::is_same_v<T, dc_int> ||
                        std::is_same_v<T, dc_double> ||
                        std::is_same_v<T, dc_char>) {
            c = c - 1;
          }
        },
        this->data);
  } else
    throw VM_error("The increment operator cannot be used to such value type");
}

void RuntimeValue::set(const RuntimeValue &var) { *this = var; }

void RuntimeValue::setData(const Data &data) {
  this->data = data;
  type = deduceType(data);
}

const Data &RuntimeValue::getData() const { return this->data; }

const Datatype &RuntimeValue::getType() const { return this->type; }

void RuntimeValue::convertToBits() {
  switch (this->type) {
  case Datatype::Int:
    break;
  case Datatype::Double:
    this->data = std::bit_cast<int64_t>(std::get<dc_double>(this->data));
    break;
  case Datatype::Char:
    this->data = static_cast<int64_t>(std::get<dc_char>(this->data));
    break;
  case Datatype::Bool:
    this->data = static_cast<int64_t>(std::get<dc_bool>(this->data));
    break;
  case Datatype::String:
    this->data = std::bit_cast<int64_t>(&std::get<dc_string>(this->data));
    break;
  default:
    break;
  }
}

void RuntimeValue::convertToValue() {
  auto adress = std::bit_cast<JITvalue *>(std::get<dc_int>(this->data));

  this->type = static_cast<Datatype>(adress->datatype);
  switch (this->type) {
  case Datatype::Int:
    this->data = adress->value;
    break;
  case Datatype::Double:
    this->data = std::bit_cast<dc_double>(adress->value);
    break;
  case Datatype::Char:
    this->data = static_cast<dc_char>(adress->value);
    break;
  case Datatype::Bool:
    this->data = static_cast<dc_bool>(adress->value);
    break;
  case Datatype::String:
    this->data = *std::bit_cast<dc_string *>(adress->value);
    break;
  default:
    break;
  }
}
