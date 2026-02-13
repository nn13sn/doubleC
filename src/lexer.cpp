#include "lexer.h"

std::vector <std::string> readFile(std::string name){
  std::vector <std::string> code;
  std::string line;
  std::ifstream in(name);
  if(in.is_open()){
    while(std::getline(in, line)){
      code.push_back(line);
    }
  }
  return code;
}

Token::Token(TokenType type, const Keyword& keyword, std::string lexeme,size_t lineID, size_t columnID){
    this->type=type;
    this->keyword=keyword;
    this->lexeme=lexeme;
    this->lineID=lineID;
    this->columnID=columnID;
} 

void unexEnd(size_t& i,size_t& pos, std::vector <std::string>& Initialcode){
  if (pos==Initialcode[i].size()) throw std::invalid_argument("Error: Unexpected end, at line: " + std::to_string(i) + "; column: " + std::to_string(pos)); 
}

std::string_view readIdentifier(std::string_view InitLine,size_t& i, size_t &pos){
    size_t startpos = pos;
     while(pos<InitLine.size() && (std::isalpha(InitLine[pos]) || InitLine[pos]=='_')) {
       pos++;
    }
     pos--;
    return std::string_view(InitLine.data()+startpos, InitLine.data()+pos+1);
}

Keyword IsKeyword(const std::string_view lexeme){
    static constexpr std::array <std::string_view, static_cast <size_t> (Keyword::amount)> keywords {
        "if", "else", "true", "false", "int", "in", "out"
    };
    for(size_t i=0; i<keywords.size();i++){
        if (lexeme==keywords[i]) return static_cast<Keyword>(i);
    }
    return Keyword::amount;
}

char getEscapes(const char& c){
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

std::vector <std::vector <Token>> Tokenize(std::vector <std::string>& Initialcode){
   std::vector <std::vector <Token>> tokens;
   auto letter {[&](size_t& i, size_t& pos){
    if(std::isalpha(Initialcode[i][pos]) || Initialcode[i][pos]=='_') {
            std::string_view lexeme = readIdentifier(Initialcode[i], i, pos);
            if (lexeme == "true" || lexeme == "false"){
              tokens[i].emplace_back(TokenType::Boolean, Keyword::amount, std::string(lexeme), i, pos);
              return true;
            }
            Keyword result = IsKeyword(lexeme);
            if(result != Keyword::amount) tokens[i].emplace_back(TokenType::Keyword, result, std::string(lexeme), i, static_cast<size_t> (pos));
            else tokens[i].emplace_back(TokenType::Identifier, result, std::string(lexeme), i, static_cast<size_t> (pos));
            return true;
    }
    return false;
   }
   };
   auto digit { [&](size_t& i, size_t& pos){
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
     tokens[i].emplace_back(TokenType::Double, Keyword::amount, std::string(Initialcode[i].data() + startpos, Initialcode[i].data() + pos), i, pos);
     pos--;
     return true;
     }
     tokens[i].emplace_back(TokenType::Number, Keyword::amount, std::string(Initialcode[i].data() + startpos, Initialcode[i].data() + pos), i, static_cast<size_t> (pos));
     pos--;
     return true;
    }
    return false;
   }
   };
   auto symbols {[&](size_t& i, size_t& pos) {
    if(Initialcode[i][pos] == '\''){
      pos++;
      if(Initialcode[i][pos] == '\\'){
        pos++;
        char c = getEscapes(Initialcode[i][pos]);
        if (c==-1) throw std::runtime_error("Error: Invalid Escape Sequence at line: " + std::to_string(i) + "; column: " + std::to_string(pos));
        tokens[i].emplace_back(TokenType::Symbol, Keyword::amount,std::string(1,c), i, pos);
      }
      else{
        tokens[i].emplace_back(TokenType::Symbol, Keyword::amount, std::string(1,Initialcode[i][pos]), i,pos);
      }
      if(Initialcode[i][++pos] !='\'') throw std::invalid_argument("Error: Invalid argument for char, at line: " + std::to_string(i) + "; column: " + std::to_string(pos));
      return true;
    }
    else if (Initialcode[i][pos] == '"'){
      std::string str = "";
      pos++;
      while(Initialcode[i][pos] != '"'){
       if(Initialcode[i][pos] == '\\') str+=getEscapes(Initialcode[i][++pos]);
       else str+=Initialcode[i][pos];
       pos++;
       unexEnd(i,pos, Initialcode);
      }
      tokens[i].emplace_back(TokenType::String, Keyword::amount, str, i,pos);
      return true;
    }
    return false;
   }
   };
   auto OPERATOR { [&](size_t& i, size_t& pos){
    switch(Initialcode[i][pos]){
      case '+':
      case '-':
      case '*':
      case '/':
      case '%':
      case '>':
      case '=':
      case '<':
      case '!':
      tokens[i].emplace_back(TokenType::Operator, Keyword::amount, std::string(1, Initialcode[i][pos]), i, static_cast<size_t> (pos));
      return true;
    }
    return false;
   }
   };
   auto separator {[&](size_t& i, size_t& pos){
      switch(Initialcode[i][pos]){
        case '(':
        case ')':
        case ':':
        case ';':
        case '[':
        case ']':
        case '{':
        case '}':
        tokens[i].emplace_back(TokenType::Separator, Keyword::amount, std::string(1, Initialcode[i][pos]), i, static_cast<size_t> (pos));
        return true;
        break;
      }
      return false;
   }
   };
   try {
   for(size_t i = 0; i < Initialcode.size(); i++){
   tokens.emplace_back();
    for(size_t pos = 0; pos < Initialcode[i].size(); pos++){
      if(Initialcode[i][pos] == ' ') continue;
      else if(letter(i,pos)) continue;
      else if (digit(i,pos)) continue;
      else if (symbols(i,pos)) continue;
      else if (OPERATOR(i,pos)) continue;
      else if (separator(i,pos)) continue;
      else throw std::invalid_argument("Error: Invalid symbol at line " + std::to_string(i) + "; column: " + std::to_string(pos));
    }
    tokens[i].emplace_back(TokenType::End, Keyword::amount, "", i, Initialcode[i].size()-1);
   }
   }
   catch (const std::invalid_argument& e) {
     std::cerr << e.what() << std::endl;
   }
   catch (const std::runtime_error& e){
     std::cerr << e.what() << std::endl;
   }
    return tokens;
}


