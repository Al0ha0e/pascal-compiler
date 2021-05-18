#ifndef PARSER
#define PARSER

#include "lexer.h"
#include "tools/tools.h"
#include "ast.h"
#include <string>
#include <sstream>
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
        Parser(std::string path, std::string symbolPath, std::string tablePath, std::string syncPath, std::string st) : path(path), lexer(path)
        {
            Tools::AddConstantSymbols();
            Tools::LoadSymbols(symbolPath);
            Tools::LoadLL1Table(tablePath, syncPath);
            //std::cout << "----------------------------INIT " << st << std::endl;
            symbolStack.push(Tools::SymbolNameMap.find(st)->second);
        }

        std::unique_ptr<PascalAST::ASTNode> Parse(bool &ok);

    private:
        std::string path;
        Lexer lexer;
        std::stack<int> symbolStack;
        std::stack<ReduceInfo> reduceStack;
        std::vector<std::unique_ptr<PascalAST::ASTNode>> astStack;
        Token curToken;
        Token nxtToken;

        void logErrMsg(int line, int column, int topSymbolId, int curSymbolId, bool isTermi)
        {
            std::ostringstream errMsg;
            errMsg << "line: " << line << " column: " << column << " met token: ";

            errMsg << Tools::InvSymbolNameMap.find(curSymbolId)->second << ",";
            errMsg << " expecting:";
            if (isTermi)
            {
                errMsg << Tools::InvSymbolNameMap.find(topSymbolId)->second;
            }
            else
            {
                auto &item = Tools::LL1Table.find(topSymbolId)->second;
                for (auto &it : item)
                    errMsg << " " << Tools::InvSymbolNameMap.find(it.first)->second;
            }
            std::cout << errMsg.str() << std::endl;
        }
    };
}

#endif