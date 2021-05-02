#ifndef PARSER
#define PARSER

#include "lexer.h"
#include "tools/tools.h"
#include <string>
#include <stack>

namespace CompilerFront
{
    class Parser
    {
    public:
        Parser();
        Parser(std::string path) : path(path), lexer(path) {}

    private:
        std::string path;
        Lexer lexer;
        std::stack<int> symbolStack;
    };
}

#endif