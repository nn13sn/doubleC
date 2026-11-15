#pragma once
#include <cstdint>

/**
 * Action - main component of the bytecode instruction, it tells what the
 * instruction is supposed to do
 */
enum class Action : uint8_t {
  Push_Value,

  Store_Local,
  Load_Local,

  Add,
  Sub,
  Mul,
  Div,
  Mod,

  Equal,
  NotEqual,
  Greater,
  Less,
  LessEq,
  GreaterEq,

  Cast,

  Neg,
  Not,
  PreIncr,
  PostIncr,
  PreDecr,
  PostDecr,

  Pop,

  Print,
  Read,

  Jump,
  JumpIfFalse,

  DefaultInit,
  ForInit,
  ForCheck,
  ForStep,
  ForEnd,

  PushFunction,
  CallFunction,
  Return,

  Halt
};
