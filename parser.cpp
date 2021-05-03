#include "parser.h"

namespace CompilerFront
{
    void Parser::Parse()
    {
        std::map<int, std::map<int, Tools::LL1Item>> &ll1Table = Tools::LL1Table;
        curToken = lexer.GetToken();
        while (!symbolStack.empty())
        {
            std::cout << "CURTOKEN " << curToken.type << " " << curToken.content << std::endl;
            int topSymbolId = symbolStack.top();
            std::cout << "STK TOP " << Tools::InvSymbolNameMap.find(topSymbolId)->second << std::endl;
            const Tools::Symbol &topSymbol = Tools::Symbols.find(topSymbolId)->second;

            int oriSymbolId = Tools::SymbolNameMap.find(curToken.type)->second;
            const Tools::Symbol &oriSymbol = Tools::Symbols.find(oriSymbolId)->second;

            if (topSymbol.type == Tools::TERMI)
            {
                std::cout << "SB1" << std::endl;
                symbolStack.pop();
                curToken = lexer.GetToken();
            }
            else
            {
                std::map<int, Tools::LL1Item> &items = ll1Table.find(topSymbolId)->second;
                auto itemIt = items.find(oriSymbolId);
                if (itemIt == items.end())
                {
                    //TODO Error Handling
                    std::cout << "ERROR" << std::endl;
                    return;
                }
                symbolStack.pop();

                Tools::LL1Item &item = itemIt->second;
                if (item.size() > 1)
                {
                    std::cout << "AMBIGU" << std::endl;
                    return;
                    //TODO ambiguity
                }
                else
                {
                    Tools::Expression expression = topSymbol.subExpressions.find(item[0])->second[0];
                    for (int i = expression.size() - 1; i >= 0; --i)
                        if (expression[i] != Tools::EPS)
                            symbolStack.push(expression[i]);
                    if (item[0] != Tools::EPS)
                        symbolStack.push(item[0]);
                    std::cout << Tools::InvSymbolNameMap.find(topSymbolId)->second << "->" << Tools::InvSymbolNameMap.find(item[0])->second;
                    for (int sb : expression)
                        std::cout << " " << Tools::InvSymbolNameMap.find(sb)->second;
                    std::cout << std::endl;
                }
            }
        }
    }
} // namespace CompilerFront