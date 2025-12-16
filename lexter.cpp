enum class TokenType{
    Identifier,
    Keyword,
    Number, 
    String,
    Operator,
    Separator,
    End,
    Invalid
};

struct Token{
    Tokentype type;
    std::string lexeme;
    int lineID;
    int columnID;
};



