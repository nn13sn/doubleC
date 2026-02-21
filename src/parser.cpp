#include "parser.h"
const Token& Parser::peek() const {
    return tokens[line][pos];
}

Token& Parser::advance(){
    if(isEnd()) throw std::invalid_argument("Unexepected ending at line: " + std::to_string(peek().lineID) + "; column: " + std::to_string(peek().columnID));
    return tokens[line][pos++];
}


Parser::Parser(std::vector <std::vector <Token>>& T) : tokens(T) {}

bool Parser::isEnd(){
    return peek().type == TokenType::End;
}

void Parser::SyntaxErr(){
    throw std::invalid_argument("Invalid Syntax at line: " + std::to_string(peek().lineID) + "; column: " + std::to_string(peek().columnID));
}

bool Parser::Check(TokenType type){
    return type == peek().type;
}

bool Parser::Check(std::string lexeme){
    return lexeme == peek().lexeme;
}

bool Parser::Check(Keyword keyword){
    return keyword == peek().keyword;
}

bool Parser::eatEnd(){
  if(Check(TokenType::End)){
  if(line == tokens.size()-1) return false;
  line++;
  pos = 0;
  return true;
  }
  return false;
}

Datatype Parser::getDatatype(const TokenType& tokentype){
  if(Check(TokenType::Number)) return Datatype::Int;
  if(Check(TokenType::Double)) return Datatype::Double;
  if(Check(TokenType::Symbol)) return Datatype::Char;
  if(Check(TokenType::Boolean)) return Datatype::Bool;
  if(Check(TokenType::String)) return Datatype::String;
  return Datatype::Invalid;
}

Datatype Parser::getDatatype(const Keyword& keyword){
  switch (keyword){
    case Keyword::Int: return Datatype::Int;
    case Keyword::Double: return Datatype::Double;
    case Keyword::Char: return Datatype::Char;
    case Keyword::Bool: return Datatype::Bool;
    case Keyword::String: return Datatype::String;
    default: return Datatype::Invalid;
  }
}

std::variant <int64_t, char, std::string, double, bool, std::vector <Value>> Parser::getData(){
  if(Check(TokenType::Number)) return std::stoi(peek().lexeme);
  if(Check(TokenType::Double)) return std::stod(peek().lexeme);
  if(Check(TokenType::Symbol)) return peek().lexeme[0];
  if(Check(TokenType::String)) return peek().lexeme;
  if(Check(TokenType::Boolean)){
    if(peek().lexeme == "true") return true;
    if (peek().lexeme == "false") return false;
  }
}

std::unique_ptr <Program> Parser::MakeBody(){
 auto body = std::make_unique <Program> ();
   eatEnd();
  while(true){
    if(Check("}")) break;
    body->statements.push_back(MakeStatement());
    if(Check("}")) break;
    else if (!eatEnd()) SyntaxErr();
  }
  advance();
  if(Check(TokenType::End)){
   if(line == tokens.size()-1) return body; 
   line++;
   pos=0;
  }
  return body;
}

std::unique_ptr <Expression> Parser::SingleParse(){
    if(Check(TokenType::Number) || Check(TokenType::Double) || Check(TokenType::Boolean) || Check(TokenType::Symbol) || Check(TokenType::String)){
        auto expr = std::make_unique <exprValue> ();
        expr->value.type = getDatatype(peek().type);
        expr->value.data = getData();
        expr->location.line = peek().lineID;
        expr->location.column = advance().columnID;
        return expr;
    }
    else if(Check(TokenType::Identifier)){
        auto expr = std::make_unique <Variable> ();
        expr->name = peek().lexeme;
        expr->location.line = peek().lineID;
        expr->location.column = advance().columnID;
        return expr;
    }
    else if(Check(TokenType::Keyword)){
        auto expr = std::make_unique <Cast> ();
        expr->castTo = getDatatype(peek().keyword);
        if(expr->castTo == Datatype::Invalid) SyntaxErr();
        expr->location.line = peek().lineID;
        expr->location.column = advance().columnID;
        if(Check("(")) advance();
        else SyntaxErr();
        expr->expr = MakeExpression();
        if(Check(")")) advance();
        else SyntaxErr();
        return expr;
    }
    else if (Check("(")){
      advance();
      auto expr = MakeExpression();
      if(Check(")")) advance();
      else SyntaxErr();
      return expr;
    }
    SyntaxErr();
    return nullptr;
}

std::unique_ptr <Expression> Parser::MakeExpression(){
  auto expr = ParseMidTerm();

  while(Check(">") || Check("<") || Check("=")){
    auto logic = std::make_unique <Logical> ();
    logic -> location.line = peek().lineID;
    logic -> location.column = peek().columnID;
    logic -> op = advance().lexeme;
    if(Check ("=")) logic-> op +=advance().lexeme;
    logic -> right = ParseMidTerm();
    logic -> left = std::move(expr);
    expr = std::move(logic);
  }
  return expr;
}

std::unique_ptr <Expression> Parser::ParseTerm(){
    auto expr = SingleParse();

    while(Check("*") || Check("/") || Check("%")){
        auto bin = std::make_unique <Binary> ();
        bin->location.line = peek().lineID;
        bin->location.column = peek().columnID;
        bin->op = advance().lexeme[0];
        bin->right = SingleParse();
        bin->left = std::move(expr);
        expr = std::move(bin);
    }
    return expr;
}

std::unique_ptr <Expression> Parser::ParseMidTerm(){
    auto expr = ParseTerm();

    while(Check("+") || Check("-")){
        auto bin = std::make_unique <Binary> ();
        bin->location.line = peek().lineID;
        bin->location.column = peek().columnID;
        bin->op = advance().lexeme[0];
        bin->right = ParseTerm();
        bin->left = std::move(expr);
        expr = std::move(bin);
    }
    return expr;
}

std::unique_ptr <Statement> Parser::ParseInput(){
  auto stmt = std::make_unique <Input> ();
    stmt->location.line = advance().lineID;
    if(Check("(")) advance();
    else SyntaxErr();
    stmt->input = MakeExpression();
    if(Check(")")) advance();
    else SyntaxErr();
    return stmt;
}

std::unique_ptr <Statement> Parser::ParseOutput(){
 auto stmt = std::make_unique<Output> ();
        stmt-> location.line = advance().lineID;
        if (Check("(")) advance();
        else SyntaxErr();
        stmt->output = MakeExpression();
        if (Check(")")) advance();
        else SyntaxErr();
        return stmt;
}

std::unique_ptr <Statement> Parser::ParseDefinition(){
 auto stmt = std::make_unique<Definition> ();
        stmt->name = advance().lexeme;
        if (Check("=")) stmt-> location.line = advance().lineID;
        else SyntaxErr();
        stmt->value = MakeExpression();
        return stmt;
}

std::unique_ptr <Statement> Parser::ParseIfStatement(){
  auto stmt = std::make_unique<IfStatement> ();
  stmt -> location.line = advance().lineID;
  if(Check("(")) advance();
  else SyntaxErr();
  stmt -> expr = MakeExpression();
  if (Check(")")) advance();
  else SyntaxErr();
  stmt->location.line = peek().lineID;
  eatEnd();
  if (Check("{")) advance();
  else SyntaxErr();
  stmt->Instructions = MakeBody();
  if(Check(Keyword::Else)){
    stmt-> elseStatement = std::make_unique <IfStatement> ();
    stmt-> location.line = advance().lineID; 
    eatEnd();
    if (Check("{")) {
      advance();
      stmt->elseStatement->expr = nullptr;
      stmt->elseStatement->Instructions = MakeBody();
    }
    else if(Check(Keyword::If)){
      stmt->elseStatement.reset(static_cast <IfStatement*>(ParseIfStatement().release()));
    }
    else SyntaxErr();
  }
  return stmt;
}

std::unique_ptr <Statement> Parser::MakeStatement(){
    auto stmt = std::make_unique <Statement> ();
    if(Check(Keyword::Out)) stmt = ParseOutput();
    else if (Check(Keyword::In)) stmt = ParseInput();
    else if (Check(TokenType::Identifier)) stmt = ParseDefinition();
    else if(Check(Keyword::If)) stmt = ParseIfStatement();
    if(Check(TokenType::End)) return stmt;
    SyntaxErr();
    return nullptr; 
}

void Parser::Parse(Program& program){
    while(line<tokens.size()){
         program.statements.push_back(MakeStatement());
        if(peek().type == TokenType::End){
            line++;
            pos = 0; 
        }
        else SyntaxErr();
    }
}
