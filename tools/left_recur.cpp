#include <map>
#include <string>
#include <memory>
#include <vector>
#include <fstream>
#include <sstream>
#include <iostream>
#include <algorithm>
#include <stack>
#include "tools.h"

namespace Tools
{

    int SymbolID;
    std::map<std::string, int> SymbolNameMap;
    std::map<int, std::string> InvSymbolNameMap;
    std::map<int, Symbol> Symbols;

    void LoadSymbols(const std::string &pth)
    {
        std::ifstream f(pth);
        std::string line = "";
        while (std::getline(f, line))
        {
            // std::cout << line << std::endl;
            std::istringstream lstream(line);
            std::string symbolName;
            int isL = true;
            int lId = 0;
            Expression expression;
            expression.clear();
            while (std::getline(lstream, symbolName, ' '))
            {
                if (!symbolName.length())
                    continue;
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

    void StoreSymbols(const std::string &path)
    {
        std::string retString;
        for (auto symbolIt : Symbols)
        {
            Symbol &symbol = symbolIt.second;
            std::string left = InvSymbolNameMap.find(symbolIt.first)->second;
            for (auto subExpressionIt : symbol.subExpressions)
            {

                for (auto subExpression : subExpressionIt.second)
                {
                    retString += left + " " + InvSymbolNameMap.find(subExpressionIt.first)->second;
                    for (int subSymbol : subExpression)
                        retString += " " + InvSymbolNameMap.find(subSymbol)->second;
                    retString += "\n";
                }
            }
        }
        std::ofstream f(path);
        f << retString;
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

    void PassLeft(std::vector<int> &symbolIds, Symbol &symbol, int en)
    {
        for (int j = 0; j < en; j++)
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
    }

    void ElimSingleLeftRecur(std::vector<int> &symbolIds)
    {
        for (int i = 0; i < symbolIds.size(); i++)
        {
            int curId = symbolIds[i];
            Symbol &symbol = Symbols.find(curId)->second;

            PassLeft(symbolIds, symbol, i);
            auto badExpressionsIt = symbol.subExpressions.find(curId);
            if (badExpressionsIt != symbol.subExpressions.end())
            {
                auto badExpressions = badExpressionsIt->second;
                int newSymbolId = ++SymbolID;
                std::string newSymbolName = InvSymbolNameMap.find(curId)->second + "_" + std::to_string(newSymbolId);
                InsertSymbolId(newSymbolId, newSymbolName);
                Symbol newSymbol(newSymbolId, NON_TERMI);
                symbol.subExpressions.erase(curId);

                for (auto badExpression : badExpressions)
                {
                    if (!badExpression.size())
                        continue;
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

                if (symbol.subExpressions.size() == 0)
                {
                    std::vector<Expression> expressions;
                    expressions.push_back(Expression());
                    symbol.subExpressions.insert(std::pair<int, std::vector<Expression>>(newSymbolId, expressions));
                }
                else
                {
                    Expression epsExpression;
                    std::vector<Expression> epsExpressions;
                    epsExpressions.push_back(epsExpression);
                    newSymbol.subExpressions.insert(std::pair<int, std::vector<Expression>>(EPS, epsExpressions));

                    for (auto goodExpressionsIt = symbol.subExpressions.begin(); goodExpressionsIt != symbol.subExpressions.end(); goodExpressionsIt++)
                    {
                        std::vector<Expression> &goodExpressions = goodExpressionsIt->second;
                        for (auto goodExpressionIt = goodExpressions.begin(); goodExpressionIt != goodExpressions.end(); goodExpressionIt++)
                        {
                            goodExpressionIt->push_back(newSymbolId);
                        }
                    }
                }
                Symbols.insert(std::pair<int, Symbol>(newSymbolId, newSymbol));
            }
        }
    }

    std::map<int, std::set<int>> LeftDependency;

    static int depth;
    static std::map<int, int> dfn;
    static std::map<int, int> low;
    static std::set<int> instk;
    static std::stack<int> stk;

    void ElimLeftRecur1(int id)
    {
        dfn.insert(std::pair<int, int>(id, ++depth));
        low.insert(std::pair<int, int>(id, depth));
        instk.insert(id);
        stk.push(id);
        auto it = LeftDependency.find(id);
        if (it != LeftDependency.end())
        {
            for (int nxt : it->second)
            {
                int &curlow = low.find(id)->second;
                if (dfn.find(nxt) == dfn.end())
                {
                    ElimLeftRecur1(nxt);
                    curlow = std::min(curlow, low.find(nxt)->second);
                }
                else if (instk.find(nxt) != instk.end())
                {
                    curlow = std::min(curlow, dfn.find(nxt)->second);
                }
            }
        }
        if (dfn.find(id)->second == low.find(id)->second)
        {
            std::vector<int> components;
            while (1)
            {
                int component = stk.top();
                stk.pop();
                components.push_back(component);
                instk.erase(instk.find(component));
                if (component == id)
                    break;
            }
            ElimSingleLeftRecur(components);
        }
    }

    //TODO
    void ElimLeftRecur()
    {
        std::vector<int> symbolIds;
        for (auto it : Symbols)
        {
            if (it.second.type == TERMI)
                continue;
            symbolIds.push_back(it.first);
            LeftDependency.insert(std::pair<int, std::set<int>>(it.first, std::set<int>()));
        }
        for (int id : symbolIds)
        {
            Symbol &symbol = Symbols.find(id)->second;
            for (auto subExpressionIt : symbol.subExpressions)
            {
                int firstId = subExpressionIt.first;
                if (firstId == id)
                {
                    std::vector<int> badIds;
                    badIds.push_back(id);
                    ElimSingleLeftRecur(badIds);
                    break;
                }
            }
        }
        for (int id : symbolIds)
        {
            Symbol &symbol = Symbols.find(id)->second;
            for (auto subExpressionIt : symbol.subExpressions)
            {
                int firstId = subExpressionIt.first;
                if (Symbols.find(firstId)->second.type == NON_TERMI)
                {
                    auto depIt = LeftDependency.find(id);
                    depIt->second.insert(firstId);
                }
            }
        }
        for (int id : symbolIds)
            if (dfn.find(id) == dfn.end())
                ElimLeftRecur1(id);
    }

    void CombineSingleLeftCommon(int);

    void CombineLeftCommon()
    {
        std::vector<int> symbolIds;
        for (auto symbol : Symbols)
        {
            if (symbol.second.type == TERMI)
                continue;
            symbolIds.push_back(symbol.first);
        }
        for (int id : symbolIds)
        {
            CombineSingleLeftCommon(id);
        }
    }

    void CombineSingleLeftCommon(int symbolId)
    {
        Symbol &symbol = Symbols.find(symbolId)->second;
        for (auto subExpressionsIt = symbol.subExpressions.begin(); subExpressionsIt != symbol.subExpressions.end(); subExpressionsIt++)
        {
            if (subExpressionsIt->second.size() > 1)
            {
                int newSymbolId = ++SymbolID;
                std::string newSymbolName = InvSymbolNameMap.find(symbolId)->second + "_" + std::to_string(newSymbolId);
                InsertSymbolId(newSymbolId, newSymbolName);
                Symbol newSymbol(newSymbolId, NON_TERMI);
                int maxCommonLen = -1;
                Expression preExpressionRight;
                for (auto subExpression : subExpressionsIt->second)
                {
                    if (maxCommonLen == -1)
                    {
                        maxCommonLen = subExpression.size();
                    }
                    else
                    {
                        int i;
                        for (i = 0; i < std::min(maxCommonLen, int(subExpression.size())); i++)
                        {
                            if (subExpression[i] != preExpressionRight[i])
                            {
                                break;
                            }
                        }
                        maxCommonLen = i;
                        if (maxCommonLen == 0)
                            break;
                    }
                    preExpressionRight = subExpression;
                }
                for (auto subExpression : subExpressionsIt->second)
                {
                    Expression newSubExpression;
                    int st = EPS;
                    if (subExpression.size() > maxCommonLen)
                    {
                        st = subExpression[maxCommonLen];
                        for (int i = maxCommonLen + 1; i < subExpression.size(); i++)
                        {
                            newSubExpression.push_back(subExpression[i]);
                        }
                    }

                    auto it = newSymbol.subExpressions.find(st);
                    if (it == newSymbol.subExpressions.end())
                    {
                        std::vector<Expression> newSubExpressions;
                        newSymbol.subExpressions.insert(std::pair<int, std::vector<Expression>>(st, newSubExpressions));
                        it = newSymbol.subExpressions.find(st);
                    }
                    it->second.push_back(newSubExpression);
                }
                Symbols.insert(std::pair<int, Symbol>(newSymbolId, newSymbol));

                Expression reducedExpression;
                if (preExpressionRight.size() > 0)
                {
                    for (int i = 0; i < maxCommonLen; i++)
                        reducedExpression.push_back(preExpressionRight[i]);
                }
                reducedExpression.push_back(newSymbolId);
                subExpressionsIt->second.clear();
                subExpressionsIt->second.push_back(reducedExpression);

                CombineSingleLeftCommon(newSymbolId);
            }
        }
    }
}