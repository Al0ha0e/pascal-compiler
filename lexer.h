#ifndef LEXER
#define LEXER

#include <string>

enum TokenType
{

};

struct Token
{
    TokenType type;
    int stLine;
    int stColumn;
    int len;
    std::string content;
};

void GetToken();

#endif