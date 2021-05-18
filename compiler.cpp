#include "lexer.h"
#include "parser.h"
#include <iostream>

std::string Fomatting(std::string code)
{
    std::string ret;
    int lBracketCnt = 0;
    std::string line = "";
    std::istringstream codeStream(code);
    while (std::getline(codeStream, line))
    {
        int newLBracketCnt = lBracketCnt;
        for (char c : line)
        {
            if (c == '{')
            {
                newLBracketCnt++;
            }
            else if (c == '}')
            {
                newLBracketCnt--;
            }
        }
        ret += newLBracketCnt > lBracketCnt
                   ? std::string(lBracketCnt * 4, ' ')
                   : std::string(newLBracketCnt * 4, ' ');
        ret += line + "\n";

        lBracketCnt = newLBracketCnt;
    }
    return ret;
}

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
        std::cout << Fomatting(ast.GenCCode());
    }

    return 0;
}