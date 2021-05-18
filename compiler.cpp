#include "lexer.h"
#include "parser.h"
#include <iostream>

int main(int argc, char *argv[])
{

    if (argc <= 1)
        return 0;
    std::string code(argv[1]);

    CompilerFront::Parser parser(
        code,
        "./tools/reduced.txt",
        "./tools/ll1_table.txt",
        "./tools/sync.txt",
        "programstruct");
    // Tools::ShowLL1Table();
    std::cout << "-----------PARSE ST-----------" << std::endl;
    bool ok;
    PascalAST::AbstractSyntaxTree ast(parser.Parse(ok));
    if (!ok)
        return 0;
    // ast.astRoot->Show();
    std::cout << "-----------PARSE EN-----------" << std::endl;
    if (ast.Check())
    {
        std::cout << ast.GenCCode();
    }

    return 0;
}