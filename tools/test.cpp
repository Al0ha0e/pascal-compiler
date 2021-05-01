#include "tools.h"
#include <iostream>

int main()
{
    AddConstantSymbols();
    GenSymbols("test.txt");
    ElimLeftRecur();
    // Show();
    CombineLeftCommon();
    std::cout << "---------------------------" << std::endl;
    Show();
    std::cout << "---------------------------" << std::endl;
    // std::cout << "before show" << std::endl;
    GenFirst();
    std::cout << "---------FIRST--------------" << std::endl;
    for (auto fs : FirstSet)
    {
        std::cout << InvSymbolNameMap.find(fs.first)->second << "---" << std::endl;
        for (int i : fs.second)
        {
            std::cout << InvSymbolNameMap.find(i)->second << " ";
        }
        std::cout << std::endl;
    }
    GenFollow(SymbolNameMap.find("programstruct")->second);
    std::cout << "---------FOLLOW--------------" << std::endl;
    for (auto fs : FollowSet)
    {
        std::cout << InvSymbolNameMap.find(fs.first)->second << "---" << std::endl;
        for (int i : fs.second)
        {
            std::cout << InvSymbolNameMap.find(i)->second << " ";
        }
        std::cout << std::endl;
    }
    std::cout << "---------FOLLOW END--------------" << std::endl;
    GenLL1();
    // std::cout << "---------LL1--------------" << std::endl;
    // ShowLL1Table();
    // std::cout << "OK ALL" << std::endl;
    return 0;
}