#include "interpreter.h"

Value Interpreter::eval(const Expression& expr){
  if(auto a = dynamic_cast<const exprValue*> (&expr)) return a->value;
  else if (auto a = dynamic_cast<const Variable*> (&expr)) {
    if(auto b = findVar(a->name)) return *b;
    else throw std::invalid_argument("Error: No such variable.");
  }
  else if (auto a = dynamic_cast<const Logical*> (&expr)){
    if(a->op == ">") return evalGr(eval(*(a->left)), eval(*(a -> right))); 
    else if(a->op == "<") return evalLs(eval(*(a->left)), eval(*(a -> right)));
    else if(a->op == "==") return evalEq(eval(*(a->left)), eval(*(a -> right)));
    else if(a->op == "!=") return evalNq(eval(*(a->left)), eval(*(a -> right)));
    else if(a->op == ">=") return evalGe(eval(*(a->left)), eval(*(a -> right)));
    else if(a->op == "<=") return evalLe(eval(*(a->left)), eval(*(a -> right)));
    else throw std::invalid_argument("Error: Invalid logical operator.");
  }
  else if (auto a = dynamic_cast<const Binary*> (&expr)) {
    switch(a->op){
      case '+':
        return evalAdd(eval(*(a->left)), eval(*(a->right)));
        break;
      case '-':
        return evalSub(eval(*(a->left)), eval(*(a->right)));
        break;
      case '*':
        return evalMul(eval(*(a->left)), eval(*(a->right)));
        break;
      case '/':
        return evalDiv(eval(*(a->left)), eval(*(a->right)));
        break;
      case '%':
        return evalMod(eval(*(a->left)), eval(*(a->right)));
        break;
      default:
        throw std::invalid_argument("Error: No such operator.");
    }
  }
}

bool Interpreter::isNumeric(const Value& value){
  if(value.type == Datatype::Int || value.type == Datatype::Char || value.type == Datatype::Double || value.type == Datatype::Bool) return true;
  return false;
}

double Interpreter::toDouble(const Value& value){
  switch(value.type){
    case Datatype::Int:
      return std::get<int64_t> (value.data);
    case Datatype::Double:
      return std::get<double> (value.data);
    case Datatype::Char:
      return static_cast<unsigned char> (std::get<char> (value.data));
    case Datatype::Bool:
      return std::get<bool> (value.data) ? 1.0 : 0.0;
    default:
      throw std::invalid_argument("Error: Not a numeric type");
  }
}

int64_t Interpreter::toInt(const Value& value){
  switch(value.type){
    case Datatype::Int:
      return std::get<int64_t> (value.data);
    case Datatype::Double:
      return static_cast<int64_t> (std::round(std::get<double> (value.data)));
    case Datatype::Char:
      return std::get<char> (value.data);
    case Datatype::Bool:
      return std::get<bool> (value.data);
    default:
      throw std::invalid_argument("Error: Not a numeric type");
  }
}

Value Interpreter::evalAdd(const Value& left, const Value& right){
  if(!isNumeric(left) || !isNumeric(right)) throw std::invalid_argument("Error: Operator \"+\" cannot be used to such value type.");
  if(left.type == Datatype::Double || right.type == Datatype::Double) return {Datatype::Double, toDouble(left) + toDouble(right)};
  return {Datatype::Int, toInt(left) + toInt(right)};
}

Value Interpreter::evalSub(const Value& left, const Value& right){
  if(!isNumeric(left) || !isNumeric(right)) throw std::invalid_argument("Error: Operator \"-\" cannot be used to such value type.");
  if(left.type == Datatype::Double || right.type == Datatype::Double) return {Datatype::Double, toDouble(left) - toDouble(right)};
  return {Datatype::Int, toInt(left) - toInt(right)};
}

Value Interpreter::evalMul(const Value& left, const Value& right){
  if(!isNumeric(left) || !isNumeric(right)) throw std::invalid_argument("Error: Operator \"*\" cannot be used to such value type.");
  if(left.type == Datatype::Double || right.type == Datatype::Double) return {Datatype::Double, toDouble(left) * toDouble(right)};
  return {Datatype::Int, toInt(left) * toInt(right)};
}

Value Interpreter::evalDiv(const Value& left, const Value& right){
  if(!isNumeric(left) || !isNumeric(right)) throw std::invalid_argument("Error: Operator \"/\" cannot be used to such value type.");
  return {Datatype::Double, toDouble(left) / toDouble(right)};
}

Value Interpreter::evalMod(const Value& left, const Value& right){
  if(left.type != Datatype::Int || right.type != Datatype::Int) throw std::invalid_argument("Error: Operator \"%\" cannot be used to such value type.");
  return {Datatype::Int, toInt(left) % toInt(right)};
}

Value Interpreter::evalGr(const Value& left, const Value& right){
  if(!isNumeric(left) || !isNumeric(right)) throw std::invalid_argument("Error: Operator \">\" cannot be used to such value type.");
  if(left.type == Datatype::Double || right.type == Datatype::Double) return {Datatype::Bool, toDouble(left) > toDouble(right)};
  return {Datatype::Bool, toInt(left) > toInt(right)};
}

Value Interpreter::evalLs(const Value& left, const Value& right){
  if(!isNumeric(left) || !isNumeric(right)) throw std::invalid_argument("Error: Operator \"<\" cannot be used to such value type.");
  if(left.type == Datatype::Double || right.type == Datatype::Double) return {Datatype::Bool, toDouble(left) < toDouble(right)};
  return {Datatype::Bool, toInt(left) < toInt(right)};
}

Value Interpreter::evalGe(const Value& left, const Value& right){
  if(!isNumeric(left) || !isNumeric(right)) throw std::invalid_argument("Error: Operator \">=\" cannot be used to such value type.");
  if(left.type == Datatype::Double || right.type == Datatype::Double) return {Datatype::Bool, toDouble(left) >= toDouble(right)};
  return {Datatype::Bool, toInt(left) >= toInt(right)};
}

Value Interpreter::evalLe(const Value& left, const Value& right){
  if(!isNumeric(left) || !isNumeric(right)) throw std::invalid_argument("Error: Operator \"<=\" cannot be used to such value type.");
  if(left.type == Datatype::Double || right.type == Datatype::Double) return {Datatype::Bool, toDouble(left) <= toDouble(right)};
  return {Datatype::Bool, toInt(left) <= toInt(right)};
}

Value Interpreter::evalEq(const Value& left, const Value& right){
  if(!isNumeric(left) || !isNumeric(right)) throw std::invalid_argument("Error: Operator \"==\" cannot be used to such value type.");
  if(left.type == Datatype::Double || right.type == Datatype::Double) return {Datatype::Bool, toDouble(left) == toDouble(right)};
  return {Datatype::Bool, toInt(left) == toInt(right)};
}

Value Interpreter::evalNq(const Value& left, const Value& right){
  if(!isNumeric(left) || !isNumeric(right)) throw std::invalid_argument("Error: Operator \"!=\" cannot be used to such value type.");
  if(left.type == Datatype::Double || right.type == Datatype::Double) return {Datatype::Bool, toDouble(left) != toDouble(right)};
  return {Datatype::Bool, toInt(left) != toInt(right)};
}

bool Interpreter::isTrue(const Value& value){
  switch (value.type){
    case Datatype::Int:
      return std::get <int64_t> (value.data) != 0;
    case Datatype::Char:
      return std::get <char> (value.data) != '\0';
    case Datatype::String:
      return !std::get <std::string>(value.data).empty();
    case Datatype::Double:
      return std::get <double> (value.data) != 0;
    case Datatype::Bool:
      return std::get <bool> (value.data);
    case Datatype::Array:
      return !std::get <std::vector <Value>>(value.data).empty();
  }
}

Value* Interpreter::findVar(const std::string& name){
  for (auto i = variables.rbegin(); i != variables.rend();i++){
    auto found = i->find(name);
    if(found != i->end()) return &found -> second;
  }
  return nullptr;
}

void Interpreter::definition(const Definition& stmt){
  variables.back()[stmt.name] = eval(*stmt.value);
}


void Interpreter::output(const Output& stmt){
  Value value = eval(*stmt.output);
  switch(value.type){
    case Datatype::Int:
      std::cout<<std::get<int64_t>(value.data);
      break;
    case Datatype::Double:
      std::cout<<std::get<double>(value.data);
      break;
    case Datatype::Char:
      std::cout<<std::get<char>(value.data);
      break;
    case Datatype::Bool:
      std::cout<<std::get<bool> (value.data);
      break;
    case Datatype::String:
      std::cout<<std::get<std::string>(value.data);
      break;
    default:
      throw std::invalid_argument("Cannot be printed.");
  }
}

void Interpreter::ifStatement(const IfStatement& stmt){
  if(isTrue(eval(*stmt.expr))) {
    variables.push_back({});
    for(size_t i = 0; i < stmt.Instructions->statements.size(); i++){
      matchStatement(*stmt.Instructions->statements[i]);
    }
  variables.pop_back();
  }
  else if(stmt.elseStatement){
     if(stmt.elseStatement->expr) ifStatement(*stmt.elseStatement);
     else {
       variables.push_back({});
       for(size_t i = 0; i < stmt.elseStatement->Instructions->statements.size(); i++){
         matchStatement(*stmt.elseStatement->Instructions->statements[i]);
         }
       variables.pop_back();
     }
  } 
}

void Interpreter::matchStatement(const Statement& stmt){
  if (auto a = dynamic_cast<const Output*> (&stmt)) output(*a);
  else if (auto a = dynamic_cast<const Definition*> (&stmt)) definition(*a);
  else if (auto a = dynamic_cast<const IfStatement*> (&stmt)) ifStatement(*a);
}

void Interpreter::execute(const Program& program){
  try{
    variables.push_back({});
    for(size_t i = 0; i < program.statements.size(); i++){
      matchStatement(*program.statements[i]);
    }
  }
  catch(const std::invalid_argument& e){
    std::cerr << e.what()<<std::endl;
  }
}
