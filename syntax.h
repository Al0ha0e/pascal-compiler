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

    class SymbolTable
    {
    public:
        SymbolTable()
        {
            table.push_back(SymbolMap());
        }

        void PushMap()
        {
            table.push_back(SymbolMap());
        }

        void PopMap()
        {
            table.pop_back();
        }

        int GetLayerCnt()
        {
            return table.size();
        }

        SymbolMap::iterator FindSymbol(std::string id, bool &has, int &layer)
        {
            SymbolMap::iterator ret;
            has = false;
            for (layer = table.size() - 1; layer >= 0; --layer)
            {
                ret = table[layer].find(id);
                if (ret != table[layer].end())
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
            for (layer = table.size() - 1; layer >= 0; --layer)
            {
                if (table[layer].find(id) != table[layer].end())
                {
                    ret = true;
                    break;
                }
            }
            return ret;
        }

        bool SymbolAtTop(std::string id)
        {
            auto &top = table[table.size() - 1];
            if (top.find(id) != top.end())
                return false;
            return true;
        }

        void InsertSymbol(std::string id, std::unique_ptr<TypeInfo> &&type, bool isConstant, std::string oriVal)
        {
            table[table.size() - 1][id] = SymbolTableItem(std::move(type), isConstant, oriVal);
        }

    private:
        std::vector<SymbolMap> table;
    };
}

#endif