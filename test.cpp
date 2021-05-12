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
    PascalAST::AbstractSyntaxTree ast(parser.Parse());
    ast.astRoot->Show();
    std::cout << "PARSE OVER" << std::endl;
    if (ast.Check())
    {
        std::cout << ast.GenCCode();
    }

    return 0;
}