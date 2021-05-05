#include "ast.h"

namespace PascalAST
{
    void AbstractSyntaxTree::Check()
    {
        while (!symTable.empty())
            symTable.pop();
        astRoot->Check(symTable);
    }

    std::unique_ptr<TypeInfo> OriASTNode::Check(SymbolTable &table)
    {
        return GenType(VOID);
    }

    std::unique_ptr<TypeInfo> Identifiers::Check(SymbolTable &table)
    {
        return GenType(VOID);
    }

    std::unique_ptr<TypeInfo> Range::Check(SymbolTable &table)
    {
        //TODO if(l>=r)
        return GenType(VOID);
    }

    std::unique_ptr<TypeInfo> Ranges::Check(SymbolTable &table)
    {
        for (int i = 0; i < ranges.size(); i++)
            ranges[i]->Check(table);
        return GenType(VOID);
    }

    std::unique_ptr<TypeInfo> Type::Check(SymbolTable &table)
    {
        return GenType(VOID);
    }

    std::unique_ptr<TypeInfo> BasicType::Check(SymbolTable &table)
    {
        if (basicType == "int")
        {
            return GenType(INTEGER);
        }
        if (basicType == "real")
        {
            return GenType(REAL);
        }
        if (basicType == "char")
        {
            return GenType(CHAR);
        }
        if (basicType == "boolean")
        {
            return GenType(BOOLEAN);
        }
    }

    std::unique_ptr<TypeInfo> ArrayType::Check(SymbolTable &table)
    {
        ranges->Check(table);
        TypeInfo *arrType = new ArrayType(ranges->ranges.size(), type->Check(table)->GetTypeId());
        return std::unique_ptr<arrType>;
    }
    std::unique_ptr<TypeInfo> ConstantDeclaration::Check(SymbolTable &table)
    {
        //TODO content
        table.top().insert(std::pair<std::string, std::unique_ptr<TypeInfo>>(name, type->Check(table)));
        return GenType(VOID);
    }
    std::unique_ptr<TypeInfo> ConstantDeclarations::Check(SymbolTable &table)
    {
        for (int i = 0; i < constantDeclarations.size(); i++)
            constantDeclarations[i]->Check(table);
    }
    std::unique_ptr<TypeInfo> VariableDeclaration::Check(SymbolTable &table)
    {
        TypeID tp = type->Check(table)->GetTypeId();
        identifiers->Check(table);
        SymbolTableItem &item = table.top();
        for (std::string id : identifiers->identifiers)
        {
            if (item.find(id) != item.end())
            {
                //TODO
            }
            item.insert(std::pair<std::string, std::unique_ptr<TypeInfo>>(id, GenType(tp)));
        }
    }
}