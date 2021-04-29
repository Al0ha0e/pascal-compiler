#ifndef TOOLS
#define TOOLS

#include<string>
#include<map>
#include<vector>

const int EPS = 0;

typedef std::vector<int> Expression;

enum SymbolType{
    NON_TERMI,
    TERMI
};

struct Symbol{
    int id;
    SymbolType type;
    std::map<int,std::vector<Expression>> subExpressions;
};

inline void InsertSymbolId(int,std::string);

void GenSymbols(const std::string &pth);

void CombineLeftCommon();

void Show();

void GenFirst();

extern std::map<std::string,int> SymbolNameMap;
extern std::map<int,std::string> InvSymbolNameMap;
extern std::map<int,Symbol> Symbols;

#endif