#include "ast.h"

namespace PascalAST
{
    void AbstractSyntaxTree::Check()
    {
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

    std::unique_ptr<TypeInfo> TypeDecl::Check(SymbolTable &table)
    {
        return GenType(VOID);
    }

    std::unique_ptr<TypeInfo> BasicTypeDecl::Check(SymbolTable &table)
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

    std::unique_ptr<TypeInfo> ArrayTypeDecl::Check(SymbolTable &table)
    {
        ranges->Check(table);
        TypeInfo *arrType = new ArrayType(int(ranges->ranges.size()), type->Check(table)->GetTypeId());
        return std::unique_ptr<TypeInfo>(arrType);
    }
    std::unique_ptr<TypeInfo> ConstantDeclaration::Check(SymbolTable &table)
    {
        table.InsertSymbol(name, type->Check(table), true, content);
        return GenType(VOID);
    }
    std::unique_ptr<TypeInfo> ConstantDeclarations::Check(SymbolTable &table)
    {
        for (int i = 0; i < constantDeclarations.size(); i++)
            constantDeclarations[i]->Check(table);
        return GenType(VOID);
    }
    std::unique_ptr<TypeInfo> VariableDeclaration::Check(SymbolTable &table)
    {
        auto tp(type->Check(table));
        identifiers->Check(table);
        int layerCnt = table.GetLayerCnt();
        int layer;
        for (std::string id : identifiers->identifiers)
        {
            if (table.HasSymbol(id, layer) && layer + 1 == layerCnt)
            {
                //TODO
            }
            else
            {
                table.InsertSymbol(id, tp->Copy(), false, "");
            }
            //item.insert(std::pair<std::string, std::unique_ptr<TypeInfo>>(id, GenType(tp)));
        }
        return GenType(VOID);
    }

    std::unique_ptr<TypeInfo> VariableDeclarations::Check(SymbolTable &table)
    {
        for (int i = 0; i < variableDeclarations.size(); i++)
            variableDeclarations[i]->Check(table);
        return GenType(VOID);
    }

    std::unique_ptr<TypeInfo> Parameter::Check(SymbolTable &table)
    {
        type->Check(table);
        identifiers->Check(table);
        return GenType(VOID);
    }

    std::unique_ptr<TypeInfo> ParameterList::Check(SymbolTable &table)
    {
        for (int i = 0; i < parameters.size(); i++)
            parameters[i]->Check(table);
        return GenType(VOID);
    }

    std::unique_ptr<TypeInfo> Variable::Check(SymbolTable &table)
    {
    }
}