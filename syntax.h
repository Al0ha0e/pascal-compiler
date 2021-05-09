#ifndef SYNTAX
#define SYNTAX

#include <map>
#include <vector>
#include <memory>
#include "ast.h"

namespace PascalAST
{

    struct SymbolTableItem
    {
        std::unique_ptr<TypeInfo> type;
        bool isConstant;
        std::string oriVal;

        SymbolTableItem() {}
        SymbolTableItem(std::unique_ptr<TypeInfo> &&type, bool isConstant, std::string oriVal) : type(std::move(type)), isConstant(isConstant), oriVal(oriVal) {}
    };

    typedef std::map<std::string, SymbolTableItem> SymbolMap;

    struct SymbolScope
    {
        SymbolMap symbolMap;
        std::shared_ptr<SymbolScope> upperScope;
        std::vector<std::shared_ptr<SymbolScope>> subScope;
        int layer;
        int curSubCnt;

        SymbolScope() {}
        SymbolScope(std::shared_ptr<SymbolScope> &upperScope, int layer) : upperScope(upperScope), layer(layer) {}
    };

    class SymbolTable
    {
    public:
        SymbolTable()
        {
            rootScope = curScope = std::make_shared<SymbolScope>();
        }

        void PushMap()
        {
            curScope->subScope.push_back(std::make_shared<SymbolScope>(curScope, curScope->layer + 1));
        }

        void Step()
        {
            if (curScope->curSubCnt + 1 < curScope->subScope.size())
                curScope = curScope->subScope[curScope->curSubCnt++];
        }

        void PopMap()
        {
            if (curScope->upperScope != nullptr)
            {
                curScope->curSubCnt = 0;
                curScope = curScope->upperScope;
            }
        }

        SymbolMap::iterator FindSymbol(std::string id, bool &has, int &layer)
        {
            SymbolMap::iterator ret;
            has = false;
            for (auto scope = curScope; scope != nullptr; scope = scope->upperScope)
            {
                ret = scope->symbolMap.find(id);
                layer = scope->layer;
                if (ret != scope->symbolMap.end())
                {
                    has = true;
                    break;
                }
            }
            return ret;
        }

        bool HasSymbol(std::string id, int &layer)
        {
            bool ret = false;
            for (auto scope = curScope; scope != nullptr; scope = scope->upperScope)
            {
                layer = scope->layer;
                if (scope->symbolMap.find(id) != scope->symbolMap.end())
                {
                    ret = true;
                    break;
                }
            }
            return ret;
        }

        bool SymbolAtTop(std::string id)
        {
            auto &top = curScope->symbolMap;
            if (top.find(id) != top.end())
                return false;
            return true;
        }

        void InsertSymbol(std::string id, std::unique_ptr<TypeInfo> &&type, bool isConstant, std::string oriVal)
        {
            curScope->symbolMap[id] = SymbolTableItem(std::move(type), isConstant, oriVal);
        }

    private:
        std::shared_ptr<SymbolScope> rootScope;
        std::shared_ptr<SymbolScope> curScope;
    };
}

#endif