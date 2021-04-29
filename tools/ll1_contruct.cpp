#include<map>
#include<set>
#include<iostream>
#include<vector>
#include "tools.h"

std::map<int,std::set<int>> FirstSet;

void GenSingleFirst(int);

void GenFirst(){
    for(auto symbol: Symbols){
        if(symbol.second.type == TERMI)
            continue;
        int id = symbol.first;
        if(FirstSet.find(id)==FirstSet.end()){
            GenSingleFirst(id);
        }
    }
}

void GenSingleFirst(int id){
    Symbol &symbol = Symbols.find(id)->second;
    std::set<int> firstSet;
    for(auto subExpressions: symbol.subExpressions){
        bool mayEps = true;
        auto subExpression = subExpressions.second[0];
        subExpression.insert(subExpression.begin(),subExpressions.first);
        for(int subSymbolId: subExpression){
            Symbol &subSymbol = Symbols.find(subSymbolId)->second;
            if(subSymbol.type == TERMI){
                if(subSymbolId != 0){
                    firstSet.insert(subSymbolId);
                }
                mayEps &= subSymbolId == 0;
            }else{
                auto it = FirstSet.find(subSymbolId);
                if(it == FirstSet.end()){
                    GenSingleFirst(subSymbolId);
                    it = FirstSet.find(subSymbolId);
                }
                auto stFirstSet = it->second;
                bool NoEps = true;
                for(int firstSymbol: stFirstSet){
                    if(firstSymbol){
                        firstSet.insert(firstSymbol);
                    }else{
                        NoEps = false;
                    }
                }
                mayEps &= (!NoEps);
            }
            if(!mayEps) break;
        }
        if(mayEps){
            firstSet.insert(0);
        }
    }
    FirstSet.insert(std::pair<int,std::set<int>>(id,firstSet));
}

int main(){
    InsertSymbolId(0,"EPS");
    Symbol epsSymbol;
    epsSymbol.type = TERMI;
    epsSymbol.id = 0;
    Symbols.insert(std::pair<int,Symbol>(0,epsSymbol));
    GenSymbols("test.txt");
    CombineLeftCommon();
    Show();
    GenFirst();
    std::cout<<"-----------------------"<<std::endl;
    for(auto fs: FirstSet){
        std::cout<<InvSymbolNameMap.find(fs.first)->second<<"---"<<std::endl;
        for(int i:fs.second){
            std::cout<<InvSymbolNameMap.find(i)->second<<" ";
        }
        std::cout<<std::endl;
    }
    return 0;
}