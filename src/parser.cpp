#include "parser.h"
const Token& Parser::peek() const {
    return tokens[line][pos];
}

Token& Parser::advance(){
    if(isEnd()) throw std::invalid_argument("Error: Unexepected Ending.");
    return tokens[line][pos++];
}


Parser::Parser(std::vector <std::vector <Token>>& T) : tokens(T) {}

bool Parser::isEnd(){
    return peek().type == TokenType::End;
}

void Parser::SyntaxErr(){
    throw std::invalid_argument("Error: Invalid Syntax at line: " + std::to_string(peek().lineID) + "; column: " + std::to_string(peek().columnID));
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
        advance();
        return expr;
    }
    else if(Check(TokenType::Identifier)){
        auto expr = std::make_unique <Variable> ();
        expr->name = peek().lexeme;
        advance();
        return expr;
    }
    SyntaxErr();
    return nullptr;
}

std::unique_ptr <Expression> Parser::MakeExpression(){
  auto expr = ParseMidTerm();

  while(Check(">") || Check("<") || Check("=")){
    std::string sign = advance().lexeme;
    if(Check ("=")) sign +=advance().lexeme;
    auto right = ParseMidTerm();
    auto left = std::move(expr);
    auto logic = std::make_unique <Logical> ();
    logic -> op = sign;
    logic-> right = std::move(right);
    logic -> left = std::move(left);
    expr = std::move(logic);
  }
  return expr;
}

std::unique_ptr <Expression> Parser::ParseTerm(){
    auto expr = SingleParse();

    while(Check("*") || Check("/")){
        char sign = advance().lexeme[0];
        auto right = SingleParse();
        auto left = std::move(expr);
        auto bin = std::make_unique <Binary> ();
        bin->op = sign;
        bin->right = std::move(right);
        bin->left = std::move(left);
        expr = std::move(bin);
    }
    return expr;
}

std::unique_ptr <Expression> Parser::ParseMidTerm(){
    auto expr = ParseTerm();

    while(Check("+") || Check("-")){
        char sign = advance().lexeme[0];
        auto right = ParseTerm();
        auto left = std::move(expr);
        auto bin = std::make_unique <Binary> ();
        bin->op = sign;
        bin->right = std::move(right);
        bin->left = std::move(left);
        expr = std::move(bin);
    }
    return expr;
}

std::unique_ptr <Statement> Parser::ParseOutput(){
 auto stmt = std::make_unique<Output> ();
        advance();
        if (Check("(")) advance();
        else SyntaxErr();
        if (Check(TokenType::Number) || Check(TokenType::Boolean) || Check(TokenType::Symbol) || Check(TokenType::Double) || Check(TokenType::String)|| Check(TokenType::Identifier)) stmt->output = MakeExpression();
        else SyntaxErr();
        if (Check(")")) advance();
        else SyntaxErr();
        if (Check(TokenType::End)) {
            return stmt;
        }
        else SyntaxErr();
        return nullptr;
}

std::unique_ptr <Statement> Parser::ParseDefinition(){
 auto stmt = std::make_unique<Definition> ();
        stmt->name = advance().lexeme;
        if (Check("=")) advance();
        else SyntaxErr();
        stmt->value = MakeExpression();
        if(Check(TokenType::End)) {
            return stmt;
        }
        else SyntaxErr();
        return nullptr;
}
std::unique_ptr <Statement> Parser::ParseIfStatement(){
  auto stmt = std::make_unique<IfStatement> ();
  advance();
  if(Check("(")) advance();
  else SyntaxErr();
  stmt -> expr = MakeExpression();
  if (Check(")")) advance();
  else SyntaxErr();
  eatEnd();
  if (Check("{")) advance();
  else SyntaxErr();
  stmt->Instructions = MakeBody();
  if(Check(Keyword::Else)){
    stmt-> elseStatement = std::make_unique <IfStatement> ();
    advance();
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
    if(Check(Keyword::Out)) return ParseOutput();
    if (Check(TokenType::Identifier)) return ParseDefinition();
    if(Check(Keyword::If)) return ParseIfStatement();
    SyntaxErr();
    return nullptr; 
}

void Parser::Parse(Program& program){
    try{
    while(line<tokens.size()){
         program.statements.push_back(MakeStatement());
        if(peek().type == TokenType::End){
            line++;
            pos = 0; 
        }
        else SyntaxErr();
    }
}
    catch (const std::invalid_argument& e){
        std::cerr <<e.what()<<std::endl;
    }
}
