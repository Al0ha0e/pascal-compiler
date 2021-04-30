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

void GenLL1()
{
}