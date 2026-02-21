#include "interpreter.h"

interpreter_error::interpreter_error(const std::string msg, size_t line, size_t column) : std::runtime_error(msg){
  location.line = line;
  location.column = column;
}

Value Interpreter::eval(const Expression& expr){
  if(auto a = dynamic_cast<const exprValue*> (&expr)) return a->value;
  else if (auto a = dynamic_cast<const Variable*> (&expr)) {
    if(auto b = findVar(a->name)) return *b;
    else throw interpreter_error("No such variable seems to be defined", a->location.line, a->location.column);
  }
  else if (auto a = dynamic_cast<const Logical*> (&expr)){
    try{
    if(a->op == ">") return evalGr(eval(*(a->left)), eval(*(a -> right))); 
    else if(a->op == "<") return evalLs(eval(*(a->left)), eval(*(a -> right)));
    else if(a->op == "==") return evalEq(eval(*(a->left)), eval(*(a -> right)));
    else if(a->op == "!=") return evalNq(eval(*(a->left)), eval(*(a -> right)));
    else if(a->op == ">=") return evalGe(eval(*(a->left)), eval(*(a -> right)));
    else if(a->op == "<=") return evalLe(eval(*(a->left)), eval(*(a -> right)));
    else throw std::runtime_error("Invalid logical operator");
    }
    catch(const std::runtime_error& err){
      throw interpreter_error(err.what(), a->location.line, a->location.column);
    }
  }
  else if (auto a = dynamic_cast<const Binary*> (&expr)) {
    try{
    switch(a->op){
      case '+':
        return evalAdd(eval(*(a->left)), eval(*(a->right)));
      case '-':
        return evalSub(eval(*(a->left)), eval(*(a->right)));
      case '*':
        return evalMul(eval(*(a->left)), eval(*(a->right)));
      case '/':
        return evalDiv(eval(*(a->left)), eval(*(a->right)));
      case '%':
        return evalMod(eval(*(a->left)), eval(*(a->right)));
      default:
        throw std::runtime_error("Invalid operator");
    }
    }
    catch(const std::runtime_error& err){
      throw interpreter_error(err.what(), a->location.line, a->location.column);
    }
  }
  else if (auto a = dynamic_cast<const Cast*> (&expr)){
    auto b = eval(*a->expr);
    if(b.type == Datatype::String) return convertString(*a);
    try{
    switch(a->castTo){
      case Datatype::Int:
        return {Datatype::Int, toInt(b)};
      case Datatype::Double:
        return {Datatype::Double, toDouble(b)};
      case Datatype::Char:
        return {Datatype::Char, toChar(b)};
      case Datatype::Bool:
        return {Datatype::Bool, isTrue(b)};
      case Datatype::String:
        return {Datatype::String, toString(b)};
      default:
        throw std::runtime_error("Invalid data type to be casted to" );
    }
    }
    catch(const std::runtime_error& err){
      throw interpreter_error(err.what(), a->location.line, a->location.column);
    }
  }
}

bool Interpreter::isNumeric(const Value& value){
  if(value.type == Datatype::Int || value.type == Datatype::Char || value.type == Datatype::Double || value.type == Datatype::Bool) return true;
  return false;
}

Value Interpreter::convertString(const Cast& expr){
  try{
    auto b = eval(*expr.expr);
    switch(expr.castTo){
      case Datatype::Int:
        return {Datatype::Int, std::stoll(std::get<std::string> (b.data))};
      case Datatype::Double:
        return {Datatype::Double, std::stod(std::get<std::string> (b.data))};
      case Datatype::Char:
        if(auto a = std::get <std::string> (b.data); a.size() == 1) return {Datatype::Char, a[0]};
        else throw std::runtime_error("err");
      case Datatype::Bool:
        if(auto a = std::get<std::string> (b.data); a == "true" || a == "false") return {Datatype::Bool, a == "true" ? true:false};
        throw std::runtime_error("err");
      default:
        throw std::runtime_error("err");
    }
  }
  catch(const std::exception&){
    throw interpreter_error("The string cannot be casted to another data type", expr.location.line, expr.location.column);
  }
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
      throw std::runtime_error("Such data type cannot be casted to double");
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
      throw std::runtime_error("Such data type cannot be casted to int");
  }
}

std::string Interpreter::toString(const Value& value){
  switch(value.type){
    case Datatype::Int:
      return std::to_string(std::get<int64_t>(value.data));
    case Datatype::Double:
      return std::to_string(std::get<double>(value.data));
    case Datatype::Char:
      return std::string(1, std::get <char> (value.data));
    case Datatype::Bool:
      return std::get<bool> (value.data) ? "true" : "false";
    default:
      throw std::runtime_error("Such data type cannot be casted to string");
  }
}

char Interpreter::toChar(const Value& value){
  int64_t var = toInt(value);
  if(var < 0 || var > 255) throw std::runtime_error("the value is too big to be casted");
  return static_cast<char>(static_cast<unsigned char>(var));
}

Value Interpreter::evalAdd(const Value& left, const Value& right){
  if(!isNumeric(left) || !isNumeric(right)) throw std::runtime_error("Operator \"+\" cannot be used to such value type");
  if(left.type == Datatype::Double || right.type == Datatype::Double) return {Datatype::Double, toDouble(left) + toDouble(right)};
  return {Datatype::Int, toInt(left) + toInt(right)};
}

Value Interpreter::evalSub(const Value& left, const Value& right){
  if(!isNumeric(left) || !isNumeric(right)) throw std::runtime_error("Operator \"-\" cannot be used to such value type");
  if(left.type == Datatype::Double || right.type == Datatype::Double) return {Datatype::Double, toDouble(left) - toDouble(right)};
  return {Datatype::Int, toInt(left) - toInt(right)};
}

Value Interpreter::evalMul(const Value& left, const Value& right){
  if(!isNumeric(left) || !isNumeric(right)) throw std::runtime_error("Operator \"*\" cannot be used to such value type");
  if(left.type == Datatype::Double || right.type == Datatype::Double) return {Datatype::Double, toDouble(left) * toDouble(right)};
  return {Datatype::Int, toInt(left) * toInt(right)};
}

Value Interpreter::evalDiv(const Value& left, const Value& right){
  if(!isNumeric(left) || !isNumeric(right)) throw std::runtime_error("Operator \"/\" cannot be used to such value type");
  auto DBLright = toDouble(right);
  if(DBLright == 0.0) throw std::runtime_error("Division by zero is not permitted");
  return {Datatype::Double, toDouble(left) / DBLright};
}

Value Interpreter::evalMod(const Value& left, const Value& right){
  if(left.type != Datatype::Int || right.type != Datatype::Int) throw std::runtime_error("Operator \"%\" cannot be used to such value type");
  auto INTright = toInt(right);
  if(INTright == 0) throw std::runtime_error("Division by zero is not permitted");
  return {Datatype::Int, toInt(left) % toInt(right)};
}

Value Interpreter::evalGr(const Value& left, const Value& right){
  if(!isNumeric(left) || !isNumeric(right)) throw std::runtime_error("Operator \">\" cannot be used to such value type");
  if(left.type == Datatype::Double || right.type == Datatype::Double) return {Datatype::Bool, toDouble(left) > toDouble(right)};
  return {Datatype::Bool, toInt(left) > toInt(right)};
}

Value Interpreter::evalLs(const Value& left, const Value& right){
  if(!isNumeric(left) || !isNumeric(right)) throw std::runtime_error("Operator \"<\" cannot be used to such value type");
  if(left.type == Datatype::Double || right.type == Datatype::Double) return {Datatype::Bool, toDouble(left) < toDouble(right)};
  return {Datatype::Bool, toInt(left) < toInt(right)};
}

Value Interpreter::evalGe(const Value& left, const Value& right){
  if(!isNumeric(left) || !isNumeric(right)) throw std::runtime_error("Operator \">=\" cannot be used to such value type");
  if(left.type == Datatype::Double || right.type == Datatype::Double) return {Datatype::Bool, toDouble(left) >= toDouble(right)};
  return {Datatype::Bool, toInt(left) >= toInt(right)};
}

Value Interpreter::evalLe(const Value& left, const Value& right){
  if(!isNumeric(left) || !isNumeric(right)) throw std::runtime_error("Operator \"<=\" cannot be used to such value type");
  if(left.type == Datatype::Double || right.type == Datatype::Double) return {Datatype::Bool, toDouble(left) <= toDouble(right)};
  return {Datatype::Bool, toInt(left) <= toInt(right)};
}

Value Interpreter::evalEq(const Value& left, const Value& right){
  if(!isNumeric(left) || !isNumeric(right)) throw std::runtime_error("Operator \"==\" cannot be used to such value type");
  if(left.type == Datatype::Double || right.type == Datatype::Double) return {Datatype::Bool, toDouble(left) == toDouble(right)};
  return {Datatype::Bool, toInt(left) == toInt(right)};
}

Value Interpreter::evalNq(const Value& left, const Value& right){
  if(!isNumeric(left) || !isNumeric(right)) throw std::runtime_error("Operator \"!=\" cannot be used to such value type");
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

void Interpreter::input(const Input& stmt){
  if(auto a = dynamic_cast <const Variable*> (stmt.input.get())){
    std::string str;
    std::cin >> str;
    variables.back()[a->name] = {Datatype::String, str};
    return;
  }
  else if (auto a = dynamic_cast <const Cast*> (stmt.input.get())){
    if(auto b = dynamic_cast <const Variable*> (a->expr.get())){
      std::string str;
      std::cin>>str;
      variables.back()[b->name] = {Datatype::String, str};
      variables.back()[b->name] = convertString(*a);
      return;
    }
  }
  throw interpreter_error("The expressions cannot be used in the input function", stmt.location.line);

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
      throw interpreter_error("Such data type cannot be printed", stmt.location.line);
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
  else if (auto a = dynamic_cast<const Input*> (&stmt)) input(*a);
  else if (auto a = dynamic_cast<const Definition*> (&stmt)) definition(*a);
  else if (auto a = dynamic_cast<const IfStatement*> (&stmt)) ifStatement(*a);
}

void Interpreter::execute(const Program& program){
    variables.push_back({});
    for(size_t i = 0; i < program.statements.size(); i++){
      matchStatement(*program.statements[i]);
    }
}
