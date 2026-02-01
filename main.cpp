#include <iostream>
#include "lexer.h"
#include "AST.h"
#include "parser.h"

int main(){
    Program program; 
    std::vector <Token> a=Tokenize("int a;");
    std::cout<<"hey\n";
    std::vector <std::vector <Token>> b;
    b.push_back(a);
    Parser parser(b);
    parser.Parse(program);
    return 0;

}