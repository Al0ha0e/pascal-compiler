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
        for (auto &range : ranges)
            range->Check(table);
        return GenType(VOID);
    }

    std::unique_ptr<TypeInfo> TypeDecl::Check(SymbolTable &table)
    {
        return GenType(VOID);
    }

    std::unique_ptr<TypeInfo> BasicTypeDecl::Check(SymbolTable &table)
    {
        return GenTypeByStr(basicType);
    }

    std::unique_ptr<TypeInfo> ArrayTypeDecl::Check(SymbolTable &table)
    {
        ranges->Check(table);
        std::vector<std::pair<int, int>> arrRanges;
        for (auto &range : ranges->ranges)
            arrRanges.push_back(std::pair<int, int>(range->l, range->r));

        TypeInfo *arrType = new ArrayType(arrRanges, type->Check(table)->Copy());
        return std::unique_ptr<TypeInfo>(arrType);
    }

    std::unique_ptr<TypeInfo> ConstantDeclaration::Check(SymbolTable &table)
    {
        if (table.SymbolAtTop(name))
        { //TODO
        }
        else
        {
            TypeInfo *lValueType = new LValueType(type->Check(table));
            table.InsertSymbol(name, std::unique_ptr<TypeInfo>(lValueType), true, content);
        }
        return GenType(VOID);
    }

    std::unique_ptr<TypeInfo> ConstantDeclarations::Check(SymbolTable &table)
    {
        for (auto &declaration : constantDeclarations)
            declaration->Check(table);
        return GenType(VOID);
    }

    std::unique_ptr<TypeInfo> VariableDeclaration::Check(SymbolTable &table)
    {
        auto tp(type->Check(table));
        identifiers->Check(table);
        int layer;
        for (std::string id : identifiers->identifiers)
        {
            if (table.SymbolAtTop(id))
            {
                //TODO
            }
            else
            {
                TypeInfo *lValueType = new LValueType(tp->Copy());
                table.InsertSymbol(id, std::unique_ptr<TypeInfo>(lValueType), false, "");
            }
        }
        return GenType(VOID);
    }

    std::unique_ptr<TypeInfo> VariableDeclarations::Check(SymbolTable &table)
    {
        for (auto &declaration : variableDeclarations)
            declaration->Check(table);
        return GenType(VOID);
    }

    std::unique_ptr<TypeInfo> Parameter::Check(SymbolTable &table)
    {
        identifiers->Check(table);
        TypeInfo *ret;
        if (isRef)
        {
            ret = new RefType(type->Check(table));
        }
        else
        {
            ret = new LValueType(type->Check(table));
        }
        return std::unique_ptr<TypeInfo>(ret);
    }

    std::unique_ptr<TypeInfo> ParameterList::Check(SymbolTable &table)
    {
        std::vector<std::unique_ptr<TypeInfo>> types;
        for (auto &parameter : parameters)
        {
            auto pType(parameter->Check(table));
            for (auto id : parameter->identifiers->identifiers)
            {
                if (table.SymbolAtTop(id))
                {
                    //TODO
                }
                else
                {
                    table.InsertSymbol(id, pType->Copy(), false, "");
                }
                types.push_back(pType->Copy());
            }
        }
        TypeInfo *tupleType = new TupleType(std::move(types));
        return std::unique_ptr<TypeInfo>(tupleType);
    }

    std::unique_ptr<TypeInfo> Variable::Check(SymbolTable &table)
    {
        bool has;
        int layer;
        auto symbolIt = table.FindSymbol(name, has, layer);

        if (!has)
        {
            //TODO
            TypeInfo *ret = new RValueType(GenType(VOID));
            return std::unique_ptr<TypeInfo>(ret);
        }
        std::unique_ptr<TypeInfo> &type = symbolIt->second.type;
        auto targetType(((WrapperType *)type.get())->DeWrap());
        if (targetType->GetTypeId() == FUNC)
        {
            if (varPart != nullptr)
            {
                if (varPart->isProcedureCall)
                    return type->CalcFuncType(UniquePtrCast<TupleType>(varPart->Check(table)));
                return type->CalcArrayType(UniquePtrCast<TupleType>(varPart->Check(table)));
                //TODO: Check Array Range
            }
            TupleType *emptyTuple = new TupleType();
            return type->CalcFuncType(std::unique_ptr<TupleType>(emptyTuple));
        }
        if (varPart != nullptr)
        {
            if (varPart->isProcedureCall)
                return type->CalcFuncType(UniquePtrCast<TupleType>(varPart->Check(table)));
            return type->CalcArrayType(UniquePtrCast<TupleType>(varPart->Check(table)));
        }
        return type->Copy();
    }

    std::unique_ptr<TypeInfo> VariableList::Check(SymbolTable &table)
    {
        std::vector<std::unique_ptr<TypeInfo>> types;
        for (auto &variable : variables)
        {
            //TODO: Check LVALUE
            types.push_back(variable->Check(table));
        }
        TypeInfo *tupleType = new TupleType(std::move(types));
        return std::unique_ptr<TypeInfo>(tupleType);
    }

    std::unique_ptr<TypeInfo> Factor::Check(SymbolTable &table)
    {
        TypeInfo *ret = new RValueType(GenType(VOID));
        return std::unique_ptr<TypeInfo>(ret);
    }

    std::unique_ptr<TypeInfo> ExpressionFactor::Check(SymbolTable &table)
    {
        return expression->Check(table);
    }

    std::unique_ptr<TypeInfo> NumFactor::Check(SymbolTable &table)
    {
        TypeInfo *ret = new RValueType(GenTypeByStr(type));
        return std::unique_ptr<TypeInfo>(ret);
    }

    std::unique_ptr<TypeInfo> InvFactor::Check(SymbolTable &table)
    {
        //TODO type check
        TypeInfo *ret = new RValueType(subFactor->Check(table));
        return std::unique_ptr<TypeInfo>(ret);
    }

    std::unique_ptr<TypeInfo> VariableFactor::Check(SymbolTable &table)
    {
        return variable->Check(table);
    }

    std::unique_ptr<TypeInfo> NotFactor::Check(SymbolTable &table)
    {
        //TODO type check
        TypeInfo *ret = new RValueType(subFactor->Check(table));
        return std::unique_ptr<TypeInfo>(ret);
    }

    std::unique_ptr<TypeInfo> MulOpPart::Check(SymbolTable &table)
    {
        if (followPart == nullptr)
            return secondFactor->Check(table);
        return followPart->Check(table)->CalcType(secondFactor->Check(table));
    }

    std::unique_ptr<TypeInfo> Term::Check(SymbolTable &table)
    {
        if (mulOpPart == nullptr)
            return firstFactor->Check(table);
        return mulOpPart->Check(table)->CalcType(firstFactor->Check(table));
    }

    std::unique_ptr<TypeInfo> AddOpPart::Check(SymbolTable &table)
    {
        if (followPart == nullptr)
            return secondTerm->Check(table);
        return followPart->Check(table)->CalcType(secondTerm->Check(table));
    }

    std::unique_ptr<TypeInfo> SimpleExpression::Check(SymbolTable &table)
    {
        if (addOpPart == nullptr)
            return firstTerm->Check(table);
        return addOpPart->Check(table)->CalcType(firstTerm->Check(table));
    }

    std::unique_ptr<TypeInfo> RelPart::Check(SymbolTable &table)
    {
        return secondExpression->Check(table);
    }

    std::unique_ptr<TypeInfo> Expression::Check(SymbolTable &table)
    {
        if (relPart == nullptr)
            return firstExpression->Check(table);
        firstExpression->Check(table);
        relPart->Check(table);
        //TODO
        TypeInfo *ret = new RValueType(GenType(BOOLEAN));
        return std::unique_ptr<TypeInfo>(ret);
    }

    std::unique_ptr<TypeInfo> ExpressionList::Check(SymbolTable &table)
    {
        std::vector<std::unique_ptr<TypeInfo>> types;
        for (auto &expression : expressions)
        {
            types.push_back(expression->Check(table));
        }
        TypeInfo *tupleType = new TupleType(std::move(types));
        return std::unique_ptr<TypeInfo>(tupleType);
    }

    std::unique_ptr<TypeInfo> VarPart::Check(SymbolTable &table)
    {
        return expressionList->Check(table);
    }

    std::unique_ptr<TypeInfo> Statement::Check(SymbolTable &table)
    {
        return GenType(VOID);
    }

    std::unique_ptr<TypeInfo> VariableAssignStatement::Check(SymbolTable &table)
    {
        variable->Check(table);
        expression->Check(table);
        if (!variable->Check(table)->Compatible(expression->Check(table)))
        {
            //TODO
        }
        return GenType(VOID);
    }

    std::unique_ptr<TypeInfo> ProcedureCallStatement::Check(SymbolTable &table)
    {
        //TODO:CHECK CALLABLE
        variable->Check(table);
        return GenType(VOID);
    }

    std::unique_ptr<TypeInfo> SubCompoundStatement::Check(SymbolTable &table)
    {
        compoundStatement->Check(table);
        return GenType(VOID);
    }
    std::unique_ptr<TypeInfo> IfElseStatement::Check(SymbolTable &table)
    {
        ifExpression->Check(table);
        if (thenStatement != nullptr)
            thenStatement->Check(table);
        if (elseStatement != nullptr)
            elseStatement->Check(table);
        return GenType(VOID);
    }
    std::unique_ptr<TypeInfo> ForLoopStatement::Check(SymbolTable &table)
    {
        //TODO:Check counter
        //TODO duplicate?
        initExpression->Check(table);
        termiExpression->Check(table);
        if (loopStatement != nullptr)
            loopStatement->Check(table);
        return GenType(VOID);
    }
    std::unique_ptr<TypeInfo> ReadStatement::Check(SymbolTable &table)
    {
        variableList->Check(table);
        return GenType(VOID);
    }
    std::unique_ptr<TypeInfo> WriteStatement::Check(SymbolTable &table)
    {
        expressionList->Check(table);
        return GenType(VOID);
    }
    std::unique_ptr<TypeInfo> StatementList::Check(SymbolTable &table)
    {
        for (auto &statement : statements)
        {
            if (statement != nullptr)
                statement->Check(table);
        }
        return GenType(VOID);
    }
    std::unique_ptr<TypeInfo> CompoundStatement::Check(SymbolTable &table)
    {
        statementList->Check(table);
        return GenType(VOID);
    }

    std::unique_ptr<TypeInfo> SubProgramHead::Check(SymbolTable &table)
    {
        table.InsertSymbol(name, std::unique_ptr<TypeInfo>(), true, "");
        auto retType(GenType(VOID));
        if (returnType != nullptr)
            retType = returnType->Check(table);

        std::unique_ptr<TypeInfo> funcType(
            new FuncType(
                UniquePtrCast<TupleType>(parameters->Check(table)),
                std::move(retType)));

        TypeInfo *lValueType = new LValueType(std::move(funcType));
        std::unique_ptr<TypeInfo> ret(lValueType);
        table.InsertSymbol(name, ret->Copy(), true, "");

        return ret;
    }

    std::unique_ptr<TypeInfo> SubProgramBody::Check(SymbolTable &table)
    {
        constantDeclarations->Check(table);
        variableDeclarations->Check(table);
        compoundStatement->Check(table);
        return GenType(VOID);
    }
    std::unique_ptr<TypeInfo> SubProgram::Check(SymbolTable &table)
    {
        table.PushMap();
        table.Step();
        auto funcType(head->Check(table));
        body->Check(table);
        table.PopMap();
        table.InsertSymbol(head->name, std::move(funcType), true, "");

        return GenType(VOID);
    }
    std::unique_ptr<TypeInfo> SubProgramDeclarations::Check(SymbolTable &table)
    {
        for (auto &subProgram : subPrograms)
            subProgram->Check(table);
        return GenType(VOID);
    }

    std::unique_ptr<TypeInfo> ProgramHead::Check(SymbolTable &table)
    {
        table.InsertSymbol(name, std::unique_ptr<TypeInfo>(), true, "");
        identifiers->Check(table);
        for (auto &id : identifiers->identifiers)
            table.InsertSymbol(id, std::unique_ptr<TypeInfo>(), true, "");

        return GenType(VOID);
    }

    std::unique_ptr<TypeInfo> ProgramBody::Check(SymbolTable &table)
    {
        constantDeclarations->Check(table);
        variableDeclarations->Check(table);
        subProgramDeclarations->Check(table);
        compoundStatemnet->Check(table);
        return GenType(VOID);
    }
    std::unique_ptr<TypeInfo> Program::Check(SymbolTable &table)
    {
        table.PushMap();
        table.Step();
        programHead->Check(table);
        programBody->Check(table);
        table.PopMap();
        return GenType(VOID);
    }
}