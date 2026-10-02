#include "VM.h"
#include "callstate.h"
#include "functionenv.h"
#include "runtime_cast.h"
#include "runtime_operations.h"
#include "runtime_streams.h"
#include "vm_error.h"

VM::VM(const uint32_t &size, const size_t &funcsize) {
  env = std::make_shared<Environment>(size, funcsize);
}

template <typename OperationFunc> void VM::BinaryOperation(OperationFunc func) {
  auto right = stack.Pop();
  auto left = stack.Pop();
  stack.Push(func(left, right));
}

template <typename OperationFunc> void VM::UnaryOperation(OperationFunc func) {
  auto value = stack.Pop();
  stack.Push(func(value));
}

signed char VM::evaluate(const Bytecode &code) {

  try {
    while (instruction_number < code.code.size()) {
      const Instruction &instruction = code.code[instruction_number++];
      switch (instruction.action) {
      case Action::Push_Value:
        stack.Push(code.values[instruction.operand]);
        break;
      case Action::Pop:
        stack.Pop();
        break;
      case Action::Add:
        BinaryOperation(RuntimeOperations::Add);
        break;
      case Action::Sub:
        BinaryOperation(RuntimeOperations::Sub);
        break;
      case Action::Mul:
        BinaryOperation(RuntimeOperations::Mul);
        break;
      case Action::Div:
        BinaryOperation(RuntimeOperations::Div);
        break;
      case Action::Mod:
        BinaryOperation(RuntimeOperations::Mod);
        break;
      case Action::Equal:
        BinaryOperation(RuntimeOperations::Equal);
        break;
      case Action::NotEqual:
        BinaryOperation(RuntimeOperations::NotEqual);
        break;
      case Action::Less:
        BinaryOperation(RuntimeOperations::Less);
        break;
      case Action::Greater:
        BinaryOperation(RuntimeOperations::Greater);
        break;
      case Action::LessEq:
        BinaryOperation(RuntimeOperations::LessEq);
        break;
      case Action::GreaterEq:
        BinaryOperation(RuntimeOperations::GreaterEq);
        break;
      case Action::Neg:
        UnaryOperation(RuntimeOperations::Neg);
        break;
      case Action::Not:
        UnaryOperation(RuntimeOperations::Not);
        break;
      case Action::PreIncr:
        stack.Push(
            RuntimeOperations::PreIncr(*env->getPointer(instruction.operand)));
        break;
      case Action::PostIncr:
        stack.Push(
            RuntimeOperations::PostIncr(*env->getPointer(instruction.operand)));
        break;
      case Action::PreDecr:
        stack.Push(
            RuntimeOperations::PreDecr(*env->getPointer(instruction.operand)));
        break;
      case Action::PostDecr:
        stack.Push(
            RuntimeOperations::PostDecr(*env->getPointer(instruction.operand)));
        break;
      case Action::Cast:
        RuntimeCast::Cast(stack.Pop(),
                          static_cast<Datatype>(instruction.operand));
        break;
      case Action::Store_Local:
        env->set(instruction.operand, stack.Top(), 0);
        break;
      case Action::Load_Local:
        stack.Push(env->get(instruction.operand).get());
        break;
      case Action::DefaultInit:
        if (env->getPointer(instruction.operand)->get().getType() ==
            Datatype::Invalid)
          env->set(instruction.operand, RuntimeValue(Datatype::Int, 0), 0);
        break;
      case Action::Read:
        stack.Push(RuntimeStreams::ReadValue(
            static_cast<Datatype>(instruction.operand)));
        break;
      case Action::Print:
        RuntimeStreams::Print(stack.Pop());
        break;
      case Action::JumpIfFalse:
        if (!RuntimeCast::As<bool>(stack.Pop()))
          instruction_number = instruction.operand;
        break;
      case Action::Jump:
        instruction_number = instruction.operand;
        break;
      case Action::ForInit:
        ForStates.push_back(ForState());
        ForStates.back().iterator = instruction.operand >> 2;
        ForStates.back().end = stack.Top();
        if ((instruction.operand & 0b11) == 1)
          ForStates.back().inclusive = false;
        else if ((instruction.operand & 0b11) == 2)
          ForStates.back().inclusive = true;
        else {
          stack.Pop(); // deleting start and the end because forcheck will push
                       // them again
          stack.Pop();
          break;
        }
        BinaryOperation(RuntimeOperations::LessEq);
        RuntimeCast::As<bool>(stack.Pop()) ? ForStates.back().direction = 1
                                           : ForStates.back().direction = 0;
        break;
      case Action::ForCheck:
        stack.Push(env->get(ForStates.back().iterator).get());
        stack.Push(ForStates.back().end);
        if (ForStates.back().direction == 2)
          break;
        else if (ForStates.back().direction == 1) {
          if (ForStates.back().inclusive)
            BinaryOperation(RuntimeOperations::LessEq);
          else
            BinaryOperation(RuntimeOperations::Less);
        } else {
          if (ForStates.back().inclusive)
            BinaryOperation(RuntimeOperations::GreaterEq);
          else
            BinaryOperation(RuntimeOperations::Greater);
        }
        break;
      case Action::ForStep:
        if (ForStates.back().direction == 0)
          RuntimeOperations::PreDecr(
              *env->getPointer(ForStates.back().iterator));
        else
          RuntimeOperations::PreIncr(
              *env->getPointer(ForStates.back().iterator));
        break;
      case Action::ForEnd:
        ForStates.pop_back();
        break;
      case Action::PushFunction:
        env->functions[code.functionsinfo[instruction.operand].slot] =
            code.functionsinfo[instruction.operand];
        break;
      case Action::CallFunction:
        CallStates.push_back(CallState{instruction_number});
        instruction_number = env->functions[instruction.operand].entry;
        break;
      case Action::Return:
        instruction_number = CallStates.back().returnPoint;
        CallStates.pop_back();
        if (instruction.operand == 0)
          stack.Push(RuntimeValue(Datatype::Int, 0));
        break;
      case Action::Halt:
        return VM_OK;
      default:
        throw VM_error("Unknown instruction");
      }
    }
  } catch (const VM_error &err) {
    std::cerr << "\nRuntime error: " << err.what()
              << " at line: " +
                     std::to_string(
                         code.locations[instruction_number - 1].line);
    std::cerr << "; column: " +
                     std::to_string(
                         code.locations[instruction_number - 1].column)
              << '\n';
    return VM_ERROR;
  }
  return VM_OK;
}
