#include "tools.h"
#include <string>
#include <iostream>
using namespace Tools;

int main(int argc, char *argv[])
{
    if (argc <= 1)
        return 0;
    std::string s(argv[1]);
    if (s == "r")
    {
        std::string src(argv[2]);
        std::string dst(argv[3]);
        Tools::AddConstantSymbols();
        Tools::LoadSymbols(src);
        // std::cout << "----------------------------------------" << std::endl;
        // Tools::Show();
        // std::cout << "----------------------------------------" << std::endl;
        Tools::ElimLeftRecur();
        Tools::CombineLeftCommon();
        Tools::StoreSymbols(dst);
    }
    else if (s == "t")
    {
        std::string src(argv[2]);
        std::string dst(argv[3]);
        std::string st(argv[4]);
        Tools::AddConstantSymbols();
        Tools::LoadSymbols(src);
        Tools::GenFirst();
        Tools::GenFollow(Tools::SymbolNameMap.find(st)->second);
        Tools::GenLL1();
        Tools::SaveLL1Table(dst);
    }

    return 0;
}