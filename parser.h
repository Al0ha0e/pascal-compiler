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
        Parser(std::string path, std::string symbolPath, std::string tablePath, std::string st) : path(path), lexer(path)
        {
            Tools::AddConstantSymbols();
            Tools::LoadSymbols(symbolPath);
            Tools::LoadLL1Table(tablePath);
            std::cout << "----------------------------INIT " << st << std::endl;
            symbolStack.push(Tools::SymbolNameMap.find(st)->second);
        }

        void Parse();

    private:
        std::string path;
        Lexer lexer;
        std::stack<int> symbolStack;
        Token curToken;
    };
}

#endif