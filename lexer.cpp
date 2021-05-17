#include "lexer.h"
#include <set>

namespace CompilerFront
{

    std::set<char> SinglePunct = {'(', ')', ',', ';', '[', ']',
                                  '=', '+', '-'};

    std::set<std::string> Keywords = {"not", "program", "const", "var", "procedure",
                                      "function", "begin", "end", "array", "of", "integer",
                                      "real", "boolean", "char", "if", "then", "else",
                                      "for", "to", "do", "read", "write", "or"};
    std::set<std::string> Mulop = {"div", "mod", "and"};

    Token Lexer::GetToken()
    {
        if (pos >= contentLength)
            return Token("$", "$", line, column);
        char curChar = content[pos];
        while (curChar == ' ' || curChar == '\n')
        {
            if (curChar == ' ')
            {
                column++;
            }
            else if (curChar == '\n')
            {
                column = 1;
                line++;
            }
            if (++pos >= contentLength)
                return Token("$", "$", line, column);
            curChar = content[pos];
        }
        //TODO ABNORMAL CHARACTER
        if (charST)
        {
            pos++;
            if (curChar == '\'')
            {
                charST = false;
                return Token(std::string(1, curChar), std::string(1, curChar), line, column++);
            }
            return Token("letter", std::string(1, curChar), line, column++);
        }
        if (curChar == '{') //TODO DEBUG
        {
            bool inComment = true;
            while (inComment)
            {
                column++;
                if (curChar == '\n')
                {
                    column = 1;
                    line++;
                }
                else if (curChar == '}')
                    inComment = false;
                if (++pos >= contentLength)
                    return Token("$", "$", line, column);
            }
        }
        if (SinglePunct.find(curChar) != SinglePunct.end())
        {
            pos++;
            return Token(std::string(1, curChar), std::string(1, curChar), line, column++);
        }
        if (curChar == '\'')
        {
            pos++;
            charST = true;
            return Token(std::string(1, curChar), std::string(1, curChar), line, column++);
        }
        if (curChar == '*' || curChar == '/')
        {
            pos++;
            return Token("mulop", std::string(1, curChar), line, column++);
        }
        if (curChar == ':')
        {
            if (++pos >= contentLength || content[pos] != '=')
                return Token(":", ":", line, column++);
            pos++;
            column += 2;
            return Token("assignop", ":=", line, column - 2);
        }
        if (curChar == '.')
        {
            if (++pos >= contentLength || content[pos] != '.')
                return Token(".", ".", line, column++);
            pos++;
            column += 2;
            return Token("..", "..", line, column - 2);
        }
        if (curChar == '<')
        {
            if (++pos >= contentLength || content[pos] != '>' || content[pos] != '=')
                return Token("relop", "<", line, column++);
            std::string retStr = content[pos] == '>' ? "<>" : "<=";
            pos++;
            column += 2;
            return Token("relop", retStr, line, column - 2);
        }
        if (curChar == '>')
        {
            if (++pos >= contentLength || content[pos] != '=')
                return Token("relop", ">", line, column++);
            pos++;
            column += 2;
            return Token("relop", ">=", line, column - 2);
        }
        std::string retStr;
        int digitStg = 0;
        int stColumn = column;
        while (curChar >= '0' && curChar <= '9')
        {
            digitStg = 1;
            retStr += std::string(1, curChar);
            column++;
            if (++pos >= contentLength)
                break;
            curChar = content[pos];
        }
        if (digitStg)
        {
            if (curChar == '.' && pos + 1 < contentLength && content[pos + 1] >= '0' && content[pos + 1] <= '9')
            {
                digitStg = 2;
                retStr += ".";
                curChar = content[++pos];
                column++;
                while (curChar >= '0' && curChar <= '9')
                {
                    retStr += std::string(1, curChar);
                    column++;
                    if (++pos >= contentLength)
                        break;
                    curChar = content[pos];
                }
            }
        }

        if (digitStg == 1)
            return Token("digits", retStr, line, stColumn);
        if (digitStg == 2)
            return Token("float", retStr, line, stColumn);

        if ((curChar >= 'a' && curChar <= 'z') || (curChar >= 'A' && curChar <= 'Z'))
        {
            retStr = std::string(1, curChar);
            column++;
            if (++pos >= contentLength)
                return Token("id", retStr, line, stColumn);
            curChar = content[pos];
            while ((curChar >= 'a' && curChar <= 'z') || (curChar >= 'A' && curChar <= 'Z') || (curChar >= '0' && curChar <= '9'))
            {
                retStr += std::string(1, curChar);
                column++;
                if (++pos >= contentLength)
                    break;
                curChar = content[pos];
            }
            if (Mulop.find(retStr) != Mulop.end())
                return Token("mulop", retStr, line, stColumn);
            if (Keywords.find(retStr) != Keywords.end())
                return Token(retStr, retStr, line, stColumn);
            return Token("id", retStr, line, stColumn);
        }
        return Token("$", "$", line, column);
    }
}
