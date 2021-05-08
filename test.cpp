#include "lexer.h"
#include "parser.h"
#include <iostream>
using namespace CompilerFront;

int main()
{

    Parser parser("./test.txt", "./tools/reduced.txt", "./tools/ll1_table.txt", "programstruct");
    std::cout << "---------------------------" << std::endl;
    // Tools::ShowLL1Table();
    std::cout << "PARSE ST" << std::endl;
    auto root(parser.Parse());
    std::cout << "PARSE OVER" << std::endl;
    root->Show();
    return 0;
}