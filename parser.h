#ifndef PARSER
#define PARSER

#include "lexer.h"
#include "tools/tools.h"
#include "ast.h"
#include <string>
#include <stack>

namespace CompilerFront
{
    struct ReduceInfo
    {
        int reduceStackSize;
        int reduceCnt;
        std::string expressionLeft;
        std::string expressionFirst;
        ReduceInfo() {}
        ReduceInfo(int reduceStackSize,
                   int reduceCnt,
                   std::string expressionLeft,
                   std::string expressionFirst)
            : reduceStackSize(reduceStackSize),
              reduceCnt(reduceCnt),
              expressionLeft(expressionLeft),
              expressionFirst(expressionFirst) {}
    };

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

        std::unique_ptr<PascalAST::ASTNode> Parse();

    private:
        std::string path;
        Lexer lexer;
        std::stack<int> symbolStack;
        std::stack<ReduceInfo> reduceStack;
        std::vector<std::unique_ptr<PascalAST::ASTNode>> astStack;
        Token curToken;
        Token nxtToken;
    };
}

#endif