#pragma once
#include "runtime_value.h"
class RuntimeVariable {
  RuntimeValue value;
  bool isConst = false;
  int32_t locked = 0;

public:
  /// checks if the value is const
  bool CheckConst();
  /// locks the value (const for some amount of time)
  void lock();
  /// unlocks the value
  void unlock();
  /// reference to the value
  RuntimeValue &get();
  /// increment
  void increment();
  /// decrement
  void decrement();
  /// sets the value
  void set(const RuntimeValue &value);
  /// sets the data of the value
  void setData(const Data &data);
  /// constructor (copy constructor)
  RuntimeVariable(const RuntimeValue &val, const bool &isconst = false)
      : value(val), isConst(isconst) {};
  /// default constructor
  RuntimeVariable() {};
};
