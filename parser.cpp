#include "parser.h"

namespace CompilerFront
{
    std::unique_ptr<PascalAST::ASTNode> Parser::Parse(bool &ok)
    {
        ok = true;
        std::map<int, std::map<int, Tools::LL1Item>> &ll1Table = Tools::LL1Table;
        curToken = lexer.GetToken();
        nxtToken = lexer.GetToken();
        int ifCnt = 0;
        while (!symbolStack.empty())
        {
            // std::cout << "--------CURTOKEN " << curToken.type << " " << curToken.content << " --------" << std::endl;
            int topSymbolId = symbolStack.top();
            // std::cout << "STK TOP " << symbolStack.size() << " " << Tools::InvSymbolNameMap.find(topSymbolId)->second << std::endl;
            const Tools::Symbol &topSymbol = Tools::Symbols.find(topSymbolId)->second;

            int oriSymbolId = Tools::SymbolNameMap.find(curToken.type)->second;
            const Tools::Symbol &oriSymbol = Tools::Symbols.find(oriSymbolId)->second;

            if (topSymbol.type == Tools::TERMI)
            {
                if (topSymbolId == oriSymbolId)
                {
                    // std::cout << "SB1" << std::endl;
                    if (curToken.type == "if")
                        ifCnt++;
                    symbolStack.pop();

                    if (ok)
                        astStack.push_back(PascalAST::GenOriAstNode(curToken));

                    curToken = nxtToken;
                    nxtToken = lexer.GetToken();
                }
                else
                {
                    ok = false;
                    logErrMsg(curToken.stLine, curToken.stColumn, topSymbolId, oriSymbolId, true);
                    symbolStack.pop();
                    continue;
                    // return std::unique_ptr<PascalAST::ASTNode>();
                }
            }
            else
            {
                std::map<int, Tools::LL1Item> &items = ll1Table.find(topSymbolId)->second;
                auto itemIt = items.find(oriSymbolId);
                if (itemIt == items.end())
                {
                    ok = false;
                    logErrMsg(curToken.stLine, curToken.stColumn, topSymbolId, oriSymbolId, false);
                    auto &syncSet = Tools::SyncTable.find(topSymbolId)->second;
                    if (syncSet.find(oriSymbolId) != syncSet.end())
                    {
                        symbolStack.pop();
                    }
                    else
                    {
                        while (items.find(oriSymbolId) == items.end() || syncSet.find(oriSymbolId) == syncSet.end())
                        {
                            curToken = nxtToken;
                            nxtToken = lexer.GetToken();
                            oriSymbolId = Tools::SymbolNameMap.find(curToken.type)->second;
                            if (oriSymbolId == Tools::LINE_END)
                                break;
                        }
                    }
                    continue;
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
                                expression = topSymbol.subExpressions.find(sbitem)->second[0];
                                expression.insert(expression.begin(), sbitem);
                                break;
                            }
                        }
                    }
                    else if (Tools::InvSymbolNameMap.find(topSymbolId)->second == "else_part")
                    {
                        // std::cout << "AMB3";
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
                                expression = topSymbol.subExpressions.find(sbitem)->second[0];
                                expression.insert(expression.begin(), sbitem);
                                ifCnt--; //TODO DEBUG
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
                if (ok)
                {
                    if (allEps)
                    {
                        std::vector<std::unique_ptr<PascalAST::ASTNode>> empty;
                        astStack.push_back(PascalAST::GenAstNode(
                            Tools::InvSymbolNameMap.find(topSymbolId)->second,
                            Tools::InvSymbolNameMap.find(expression[0])->second, empty));
                    }
                    else
                    {
                        reduceStack.push(ReduceInfo(
                            reduceStackSize,
                            reduceCnt,
                            Tools::InvSymbolNameMap.find(topSymbolId)->second,
                            Tools::InvSymbolNameMap.find(expression[0])->second));
                        // std::cout << "--------PREPARE--------"
                        //           << " " << reduceStackSize << " " << reduceCnt << std::endl;
                    }
                }
                // std::cout << Tools::InvSymbolNameMap.find(topSymbolId)->second << "->";
                // for (int sb : expression)
                //     std::cout << " " << Tools::InvSymbolNameMap.find(sb)->second;
                // std::cout << std::endl;
            }
            // std::cout << "<<<<<<<<<<<<<<<<<<<<<<<<<<STK SZ " << symbolStack.size() << " >>>>>>>>>>>>>>>>>>" << std::endl;
            for (ReduceInfo info = reduceStack.top(); ok && (symbolStack.size() == info.reduceStackSize); info = reduceStack.top())
            {
                reduceStack.pop();
                std::vector<std::unique_ptr<PascalAST::ASTNode>> subNodes;
                // std::cout << "POP!!!!!!! " << info.expressionLeft << " " << info.expressionFirst << std::endl;
                for (int i = 0; i < info.reduceCnt; i++)
                {
                    subNodes.insert(subNodes.begin(), std::move(astStack[astStack.size() - 1]));
                    astStack.pop_back();
                }
                astStack.push_back(PascalAST::GenAstNode(info.expressionLeft, info.expressionFirst, subNodes));
                if (reduceStack.empty())
                    break;
            }
        }
        return ok ? std::move(astStack[astStack.size() - 1]) : std::unique_ptr<PascalAST::ASTNode>();
    }
} // namespace CompilerFront