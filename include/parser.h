#pragma once
#include <iostream>
#include <stdexcept>
#include <cstddef>
#include "lexer.h"
#include "AST.h"
#define OPENBRACKET "Expected \"(\""
#define CLOSEBRACKET "Expected \")\""
#define CURLYBRACKET "Expected \"{\""
class Parser {
    private:
    size_t line = 0;
    size_t pos = 0;
    void SyntaxErr(const std::string& err);
    const Token& peek() const;
    Token& advance();
    std::vector <std::vector <Token>>& tokens;
    std::unique_ptr <Statement> MakeStatement();
    std::unique_ptr <Statement> ParseInput();
    std::unique_ptr <Statement> ParseOutput();
    std::unique_ptr <Statement> ParseDefinition();
    std::unique_ptr <Statement> ParseIfStatement();
    std::unique_ptr <Statement> ParseWhile();
    std::unique_ptr <Statement> ParseFor();
    std::unique_ptr <Expression> ParseMidTerm();
    std::unique_ptr <Expression> MakeExpression();
    std::unique_ptr <Expression> ParseTerm();
    std::unique_ptr <Expression> SingleParse();
    std::unique_ptr <Program> MakeBody();
    bool Check(TokenType type);
    bool Check(std::string lexeme);
    bool Check(Keyword keyword);
    bool isEnd();
    bool eatEnd();
    Datatype getDatatype(const TokenType& tokentype);
    Datatype getDatatype(const Keyword& keyword);
    std::variant <int64_t, char, std::string, double, bool, std::vector <Value>> getData(); 
    Operator GetOperator(const std::string& op);
    public:
    Parser(std::vector <std::vector <Token>>& T);
    void Parse(Program& program);
};
