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

    std::cout << "Parsing..." << std::endl;
    bool ok;
    PascalAST::AbstractSyntaxTree ast(parser.Parse(ok));
    if (!ok)
    {
        std::cout << "Fatal error encountered, exiting..." << std::endl;
        return 0;
    }
    std::cout << "Done!" << std::endl;

    // 保存 AST 到文件
    if (args.exist("tree"))
    {
        std::cout << "Saving AST to \"" + args.get<std::string>("input") + ".tree.txt\"..." << std::endl;
        // 将 cout 重定向至文件输出流
        std::ofstream fout(args.get<std::string>("input") + ".tree.txt");
        std::streambuf *oldcout = std::cout.rdbuf(fout.rdbuf());
        ast.astRoot->FormatShow(0);
        // 将 cout 重定向至标准输出流
        std::cout.rdbuf(oldcout);
        fout.close();
        std::cout << "Done!" << std::endl;
    }

    std::cout << "Checking..." << std::endl;
    ok = ast.Check();
    if (!ok)
    {
        std::cout << "Fatal error encountered, exiting..." << std::endl;
        return 0;
    }
    std::cout << "Done!" << std::endl;

    // output C code to file
    std::cout << "Saving C code to \"" + args.get<std::string>("output") + "\"..." << std::endl;
    std::ofstream fout(args.get<std::string>("output"));
    fout << Fomatting(ast.GenCCode());
    fout.close();
    std::cout << "Done!" << std::endl;

    std::cout << "Compilation complete, exiting..." << std::endl;
    return 0;
}