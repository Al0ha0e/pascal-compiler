#ifndef TOOLS
#define TOOLS

#include <string>
#include <map>
#include <vector>
#include <set>
#include <limits.h>

namespace Tools
{

    const int EPS = 0;
    const int LINE_END = INT_MAX;

    typedef std::vector<int> Expression;
    typedef std::vector<int> LL1Item;

    enum SymbolType
    {
        NON_TERMI,
        TERMI
    };

    struct Symbol
    {
        int id;
        SymbolType type;
        std::map<int, std::vector<Expression>> subExpressions;

        Symbol()
        {
        }
        Symbol(int sid, SymbolType tp) : id(sid), type(tp)
        {
        }
    };

    void GenSymbols(const std::string &pth);

    void ElimLeftRecur();

    void CombineLeftCommon();

    void Show();

    void GenFirst();

    void GenFollow(int);

    void GenLL1();

    void ShowLL1Table();

    void SaveLL1Table(std::string);
    void LoadLL1Table(std::string);

    extern std::map<std::string, int> SymbolNameMap;
    extern std::map<int, std::string> InvSymbolNameMap;
    extern std::map<int, Symbol> Symbols;
    extern std::map<int, std::set<int>> FirstSet;
    extern std::map<int, std::set<int>> FollowSet;
    extern std::map<int, std::map<int, LL1Item>> LL1Table;

    inline void InsertSymbolId(int id, std::string name)
    {
        SymbolNameMap.insert(std::pair<std::string, int>(name, id));
        InvSymbolNameMap.insert(std::pair<int, std::string>(id, name));
    }

    inline void AddConstantSymbols()
    {
        InsertSymbolId(EPS, "EPS");
        Symbol epsSymbol(EPS, TERMI);
        Symbols.insert(std::pair<int, Symbol>(EPS, epsSymbol));

        InsertSymbolId(LINE_END, "$");
        Symbol endSymbol(LINE_END, TERMI);
        Symbols.insert(std::pair<int, Symbol>(LINE_END, endSymbol));
    }
}
#endif