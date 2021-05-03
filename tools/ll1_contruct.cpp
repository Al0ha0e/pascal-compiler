#include <map>
#include <set>
#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include <stack>
#include <algorithm>
#include <cstdlib>
#include "tools.h"

namespace Tools
{

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
                        if (toEnd && symbolIt.first != subSymbolId)
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
    static std::set<int> visit;
    static std::map<int, int> dfn;
    static std::map<int, int> low;
    static std::set<int> instk;
    static std::stack<int> stk;

    void GenFollow2(int id)
    {
        visit.insert(id);
        dfn.insert(std::pair<int, int>(id, ++depth));
        low.insert(std::pair<int, int>(id, depth));
        instk.insert(id);
        stk.push(id);
        auto it = FollowDependency.find(id);
        std::set<int> &curFollowSet = FollowSet.find(id)->second;
        if (it != FollowDependency.end())
        {
            // std::cout << "NOW " << InvSymbolNameMap.find(id)->second << std::endl;
            // for (int nxt : it->second)
            // {
            //     std::cout << InvSymbolNameMap.find(nxt)->second << " ";
            // }
            // std::cout << std::endl;
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
        }
        if (dfn.find(id)->second == low.find(id)->second)
        {
            std::set<int> tmpFollow;
            std::vector<int> components;
            //std::cout << "<<<<<<<<<<<<<<<<<<COMPONENTS>>>>>>>>>>>>>>>>" << std::endl;
            while (1)
            {
                int component = stk.top();
                //std::cout << InvSymbolNameMap.find(component)->second << " " << std::endl;
                stk.pop();
                components.push_back(component);
                instk.erase(instk.find(component));
                for (int follow : FollowSet.find(component)->second)
                    tmpFollow.insert(follow);
                if (component == id)
                    break;
            }
            //std::cout << "<<<<<<<<<<<<<<<<END COMPONENTS>>>>>>>>>>>>>" << std::endl;
            for (int component : components)
            {
                std::set<int> &componentFollowSet = FollowSet.find(component)->second;
                for (int follow : tmpFollow)
                    componentFollowSet.insert(follow);
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
        // std::cout << "OK1" << std::endl;
        // std::cout << "-----------------AFTER OK1-----------------" << std ::endl;
        // for (auto fs : FollowSet)
        // {
        //     std::cout << InvSymbolNameMap.find(fs.first)->second << "---" << std::endl;
        //     for (int i : fs.second)
        //     {
        //         std::cout << InvSymbolNameMap.find(i)->second << " ";
        //     }
        //     std::cout << std::endl;
        // }
        // std::cout << "-----------------DEP-----------------" << std ::endl;
        // for (auto dep : FollowDependency)
        // {
        //     std::cout << InvSymbolNameMap.find(dep.first)->second << "---" << std::endl;
        //     for (int i : dep.second)
        //     {
        //         std::cout << InvSymbolNameMap.find(i)->second << " ";
        //     }
        //     std::cout << std::endl;
        // }
        // std::cout << "----------------AFTER OK END---------------" << std::endl;
        for (auto symbolIt : Symbols)
        {
            if (symbolIt.second.type == NON_TERMI && visit.find(symbolIt.first) == visit.end())
            {
                depth = 0;
                dfn.clear();
                low.clear();
                instk.clear();
                while (!stk.empty())
                    stk.pop();
                GenFollow2(symbolIt.first);
            }
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

    std::map<int, std::map<int, LL1Item>> LL1Table;

    void GenLL1()
    {
        std::vector<int> nonTermiIds;

        for (auto symbolIt : Symbols)
        {
            if (symbolIt.first == EPS)
                continue;
            Symbol &symbol = symbolIt.second;
            if (symbol.type == NON_TERMI)
                nonTermiIds.push_back(symbolIt.first);
        }

        for (int nonTermiId : nonTermiIds)
        {
            std::map<int, LL1Item> ll1Map;
            Symbol &symbol = Symbols.find(nonTermiId)->second;
            std::set<int> &followSet = FollowSet.find(nonTermiId)->second;
            std::cout << "CONSTRUCT FOR " << InvSymbolNameMap.find(nonTermiId)->second << "------------" << std::endl;
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
                            auto itemIt = ll1Map.find(follow);
                            if (itemIt != ll1Map.end())
                            {
                                std::cout << "@@@@@LL1 ERROR FOLLOW " << InvSymbolNameMap.find(nonTermiId)->second << " " << InvSymbolNameMap.find(follow)->second << std::endl;
                                // int st = ll1Map.find(follow)->second;

                                // std::cout << "EXIST " << InvSymbolNameMap.find(st)->second << " ";
                                // Expression expression = symbol.subExpressions.find(st)->second[0];
                                // for (int subSymbolId : expression)
                                //     std::cout << InvSymbolNameMap.find(subSymbolId)->second << " ";
                                // std::cout << std::endl;
                                // return;
                                itemIt->second.push_back(subExpressionsIt.first);
                            }
                            else
                            {
                                std::cout << "LL1 INSERT FOLLOW " << InvSymbolNameMap.find(nonTermiId)->second << " " << InvSymbolNameMap.find(follow)->second << std::endl;
                                LL1Item item;
                                item.push_back(subExpressionsIt.first);
                                ll1Map.insert(std::pair<int, LL1Item>(follow, item));
                            }
                        }
                    }
                    else
                    {
                        auto itemIt = ll1Map.find(first);
                        if (itemIt != ll1Map.end())
                        {
                            std::cout << "@@@@@LL1 ERROR FIRST " << InvSymbolNameMap.find(nonTermiId)->second << " " << InvSymbolNameMap.find(first)->second << std::endl;
                            // int st = ll1Map.find(first)->second;
                            // std::cout << "EXIST " << InvSymbolNameMap.find(st)->second << " ";
                            // Expression expression = symbol.subExpressions.find(st)->second[0];
                            // for (int subSymbolId : expression)
                            //     std::cout << InvSymbolNameMap.find(subSymbolId)->second << " ";
                            // std::cout << std::endl;
                            // return;
                            itemIt->second.push_back(subExpressionsIt.first);
                        }
                        else
                        {
                            std::cout << "LL1 INSERT FIRST " << InvSymbolNameMap.find(nonTermiId)->second << " " << InvSymbolNameMap.find(first)->second << std::endl;
                            LL1Item item;
                            item.push_back(subExpressionsIt.first);
                            ll1Map.insert(std::pair<int, LL1Item>(first, item));
                        }
                    }
                }
            }
            LL1Table.insert(std::pair<int, std::map<int, LL1Item>>(nonTermiId, ll1Map));
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
            for (auto items : LL1Table.find(symbolId)->second)
            {
                for (auto item : items.second)
                {
                    std::cout << InvSymbolNameMap.find(items.first)->second << " :: " << InvSymbolNameMap.find(item)->second;
                    Expression expression = symbol.subExpressions.find(item)->second[0];
                    for (int subSymbolId : expression)
                        std::cout << " " << InvSymbolNameMap.find(subSymbolId)->second;
                    std::cout << std::endl;
                }
            }
        }
    }

    void SaveLL1Table(std::string path)
    {
        std::string s;
        for (auto symbolIt : Symbols)
        {
            if (symbolIt.second.type == TERMI)
                continue;
            int symbolId = symbolIt.first;
            s += InvSymbolNameMap.find(symbolId)->second + "\n";
            for (auto items : LL1Table.find(symbolId)->second)
            {
                if (!items.second.size())
                    continue;
                std::string itemStr = InvSymbolNameMap.find(items.first)->second;
                for (auto item : items.second)
                    itemStr += " " + InvSymbolNameMap.find(item)->second;
                s += itemStr + "\n";
            }
        }
        std::ofstream ofile(path);
        ofile << s;
        ofile.close();
    }

    void LoadLL1Table(std::string path)
    {
        std::ifstream f(path);
        std::string line = "";
        int curSymbol;
        while (std::getline(f, line))
        {
            std::istringstream itemStream(line);
            std::vector<std::string> items;
            std::string temp;
            while (std::getline(itemStream, temp, ' '))
            {
                if (temp.length())
                    items.push_back(temp);
            }
            if (items.size() == 1)
            {
                curSymbol = SymbolNameMap.find(items[0])->second;
                LL1Table.insert(std::pair<int, std::map<int, LL1Item>>(curSymbol, std::map<int, LL1Item>()));
            }
            else if (items.size())
            {
                int sb = SymbolNameMap.find(items[0])->second;
                LL1Item item;
                for (int i = 1; i < items.size(); i++)
                    item.push_back(SymbolNameMap.find(items[i])->second);
                LL1Table.find(curSymbol)->second.insert(std::pair<int, LL1Item>(sb, item));
            }
        }
        f.close();
    }

    void GenSyntaxTemplate()
    {
    }
}