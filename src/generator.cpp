#include "generator.h"
#include "AST.h"
#include "utils.h"

size_t Generator::emit(Location location, const Action &action,
                       uint32_t operand) {
  auto index = code.code.size();
  code.code.emplace_back(Instruction(action, std::move(operand)));
  code.locations.push_back(std::move(location));
  return index;
}

void Generator::FinishJump(const size_t &instruction, const size_t &target) {
  code.code[instruction].operand = target;
}

void Generator::GenerateBody(const Program &program) { Generate(program); }

void Generator::GenerateExpression(const Expression &expr) {
  switch (expr.ExpressionType) {
  case ExprType::exprValue:
    code.values.emplace_back(static_cast<const exprValue &>(expr).value);
    emit(expr.location, Action::Push_Value, code.values.size() - 1);
    return;
  case ExprType::Binary: {
    const Binary &binary = static_cast<const Binary &>(expr);
    GenerateExpression(*binary.left);
    GenerateExpression(*binary.right);
    emit(binary.location, utils::getOperatorAction(binary.op));
    return;
  }
  case ExprType::Assignment: {
    const Assignment &assignment = static_cast<const Assignment &>(expr);
    GenerateExpression(*assignment.right);
    emit(assignment.location, Action::Store_Local,
         indexes.slots
             [indexes.IDs[static_cast<const Variable &>(*assignment.left).id]]);
    return;
  }
  case ExprType::Variable:
    emit(expr.location, Action::Load_Local,
         indexes.slots[indexes.IDs[static_cast<const Variable &>(expr).id]]);
    return;
  case ExprType::Unary: {
    const Unary &unary = static_cast<const Unary &>(expr);
    auto op = utils::getOperatorAction(unary.op, false);
    if (op == Action::PreIncr || op == Action::PostIncr ||
        op == Action::PreDecr || op == Action::PostDecr) {
      auto index =
          indexes.slots
              [indexes.IDs[static_cast<const Variable &>(*unary.expr).id]];
      emit(expr.location, op, index);
      return;
    }
    GenerateExpression(*unary.expr);
    emit(expr.location, op);
    return;
  }
  case ExprType::Cast: {
    const auto &cast = static_cast<const Cast &>(expr);
    GenerateExpression(*cast.expr);
    emit(expr.location, Action::Cast, static_cast<uint32_t>(cast.castTo));
    return;
  }
  case ExprType::FunctionCall: {
    const auto &call = static_cast<const FunctionCall &>(expr);
    for (size_t i = 0; i < call.parameters.size(); i++) {
      GenerateExpression(*call.parameters[i]);
    }
    emit(call.location, Action::CallFunction, indexes.CallSlots[call.id]);
  }
  default:
    return;
  }
}

void Generator::GenerateOutput(const Output &stmt) {
  GenerateExpression(*stmt.output);
  emit(stmt.location, Action::Print);
}

void Generator::GenerateInput(const Input &stmt) {
  emit(stmt.location, Action::Read, static_cast<uint32_t>(stmt.InputType));
  emit(stmt.input->location, Action::Store_Local,
       indexes
           .slots[indexes.IDs[static_cast<const Variable &>(*stmt.input).id]]);
}

void Generator::GenerateIf(const IfStatement &stmt) {
  size_t escapejump;
  GenerateExpression(*stmt.expr);
  auto jump = emit(stmt.location, Action::JumpIfFalse);
  GenerateBody(*stmt.Instructions);
  if (stmt.elseStatement)
    escapejump = emit(stmt.elseStatement->location, Action::Jump);
  FinishJump(jump, code.code.size());
  if (stmt.elseStatement) {
    auto &elsestmt = static_cast<const IfStatement &>(*stmt.elseStatement);
    if (elsestmt.expr)
      GenerateIf(elsestmt);
    else
      GenerateBody(*elsestmt.Instructions);
    FinishJump(escapejump, code.code.size());
  }
}

void Generator::GenerateWhile(const While &stmt) {
  size_t loopstart = code.code.size();
  GenerateExpression(*stmt.expr);
  auto jump = emit(stmt.location, Action::JumpIfFalse);
  GenerateBody(*stmt.Instructions);
  emit(stmt.location, Action::Jump, loopstart);
  FinishJump(jump, code.code.size());
}

void Generator::GenerateFor(const For &stmt) {
  if (stmt.Initialvalue) {
    GenerateExpression(*stmt.Initialvalue);
    emit(stmt.location, Action::Store_Local,
         indexes.slots[indexes.IDs[stmt.iterator.id]]);
  } else {
    emit(stmt.location, Action::DefaultInit,
         indexes.slots[indexes.IDs[stmt.iterator.id]]);
    emit(stmt.location, Action::Load_Local,
         indexes.slots[indexes.IDs[stmt.iterator.id]]);
  }

  GenerateExpression(*stmt.Finalvalue);
  if (!utils::isArrow(stmt.op))
    emit(stmt.location, Action::ForInit,
         (indexes.slots[indexes.IDs[stmt.iterator.id]] << 2) | 0);
  else
    emit(stmt.location, Action::ForInit,
         stmt.op == Operator::Arrow
             ? (indexes.slots[indexes.IDs[stmt.iterator.id]] << 2) | 1
             : (indexes.slots[indexes.IDs[stmt.iterator.id]] << 2) | 2);
  auto loopstart = code.code.size();
  emit(stmt.location, Action::ForCheck);
  if (!utils::isArrow(stmt.op))
    emit(stmt.location, utils::getOperatorAction(stmt.op));
  auto jump = emit(stmt.location, Action::JumpIfFalse);
  GenerateBody(*stmt.Instructions);
  if (stmt.step) {
    GenerateExpression(*stmt.step);
    emit(stmt.step->location, Action::Pop);
  } else
    emit(stmt.location, Action::ForStep);
  emit(stmt.location, Action::Jump, loopstart);
  FinishJump(jump, code.code.size());
  emit(stmt.location, Action::ForEnd);
}

void Generator::GenerateFunction(const FunctionStatement &stmt) {
  for (auto it = stmt.params.rbegin(); it != stmt.params.rend(); it++) {
    emit(it->var.location, Action::Store_Local,
         indexes.slots[indexes.IDs[it->var.id]]);
    emit(it->var.location, Action::Pop);
  } // initializing variables in reverse, because the last variable is on the
    // top of the stack
  GenerateBody(*stmt.Instructions);
  emit(stmt.location, Action::Return);
}

void Generator::GenerateReturn(const ReturnStatement &stmt) {
  if (stmt.expr)
    GenerateExpression(*stmt.expr);
  emit(stmt.location, Action::Return, stmt.expr ? 1 : 0);
}

void Generator::Generate(const Program &program) {
  for (const auto &stmt : program.statements) {
    switch (stmt->StatementType) {
    case StmtType::ExpressionStmt:
      GenerateExpression(*static_cast<const ExpressionStmt &>(*stmt).expr);
      emit(stmt->location, Action::Pop);
      break;
    case StmtType::Output:
      GenerateOutput(static_cast<const Output &>(*stmt));
      break;
    case StmtType::Input:
      GenerateInput(static_cast<const Input &>(*stmt));
      emit(stmt->location, Action::Pop);
      break;
    case StmtType::IfStatement:
      GenerateIf(static_cast<const IfStatement &>(*stmt));
      break;
    case StmtType::While:
      GenerateWhile(static_cast<const While &>(*stmt));
      break;
    case StmtType::For:
      GenerateFor(static_cast<const For &>(*stmt));
      break;
    case StmtType::FunctionStatement:
      emit(stmt->location, Action::PushFunction, code.functionsinfo.size());
      code.functionsinfo.push_back(FunctionInfo{0});
      AllFunctions.push_back(&static_cast<const FunctionStatement &>(*stmt));
      break;
    case StmtType::ReturnStatement:
      GenerateReturn(static_cast<const ReturnStatement &>(*stmt));
      break;
    default:
      break;
    }
  }
  return;
}

const Bytecode &Generator::StartGeneration(const Program &program) {
  Generate(program);
  emit(Location(SIZE_MAX, SIZE_MAX), Action::Halt);
  for (size_t i = 0; i < AllFunctions.size(); i++) {
    code.functionsinfo[i].entry = code.code.size();
    code.functionsinfo[i].slot = indexes.FunctionSlots[AllFunctions[i]->id];
    GenerateFunction(*AllFunctions[i]);
  }
  return code;
}
