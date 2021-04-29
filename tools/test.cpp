#include "tools.h"
#include <iostream>

int main()
{
    AddConstantSymbols();
    GenSymbols("test.txt");
    ElimLeftRecur();
    CombineLeftCommon();
    // Show();
    std::cout << "before show" << std::endl;
    GenFirst();
    std::cout << "-----------------------" << std::endl;
    // for (auto fs : FirstSet)
    // {
    //     std::cout << InvSymbolNameMap.find(fs.first)->second << "---" << std::endl;
    //     for (int i : fs.second)
    //     {
    //         std::cout << InvSymbolNameMap.find(i)->second << " ";
    //     }
    //     std::cout << std::endl;
    // }
    GenFollow(SymbolNameMap.find("A")->second);
    for (auto fs : FollowSet)
    {
        std::cout << InvSymbolNameMap.find(fs.first)->second << "---" << std::endl;
        for (int i : fs.second)
        {
            std::cout << InvSymbolNameMap.find(i)->second << " ";
        }
        std::cout << std::endl;
    }
    return 0;
}