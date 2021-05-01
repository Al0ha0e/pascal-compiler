#include <map>
#include <set>
#include <iostream>
#include <vector>
#include <stack>
#include <algorithm>
#include "tools.h"

std::map<int, std::set<int>> FirstSet;
std::map<int, std::set<int>> FollowSet;

void GenSingleFirst(int);

void GenFirst()
{
    for (auto symbol : Symbols)
    {
        int id = symbol.first;
        if (symbol.second.type == TERMI)
        {
            FirstSet.insert(std::pair<int, std::set<int>>(id, std::set<int>()));
            FirstSet.find(id)->second.insert(id);
            continue;
        }
        if (FirstSet.find(id) == FirstSet.end())
        {
            GenSingleFirst(id);
        }
    }
}

void GenSingleFirst(int id)
{
    //std::cout << "NOW " << InvSymbolNameMap.find(id)->second << std::endl;
    Symbol &symbol = Symbols.find(id)->second;
    std::set<int> firstSet;
    for (auto subExpressions : symbol.subExpressions)
    {
        bool mayEps = true;
        auto subExpression = subExpressions.second[0];
        subExpression.insert(subExpression.begin(), subExpressions.first);
        for (int subSymbolId : subExpression)
        {
            Symbol &subSymbol = Symbols.find(subSymbolId)->second;
            if (subSymbol.type == TERMI)
            {
                if (subSymbolId != 0)
                {
                    firstSet.insert(subSymbolId);
                }
                mayEps &= subSymbolId == 0;
            }
            else
            {
                auto it = FirstSet.find(subSymbolId);
                if (it == FirstSet.end())
                {
                    GenSingleFirst(subSymbolId);
                    it = FirstSet.find(subSymbolId);
                }
                auto stFirstSet = it->second;
                bool NoEps = true;
                for (int firstSymbol : stFirstSet)
                {
                    if (firstSymbol)
                    {
                        firstSet.insert(firstSymbol);
                    }
                    else
                    {
                        NoEps = false;
                    }
                }
                mayEps &= (!NoEps);
            }
            if (!mayEps)
                break;
        }
        if (mayEps)
        {
            firstSet.insert(0);
        }
    }
    FirstSet.insert(std::pair<int, std::set<int>>(id, firstSet));
}

std::map<int, std::set<int>> FollowDependency;

void GenFollow1()
{
    for (auto symbolIt : Symbols)
    {
        for (auto subExpressions : symbolIt.second.subExpressions)
        {
            Expression subExpression = subExpressions.second[0];
            subExpression.insert(subExpression.begin(), subExpressions.first);
            bool toEnd = true;
            std::set<int> tempFollow;
            for (int i = subExpression.size() - 1; i >= 0; --i)
            {
                int subSymbolId = subExpression[i];
                std::set<int> &subFirstSet = FirstSet.find(subSymbolId)->second;
                Symbol subSymbol = Symbols.find(subSymbolId)->second;
                if (subSymbol.type == NON_TERMI)
                {
                    std::set<int> &subFollowSet = FollowSet.find(subSymbolId)->second;
                    for (int follow : tempFollow)
                        subFollowSet.insert(follow);
                    if (toEnd)
                    {
                        auto dependencyIt = FollowDependency.find(subSymbolId);
                        if (dependencyIt == FollowDependency.end())
                        {
                            FollowDependency.insert(std::pair<int, std::set<int>>(subSymbolId, std::set<int>()));
                            dependencyIt = FollowDependency.find(subSymbolId);
                        }
                        dependencyIt->second.insert(symbolIt.first);
                    }
                }
                if (subFirstSet.find(EPS) == subFirstSet.end())
                {
                    toEnd = false;
                    tempFollow.clear();
                }
                for (int first : subFirstSet)
                {
                    if (first != EPS)
                    {
                        tempFollow.insert(first);
                    }
                }
            }
        }
    }
}

static int depth;
static std::map<int, int> dfn;
static std::map<int, int> low;
static std::set<int> instk;
static std::stack<int> stk;

void GenFollow2(int id)
{
    dfn.insert(std::pair<int, int>(id, ++depth));
    low.insert(std::pair<int, int>(id, depth));
    instk.insert(id);
    stk.push(id);
    auto it = FollowDependency.find(id);
    std::set<int> &curFollowSet = FollowSet.find(id)->second;
    if (it != FollowDependency.end())
    {
        for (int nxt : it->second)
        {
            int &curlow = low.find(id)->second;
            if (dfn.find(nxt) == dfn.end())
            {
                GenFollow2(nxt);
                curlow = std::min(curlow, low.find(nxt)->second);
            }
            else if (instk.find(nxt) != instk.end())
            {
                curlow = std::min(curlow, dfn.find(nxt)->second);
            }
            for (int follow : FollowSet.find(nxt)->second)
                curFollowSet.insert(follow);
        }
        if (dfn.find(id)->second == low.find(id)->second)
        {
            std::set<int> tmpFollow;
            std::vector<int> components;
            while (1)
            {
                int component = stk.top();
                stk.pop();
                components.push_back(component);
                instk.erase(instk.find(component));
                for (int follow : FollowSet.find(component)->second)
                    tmpFollow.insert(follow);
                if (component == id)
                    break;
            }
            for (int component : components)
            {
                std::set<int> &componentFollowSet = FollowSet.find(component)->second;
                for (int follow : tmpFollow)
                    componentFollowSet.insert(follow);
            }
        }
    }
}

void GenFollow(int startId)
{
    for (auto symbolIt : Symbols)
    {
        if (symbolIt.second.type == TERMI)
            continue;
        FollowSet.insert(std::pair<int, std::set<int>>(symbolIt.first, std::set<int>()));
    }
    FollowSet.find(startId)->second.insert(LINE_END);
    GenFollow1();
    std::cout << "OK1" << std::endl;
    for (auto symbolIt : Symbols)
    {
        if (symbolIt.second.type == NON_TERMI && dfn.find(symbolIt.first) == dfn.end())
            GenFollow2(symbolIt.first);
    }
}

std::set<int> GetFirst(Expression &expression)
{
    std::set<int> ret;
    bool allEPS = true;
    for (int id : expression)
    {
        Symbol &symbol = Symbols.find(id)->second;
        std::set<int> &subFirst = FirstSet.find(id)->second;
        bool noEPS = true;
        for (int first : subFirst)
        {
            if (first == EPS)
            {
                noEPS = false;
            }
            else
            {
                ret.insert(first);
            }
        }
        if (noEPS)
        {
            allEPS = false;
            break;
        }
    }
    if (allEPS)
    {
        ret.insert(EPS);
    }
    return ret;
}

std::map<int, std::map<int, int>> LL1Table;

void GenLL1()
{
    std::vector<int> termiIds;
    std::vector<int> nonTermiIds;

    for (auto symbolIt : Symbols)
    {
        if (symbolIt.first == EPS)
            continue;
        Symbol &symbol = symbolIt.second;
        if (symbol.type == TERMI)
        {
            termiIds.push_back(symbolIt.first);
        }
        else
        {
            nonTermiIds.push_back(symbolIt.first);
        }
    }

    for (int nonTermiId : nonTermiIds)
    {
        std::map<int, int> ll1Map;
        Symbol &symbol = Symbols.find(nonTermiId)->second;
        std::set<int> &followSet = FollowSet.find(nonTermiId)->second;
        for (auto subExpressionsIt : symbol.subExpressions)
        {
            Expression subExpression = subExpressionsIt.second[0];
            subExpression.insert(subExpression.begin(), subExpressionsIt.first);
            std::set<int> firstSet = GetFirst(subExpression);
            std::cout << "FOR SUB EXPRESSION ";
            for (int sb : subExpression)
                std::cout << InvSymbolNameMap.find(sb)->second << " ";
            std::cout << std::endl;
            for (int first : firstSet)
            {
                if (first == EPS)
                {
                    for (int follow : followSet)
                    {
                        if (ll1Map.find(follow) != ll1Map.end())
                        {
                            std::cout << "LL1 ERROR FOLLOW " << InvSymbolNameMap.find(nonTermiId)->second << " " << InvSymbolNameMap.find(follow)->second << std::endl;
                            int st = ll1Map.find(follow)->second;
                            std::cout << "EXIST " << InvSymbolNameMap.find(st)->second;
                            Expression expression = symbol.subExpressions.find(st)->second[0];
                            for (int subSymbolId : expression)
                                std::cout << InvSymbolNameMap.find(subSymbolId)->second;
                            std::cout << std::endl;
                            return;
                        }
                        std::cout << "LL1 INSERT FOLLOW " << InvSymbolNameMap.find(nonTermiId)->second << " " << InvSymbolNameMap.find(follow)->second << std::endl;
                        ll1Map.insert(std::pair<int, int>(follow, subExpressionsIt.first));
                    }
                }
                else
                {
                    if (ll1Map.find(first) != ll1Map.end())
                    {
                        std::cout << "LL1 ERROR FIRST " << InvSymbolNameMap.find(nonTermiId)->second << " " << InvSymbolNameMap.find(first)->second << std::endl;
                        int st = ll1Map.find(first)->second;
                        std::cout << "EXIST " << InvSymbolNameMap.find(st)->second;
                        Expression expression = symbol.subExpressions.find(st)->second[0];
                        for (int subSymbolId : expression)
                            std::cout << InvSymbolNameMap.find(subSymbolId)->second;
                        std::cout << std::endl;
                        return;
                    }
                    std::cout << "LL1 INSERT FIRST " << InvSymbolNameMap.find(nonTermiId)->second << " " << InvSymbolNameMap.find(first)->second << std::endl;
                    ll1Map.insert(std::pair<int, int>(first, subExpressionsIt.first));
                }
            }
        }
        LL1Table.insert(std::pair<int, std::map<int, int>>(nonTermiId, ll1Map));
    }
}

void ShowLL1Table()
{
    for (auto symbolIt : Symbols)
    {
        if (symbolIt.second.type == TERMI)
            continue;
        int symbolId = symbolIt.first;
        Symbol &symbol = symbolIt.second;
        std::cout << InvSymbolNameMap.find(symbolId)->second << "----------" << std::endl;
        for (auto sb : LL1Table.find(symbolId)->second)
        {
            std::cout << InvSymbolNameMap.find(sb.first)->second << " :: " << InvSymbolNameMap.find(sb.second)->second;
            Expression expression = symbol.subExpressions.find(sb.second)->second[0];
            for (int subSymbolId : expression)
                std::cout << InvSymbolNameMap.find(subSymbolId)->second;
            std::cout << std::endl;
        }
    }
}