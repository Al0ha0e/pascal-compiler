#include "lexer.h"
#include "parser.h"
#include <iostream>
using namespace CompilerFront;

int main()
{

    Parser parser("./test.txt", "./tools/grammar.txt", "./tools/ll1_table.txt", "programstruct");
    std::cout << "---------------------------" << std::endl;
    Tools::ShowLL1Table();
    std::cout << "PARSE ST" << std::endl;
    parser.Parse();
    return 0;
}