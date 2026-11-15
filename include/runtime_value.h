#pragma once
#include "interpreter_error.h"
#include "literal.h"
#include "utils.h"
#include <string>
#include <variant>
#include <vector>
struct Function {
  const FunctionStatement *declaration;
};

using Data = std::variant<dc_int, dc_double, dc_char, dc_bool, dc_string,
                          std::vector<Literal>, std::shared_ptr<Function>>;

class RuntimeValue {
  Datatype type;
  Data data;
  /// private function that deduces the data type when the data is set
  Datatype deduceType(const Data &data);

public:
  /// constructor from a literal(constant value)
  RuntimeValue(const Literal &value);
  /// constructor for datatype and data itself
  RuntimeValue(const Datatype &type, const Data &data);
  /// default constructor
  RuntimeValue();
  /// increment
  void increment();
  /// decrement
  void decrement();
  /// seting the whole value to another
  void set(const RuntimeValue &var);
  /// seting the data only
  void setData(const Data &data);
  /**
   * converting the data to bits, so it can be stored in rawvalues (array) in a
   * bit representation
   */
  void convertToBits();
  /// converting the value from the bit representation to the original value
  void convertToValue();
  /// getting the const reference to the data
  const Data &getData() const;
  /// getting the const reference to the data type
  const Datatype &getType() const;
};
