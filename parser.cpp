#include "parser.h"

namespace CompilerFront
{
    void Parser::Parse()
    {
        std::map<int, std::map<int, Tools::LL1Item>> &ll1Table = Tools::LL1Table;
        for (Token oriToken = lexer.GetToken(); oriToken.type != "$"; oriToken = lexer.GetToken())
        {
            std::cout << oriToken.type << " " << oriToken.content << std::endl;
            int topSymbol = symbolStack.top();
            int symbolId = Tools::SymbolNameMap.find(oriToken.type)->second;
            const Tools::Symbol &symbol = Tools::Symbols.find(symbolId)->second;

            if (symbol.type == Tools::TERMI)
            {
                std::cout << "SB1" << std::endl;
                symbolStack.pop();
            }
            else
            {
                std::map<int, Tools::LL1Item> &items = ll1Table.find(topSymbol)->second;
                auto itemIt = items.find(symbolId);
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
                    Tools::Expression expression = symbol.subExpressions.find(item[0])->second[0];
                    for (int i = expression.size() - 1; i >= 0; --i)
                        symbolStack.push(expression[i]);
                    symbolStack.push(item[0]);
                    std::cout << Tools::InvSymbolNameMap.find(symbolId)->second << "->" << Tools::InvSymbolNameMap.find(item[0])->second;
                    for (int sb : expression)
                        std::cout << " " << Tools::InvSymbolNameMap.find(sb)->second;
                    std::cout << std::endl;
                }
            }
        }
    }
} // namespace CompilerFront