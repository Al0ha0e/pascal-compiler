#ifndef LEXER
#define LEXER

#include <string>
#include <iostream>
#include <fstream>
#include <sstream>

namespace CompilerFront
{
    struct Token
    {
        std::string type;
        int stLine;
        int stColumn;
        int len;
        std::string content;

        Token() {}
        Token(std::string type, std::string content, int stLine, int stColumn)
            : type(type), content(content), stLine(stLine), stColumn(stColumn), len(content.length())
        {
        }
    };

    class Lexer
    {
    public:
        Lexer() {}
        Lexer(std::string path) : pos(0), line(1), column(1), path(path), charST(false)
        {
            std::ifstream f(path);
            std::ostringstream tmp;
            tmp << f.rdbuf();
            content = tmp.str();
            contentLength = content.length();
        }

        Token GetToken();

    private:
        int pos;
        int line;
        int column;
        int contentLength;
        std::string path;
        std::string content;
        bool charST;
    };
}

#endif