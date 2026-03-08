#include "lexer.h"

void Lexer::readFile(std::string name){
  std::string line;
  std::ifstream in(name);
  if(in.is_open()){
    while(std::getline(in, line)){
      Initialcode.push_back(line);
    }
  }
}

Token::Token(TokenType type, const Keyword& keyword, std::string lexeme,size_t lineID, size_t columnID){
    this->type=type;
    this->keyword=keyword;
    this->lexeme=lexeme;
    this->lineID=lineID;
    this->columnID=columnID;
} 

void Lexer::unexEnd(){
  if (pos==Initialcode[i].size()) throw std::invalid_argument("Unexpected ending, at line: " + std::to_string(i) + "; column: " + std::to_string(pos)); 
}

Keyword Lexer::IsKeyword(const std::string_view lexeme){
    for(size_t i=0; i<keywords.size();i++){
        if (lexeme==keywords[i]) return static_cast<Keyword>(i);
    }
    return Keyword::amount;
}

char Lexer::getEscapes(const char& c){
  switch(c){
    case 'n': return '\n';
    case '"': return '\"';
    case '\\': return '\\';
    case 'r': return '\r';
    case 't': return '\t';
    case '\'': return '\'';
    case '0': return '\0';
    default: return -1;
  }
}

bool Lexer::isLetter(){
  if(std::isalpha(Initialcode[i][pos]) || Initialcode[i][pos]=='_') {
    size_t startpos = pos;
     while(pos<Initialcode[i].size() && (std::isalpha(Initialcode[i][pos]) || Initialcode[i][pos]=='_')) {
       pos++;
    }
    pos--;
    auto lexeme = std::string_view(Initialcode[i].data()+startpos, Initialcode[i].data()+pos+1);
        if (lexeme == "true" || lexeme == "false"){
          tokens.back().emplace_back(TokenType::Boolean, Keyword::amount, std::string(lexeme), i + 1, pos - lexeme.size()+2);
          return true;
        }
         Keyword result = IsKeyword(lexeme);
         if(result != Keyword::amount) tokens.back().emplace_back(TokenType::Keyword, result, std::string(lexeme), i + 1 , pos - lexeme.size() + 2);
         else tokens.back().emplace_back(TokenType::Identifier, result, std::string(lexeme), i + 1, pos - lexeme.size() + 2);
         return true;
    }
    return false;
}

bool Lexer::isDigit(){
    size_t startpos = pos;
    if(std::isdigit(Initialcode[i][pos])){
     while(pos<Initialcode[i].size() && (std::isdigit(Initialcode[i][pos]))){
       pos++;
     }
     if(pos<Initialcode[i].size() && Initialcode[i][pos] == '.'){
     pos++;
      while(pos<Initialcode[i].size() && std::isdigit(Initialcode[i][pos])){
        pos++;
      }
     tokens.back().emplace_back(TokenType::Double, Keyword::amount, std::string(Initialcode[i].data() + startpos, Initialcode[i].data() + pos), i + 1, startpos + 1);
     pos--;
     return true;
     }
     tokens.back().emplace_back(TokenType::Number, Keyword::amount, std::string(Initialcode[i].data() + startpos, Initialcode[i].data() + pos), i + 1, startpos + 1);
     pos--;
     return true;
    }
    return false;
}

bool Lexer::isText(){
    if(Initialcode[i][pos] == '\''){
      pos++;
      if(Initialcode[i][pos] == '\\'){
        pos++;
        char c = getEscapes(Initialcode[i][pos]);
        if (c==-1) throw std::invalid_argument("Invalid Escape Sequence at line: " + std::to_string(i) + "; column: " + std::to_string(pos));
        tokens.back().emplace_back(TokenType::Symbol, Keyword::amount,std::string(1,c), i + 1,pos + 1);
      }
      else{
        tokens.back().emplace_back(TokenType::Symbol, Keyword::amount, std::string(1,Initialcode[i][pos]), i + 1, pos + 1);
      }
      if(Initialcode[i][++pos] !='\'') throw std::invalid_argument("Invalid argument for char, at line: " + std::to_string(i) + "; column: " + std::to_string(pos));
      return true;
    }
    else if (Initialcode[i][pos] == '"'){
      std::string str = "";
      pos++;
      while(Initialcode[i][pos] != '"'){
       if(Initialcode[i][pos] == '\\') str+=getEscapes(Initialcode[i][++pos]);
       else str+=Initialcode[i][pos];
       pos++;
       unexEnd();
      }
      tokens.back().emplace_back(TokenType::String, Keyword::amount, str, i + 1, pos + 1);
      return true;
    }
    return false;
}

bool Lexer::isOperator(){
    switch(Initialcode[i][pos]){
      case '+':
      case '*':
      case '/':
      case '%':
      tokens.back().emplace_back(TokenType::Operator, Keyword::amount, std::string(1, Initialcode[i][pos]), i + 1, pos + 1);
      return true;
      case '-':
      if(Initialcode[i][pos+1] == '>') tokens.back().emplace_back(TokenType::Operator, Keyword::amount, "->", i + 1, ++pos +1);
      else tokens.back().emplace_back(TokenType::Operator, Keyword::amount, "-", i+1, pos + 1);
      return true;
      case '>':
      case '<':
      case '=':
      case '!':
      if(Initialcode[i][pos + 1] == '=') tokens.back().emplace_back(TokenType::Operator, Keyword::amount, std::string(1, Initialcode[i][pos]) + "=", i + 1, ++pos + 1);
      else tokens.back().emplace_back(TokenType::Operator, Keyword::amount, std::string(1, Initialcode[i][pos]), i + 1, pos +1);
      return true;
    }
    return false;
}

bool Lexer::isSeparator(){
    switch(Initialcode[i][pos]){
        case '(':
        case ')':
        case ':':
        case ';':
        case '[':
        case ']':
        case '{':
        case '}':
        tokens.back().emplace_back(TokenType::Separator, Keyword::amount, std::string(1, Initialcode[i][pos]), i + 1, pos + 1);
        return true;
        break;
      }
      return false;
}

std::vector <std::vector <Token>> Lexer::Tokenize(){
   for(i = 0; i < Initialcode.size(); i++){
    if (Initialcode[i].size() == 0 ) continue;
   tokens.emplace_back();
    for(pos = 0; pos < Initialcode[i].size(); pos++){
      if(std::isspace(Initialcode[i][pos])) continue;
      else if(isLetter()) continue;
      else if (isDigit()) continue;
      else if (isText()) continue;
      else if (isOperator()) continue;
      else if (isSeparator()) continue;
      else throw std::invalid_argument("Invalid symbol at line " + std::to_string(i) + "; column: " + std::to_string(pos));
    }
    tokens.back().emplace_back(TokenType::End, Keyword::amount, "", i + 1, Initialcode[i].size());
   }
    return tokens;
}


