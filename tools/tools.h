#ifndef TOOLS
#define TOOLS

#include <string>
#include <map>
#include <vector>
#include <set>
#include <limits.h>

const int EPS = 0;
const int LINE_END = INT_MAX;

typedef std::vector<int> Expression;

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

inline void InsertSymbolId(int, std::string);

void GenSymbols(const std::string &pth);

void ElimLeftRecur();

void CombineLeftCommon();

void Show();

void GenFirst();

void GenFollow(int);

extern std::map<std::string, int> SymbolNameMap;
extern std::map<int, std::string> InvSymbolNameMap;
extern std::map<int, Symbol> Symbols;
extern std::map<int, std::set<int>> FirstSet;
extern std::map<int, std::set<int>> FollowSet;

inline void AddConstantSymbols()
{
    InsertSymbolId(EPS, "EPS");
    Symbol epsSymbol(EPS, TERMI);
    Symbols.insert(std::pair<int, Symbol>(EPS, epsSymbol));

    InsertSymbolId(LINE_END, "$");
    Symbol endSymbol(LINE_END, TERMI);
    Symbols.insert(std::pair<int, Symbol>(LINE_END, endSymbol));
}

#endif