#include <iostream>
#include "lexer.h"

int main(){
    std::vector <Token> a=Tokenize("if 1 : hey");
    std::cout<<"hey\n";
    return 0;

}