#include <map>
#include <string>
#include <memory>
#include <vector>
#include <fstream>
#include <sstream>
#include <iostream>

const int EPS = 0;

enum SymbolType
{
    NON_TERMI,
    TERMI
};

typedef std::vector<int> Expression;

struct Symbol
{
    int id;
    SymbolType type;
    std::map<int, std::vector<Expression>> subExpressions;
};

int SymbolID;
std::map<std::string, int> SymbolNameMap;
std::map<int, std::string> InvSymbolNameMap;
std::map<int, Symbol> Symbols;

void GenSymbols(const std::string &pth)
{
    std::ifstream f(pth);
    std::string line = "";
    while (std::getline(f, line))
    {
        std::cout << line << std::endl;
        std::istringstream lstream(line);
        std::string symbolName;
        int isL = true;
        int lId = 0;
        Expression expression;
        expression.clear();
        while (std::getline(lstream, symbolName, ' '))
        {
            auto it = SymbolNameMap.find(symbolName);
            int id;
            if (it == SymbolNameMap.end())
            {
                SymbolNameMap.insert(std::pair<std::string, int>(symbolName, ++SymbolID));
                id = SymbolID;
                InvSymbolNameMap.insert(std::pair<int, std::string>(id, symbolName));
                Symbol symbol;
                symbol.id = id;
                symbol.type = TERMI;
                Symbols.insert(std::pair<int, Symbol>(id, symbol));
            }
            else
            {
                id = it->second;
            }
            if (isL)
            {
                isL = false;
                lId = id;
            }
            else
            {
                expression.push_back(id);
            }
        }
        int st = expression[0];
        expression.erase(expression.begin());
        Symbol &lsymbol = Symbols.find(lId)->second;
        lsymbol.type = NON_TERMI;
        auto it = lsymbol.subExpressions.find(st);
        if (it == lsymbol.subExpressions.end())
        {
            std::vector<Expression> expressions;
            expressions.push_back(expression);
            lsymbol.subExpressions.insert(std::pair<int, std::vector<Expression>>(st, expressions));
        }
        else
        {
            it->second.push_back(expression);
        }
    }
    f.close();
}

void Show()
{
    for (auto it1 : Symbols)
    {
        std::cout << InvSymbolNameMap.find(it1.first)->second << " " << it1.second.type << "<------" << std::endl;
        for (auto subs : it1.second.subExpressions)
        {
            for (auto subExpression : subs.second)
            {
                std::cout << InvSymbolNameMap.find(subs.first)->second << " ";
                for (auto symbol : subExpression)
                {
                    std::cout << InvSymbolNameMap.find(symbol)->second << " ";
                }
                std::cout << "|" << std::endl;
            }
        }
    }
}

void ElimLeftRecur()
{
    std::vector<int> symbolIds;
    for (auto symbol : Symbols)
    {
        symbolIds.push_back(symbol.first);
    }
    for (int i = 0; i < symbolIds.size(); i++)
    {
        int curId = symbolIds[i];
        Symbol &symbol = Symbols.find(curId)->second;
        for (int j = 0; j < i; j++)
        {
            int id = symbolIds[j];
            if (Symbols.find(id)->second.type == TERMI)
                continue;
            auto it = symbol.subExpressions.find(id);
            if (it != symbol.subExpressions.end())
            {
                auto stExpressions = Symbols.find(id)->second.subExpressions;
                auto subExpressions = it->second;
                symbol.subExpressions.erase(id);
                for (auto stExpression : stExpressions)
                {
                    int st = stExpression.first;
                    auto newIt = symbol.subExpressions.find(st);
                    if (newIt == symbol.subExpressions.end())
                    {
                        std::vector<Expression> newSubExpression;
                        newSubExpression.clear();
                        symbol.subExpressions.insert(std::pair<int, std::vector<Expression>>(st, newSubExpression));
                        newIt = symbol.subExpressions.find(st);
                    }
                    for (auto expression : stExpression.second)
                    {
                        for (auto subExpression : subExpressions)
                        {
                            auto expressionCopy = expression;
                            for (auto subSymbol : subExpression)
                            {
                                expressionCopy.push_back(subSymbol);
                            }
                            newIt->second.push_back(expressionCopy);
                        }
                    }
                }
            }
        }
        auto badExpressionsIt = symbol.subExpressions.find(curId);
        if (badExpressionsIt != symbol.subExpressions.end())
        {
            auto badExpressions = badExpressionsIt->second;
            symbol.subExpressions.erase(curId);

            int newSymbolId = ++SymbolID;
            std::string newSymbolName = InvSymbolNameMap.find(curId)->second + "_" + std::to_string(1);
            SymbolNameMap.insert(std::pair<std::string, int>(newSymbolName, newSymbolId));
            InvSymbolNameMap.insert(std::pair<int, std::string>(newSymbolId, newSymbolName));
            Symbol newSymbol;
            newSymbol.id = newSymbolId;
            newSymbol.type = NON_TERMI;
            Expression epsExpression;
            epsExpression.clear();
            std::vector<Expression> epsExpressions;
            epsExpressions.clear();
            epsExpressions.push_back(epsExpression);
            newSymbol.subExpressions.insert(std::pair<int, std::vector<Expression>>(EPS, epsExpressions));
            for (auto badExpression : badExpressions)
            {
                int st = badExpression[0];
                badExpression.erase(badExpression.begin());
                badExpression.push_back(newSymbolId);
                auto it = newSymbol.subExpressions.find(st);
                if (it == newSymbol.subExpressions.end())
                {
                    std::vector<Expression> expressions;
                    expressions.push_back(badExpression);
                    newSymbol.subExpressions.insert(std::pair<int, std::vector<Expression>>(st, expressions));
                }
                else
                {
                    it->second.push_back(badExpression);
                }
            }
            Symbols.insert(std::pair<int, Symbol>(newSymbolId, newSymbol));

            for (auto goodExpressionsIt = symbol.subExpressions.begin(); goodExpressionsIt != symbol.subExpressions.end(); goodExpressionsIt++)
            {
                std::vector<Expression> &goodExpressions = goodExpressionsIt->second;
                for (auto goodExpressionIt = goodExpressions.begin(); goodExpressionIt != goodExpressions.end(); goodExpressionIt++)
                {
                    goodExpressionIt->push_back(newSymbolId);
                }
            }
        }
    }
}

int main()
{
    SymbolNameMap.insert(std::pair<std::string, int>("EPS", 0));
    InvSymbolNameMap.insert(std::pair<int, std::string>(0, "EPS"));
    GenSymbols("test.txt");
    Show();
    std::cout << "------------------------------------" << std::endl;
    ElimLeftRecur();
    Show();
    return 0;
}