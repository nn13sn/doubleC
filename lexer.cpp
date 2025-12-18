#include "lexer.h"

Token::Token(TokenType type, const Keyword& keyword, std::string lexeme,size_t lineID, size_t columnID){
    this->type=type;
    this->keyword=keyword;
    this->lexeme=lexeme;
    this->lineID=lineID;
    this->columnID=columnID;
} 

std::string_view readIdentifier(std::string_view InitLine, size_t &pos){
     while(pos<InitLine.size() && (std::isalpha(InitLine[pos]) || InitLine[pos]=='_')) {
        pos++;
    }
    return std::string_view(InitLine.data(), InitLine.data()+pos);
}

Keyword IsKeyword(const std::string_view lexeme){
    static constexpr std::array <std::string_view, static_cast <size_t> (Keyword::amount)> keywords {
        "if", "else", "int", "in", "out"
    };
    for(size_t i=0; i<keywords.size();i++){
        if (lexeme==keywords[i]) return static_cast<Keyword>(i);
    }
    return Keyword::amount;
}

std::vector <Token> Tokenize(std::string InitLine){
   size_t pos = 0;
   Keyword result;
   std::string_view lexeme;
   std::vector <Token> tokens;
    while(pos<InitLine.size()){
        if(InitLine[pos]==' ') continue;
        else if(std::isalpha(InitLine[pos]) || InitLine[pos]=='_') {
            lexeme = readIdentifier(InitLine, pos);
            result = IsKeyword(lexeme);
            if(result != Keyword::amount) tokens.emplace_back(TokenType::Keyword, result, std::string(lexeme), size_t{0}, static_cast<size_t> (pos));
            else break;
        }
    }
    return tokens;
}


