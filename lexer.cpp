#include "lexer.h"
#include <set>

namespace CompilerFront
{

    std::set<char> ValidPunct = {
        ' ', '\n', '(', ')', ',', ';', '[', ']', '=', '+',
        '-', '\'', '*', '/', ':', '.', '<', '>', '{'};

    std::set<char> SinglePunct = {'(', ')', ',', ';', '[', ']', '=', '+', '-'};

    std::set<std::string> Keywords = {"not", "program", "const", "var", "procedure",
                                      "function", "begin", "end", "array", "of", "integer",
                                      "real", "boolean", "char", "if", "then", "else",
                                      "for", "to", "do", "read", "write", "or"};
    std::set<std::string> Mulop = {"div", "mod", "and"};

    inline bool isDigit(char c)
    {
        return c >= '0' && c <= '9';
    }

    inline bool isAlpha(char c)
    {
        return (c >= 'a' && c <= 'z') || (c >= 'A' && c <= 'Z');
    }

    Token Lexer::GetToken()
    {
        if (pos >= contentLength)
            return Token("$", "$", line, column);
        char curChar = content[pos];
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
        while (curChar == ' ' || curChar == '\n' || curChar == '{')
        {
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

            if (curChar == '{')
            {
                bool inComment = true;
                while (inComment)
                {
                    if (curChar == '\n')
                    {
                        column = 1;
                        line++;
                    }
                    else
                    {
                        column++;
                        if (curChar == '}')
                            inComment = false;
                    }
                    if (++pos >= contentLength)
                        return Token("$", "$", line, column);
                    curChar = content[pos];
                }
            }
            while (!isDigit(curChar) && !isAlpha(curChar) && ValidPunct.find(curChar) == ValidPunct.end())
            {
                std::cout << "line: " << line << " column: " << column
                          << " Lexer Error: unexpected character " << curChar << std::endl;
                column++;
                if (++pos >= contentLength)
                    return Token("$", "$", line, column);
                curChar = content[pos];
            }
        }
        //TODO ABNORMAL CHARACTER
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
        while (isDigit(curChar))
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
            if (curChar == '.' && pos + 1 < contentLength && isDigit(content[pos + 1]))
            {
                digitStg = 2;
                retStr += ".";
                curChar = content[++pos];
                column++;
                while (isDigit(curChar))
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

        if (isAlpha(curChar))
        {
            retStr = std::string(1, curChar);
            column++;
            if (++pos >= contentLength)
                return Token("id", retStr, line, stColumn);
            curChar = content[pos];
            while (isAlpha(curChar) || isDigit(curChar))
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
