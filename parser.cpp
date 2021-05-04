#include "parser.h"

namespace CompilerFront
{
    void Parser::Parse()
    {
        std::map<int, std::map<int, Tools::LL1Item>> &ll1Table = Tools::LL1Table;
        curToken = lexer.GetToken();
        nxtToken = lexer.GetToken();
        int ifCnt = 0;
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
                if (topSymbol.id == oriSymbolId)
                {
                    std::cout << "SB1" << std::endl;
                    if (curToken.type == "if")
                        ifCnt++;
                    symbolStack.pop();

                    astStack.push(PascalAST::GenOriAstNode(curToken));

                    ReduceInfo info = reduceStack.top();
                    if (symbolStack.size() == info.reduceStackSize)
                    {
                        reduceStack.pop();
                        std::vector<std::unique_ptr<PascalAST::ASTNode>> subNodes;
                        for (int i = 0; i < info.reduceCnt; i++)
                        {
                            subNodes.insert(subNodes.begin(), std::move(astStack.top()));
                            astStack.pop();
                        }
                        astStack.push(PascalAST::GenAstNode(info.expressionLeft, info.expressionLeft, subNodes));
                    }

                    curToken = nxtToken;
                    nxtToken = lexer.GetToken();
                }
                else
                {
                    //TODO Error Handling
                    std::cout << "ERROR" << std::endl;
                    return;
                }
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
                Tools::Expression expression;
                if (item.size() > 1)
                {
                    if (Tools::InvSymbolNameMap.find(topSymbolId)->second == "const_declaration_80")
                    {
                        for (int sbitem : item)
                        {
                            if (nxtToken.type == "id" && sbitem != Tools::EPS)
                            {
                                expression = topSymbol.subExpressions.find(sbitem)->second[0];
                                expression.insert(expression.begin(), sbitem);
                                break;
                            }
                            else if (nxtToken.type != "id" && sbitem == Tools::EPS)
                            {
                                //TODO ERROR HANDLINg
                                expression = topSymbol.subExpressions.find(sbitem)->second[0];
                                expression.insert(expression.begin(), sbitem);
                                break;
                            }
                        }
                    }
                    else if (Tools::InvSymbolNameMap.find(topSymbolId)->second == "var_declaration_81")
                    {
                        for (int sbitem : item)
                        {
                            if (nxtToken.type == "id" && sbitem != Tools::EPS)
                            {
                                expression = topSymbol.subExpressions.find(sbitem)->second[0];
                                expression.insert(expression.begin(), sbitem);
                                break;
                            }
                            else if (nxtToken.type != "id" && sbitem == Tools::EPS)
                            {
                                //TODO ERROR HANDLINg
                                expression = topSymbol.subExpressions.find(sbitem)->second[0];
                                expression.insert(expression.begin(), sbitem);
                                break;
                            }
                        }
                    }
                    else if (Tools::InvSymbolNameMap.find(topSymbolId)->second == "else_part")
                    {
                        std::cout << "AMB3";
                        for (int sbitem : item)
                        {
                            if (ifCnt == 0 && sbitem == Tools::EPS)
                            {
                                expression = topSymbol.subExpressions.find(sbitem)->second[0];
                                expression.insert(expression.begin(), sbitem);
                                break;
                            }
                            else if (ifCnt > 0 && sbitem != Tools::EPS)
                            {
                                //TODO ERROR HANDLINg
                                expression = topSymbol.subExpressions.find(sbitem)->second[0];
                                expression.insert(expression.begin(), sbitem);
                                ifCnt--;
                                break;
                            }
                        }
                    }
                }
                else
                {
                    if (Tools::InvSymbolNameMap.find(topSymbolId)->second == "else_part")
                        ifCnt--;
                    expression = topSymbol.subExpressions.find(item[0])->second[0];
                    expression.insert(expression.begin(), item[0]);
                }
                bool allEps = true;
                int reduceStackSize = symbolStack.size();
                int reduceCnt = 0;
                for (int i = expression.size() - 1; i >= 0; --i)
                {
                    if (expression[i] != Tools::EPS)
                    {
                        symbolStack.push(expression[i]);
                        reduceCnt++;
                        allEps = false;
                    }
                }
                if (allEps)
                {
                    astStack.push(PascalAST::GenAstNode(
                        Tools::InvSymbolNameMap.find(topSymbolId)->second,
                        Tools::InvSymbolNameMap.find(expression[0])->second,
                        std::vector<std::unique_ptr<PascalAST::ASTNode>>()));
                }
                else
                {
                    reduceStack.push(ReduceInfo(
                        reduceStackSize,
                        reduceCnt,
                        Tools::InvSymbolNameMap.find(topSymbolId)->second,
                        Tools::InvSymbolNameMap.find(expression[0])->second));
                }
                std::cout << Tools::InvSymbolNameMap.find(topSymbolId)->second << "->";
                for (int sb : expression)
                    std::cout << " " << Tools::InvSymbolNameMap.find(sb)->second;
                std::cout << std::endl;
            }
        }
    }
} // namespace CompilerFront