#include "lexer.h"
#include "parser.h"
#include <iostream>

int main()
{

    CompilerFront::Parser parser(
        "./test.txt",
        "./tools/reduced.txt",
        "./tools/ll1_table.txt",
        "./tools/sync.txt",
        "programstruct");
    std::cout << "---------------------------" << std::endl;
    // Tools::ShowLL1Table();
    std::cout << "PARSE ST" << std::endl;
    bool ok;
    PascalAST::AbstractSyntaxTree ast(parser.Parse(ok));
    if (!ok)
        return 0;
    // ast.astRoot->Show();
    std::cout << "PARSE OVER" << std::endl;
    if (ast.Check())
    {
        std::cout << ast.GenCCode();
    }

    return 0;
}