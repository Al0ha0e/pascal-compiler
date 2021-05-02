#include "lexer.h"
#include <iostream>
using namespace CompilerFront;

int main()
{
    Lexer lexer("test.txt");
    Token curToken = lexer.GetToken();
    while (curToken.type != "$")
    {
        std::cout << curToken.type << " " << curToken.content << std::endl;
        curToken = lexer.GetToken();
    }
    return 0;
}