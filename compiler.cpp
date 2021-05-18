#include "lexer.h"
#include "parser.h"
#include "libs/cmdline.h"
#include <iostream>
#include <fstream>

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
    // Create a command line parser
    cmdline::parser args;
    args.add<std::string>("input", 'i', "path of Pascal source code", true);
    args.add<std::string>("output", 'o', "path of saving C object code", false, "out.c");
    args.add("tree", 't', "outputting AST to file");
    args.parse_check(argc, argv);

    CompilerFront::Parser parser(
        args.get<std::string>("input"),
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
    if (args.exist("tree"))
    {
        // output AST to file
        std::ofstream fout(args.get<std::string>("input") + ".tree.txt");
        std::streambuf *oldcout = std::cout.rdbuf(fout.rdbuf());
        ast.astRoot->FormatShow(0);
        std::cout.rdbuf(oldcout);
        fout.close();
    }
    std::cout << "-----------PARSE EN-----------" << std::endl;
    if (ast.Check())
    {
        // output C code to file
        std::ofstream fout(args.get<std::string>("output"));
        fout << Fomatting(ast.GenCCode());
        fout.close();
    }

    return 0;
}