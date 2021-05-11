#include "ast.h"

namespace PascalAST
{
    void AbstractSyntaxTree::Check()
    {
        bool ok;
        astRoot->Check(symTable, ok);
    }

    std::unique_ptr<TypeInfo> OriASTNode::Check(SymbolTable &table, bool &ok)
    {
        return GenType(VOID);
    }

    std::unique_ptr<TypeInfo> Identifiers::Check(SymbolTable &table, bool &ok)
    {
        return GenType(VOID);
    }

    std::unique_ptr<TypeInfo> Range::Check(SymbolTable &table, bool &ok)
    {
        if (l > r)
        {
            ok = false;
            //TODO
        }
        return GenType(VOID);
    }

    std::unique_ptr<TypeInfo> Ranges::Check(SymbolTable &table, bool &ok)
    {
        for (auto &range : ranges)
            range->Check(table, ok);
        return GenType(VOID);
    }

    std::unique_ptr<TypeInfo> TypeDecl::Check(SymbolTable &table, bool &ok)
    {
        return GenType(VOID);
    }

    std::unique_ptr<TypeInfo> BasicTypeDecl::Check(SymbolTable &table, bool &ok)
    {
        return GenTypeByStr(basicType);
    }

    std::unique_ptr<TypeInfo> ArrayTypeDecl::Check(SymbolTable &table, bool &ok)
    {
        ranges->Check(table, ok);
        std::vector<std::pair<int, int>> arrRanges;
        for (auto &range : ranges->ranges)
            arrRanges.push_back(std::pair<int, int>(range->l, range->r));

        TypeInfo *arrType = new ArrayType(arrRanges, type->Check(table, ok)->Copy());
        return std::unique_ptr<TypeInfo>(arrType);
    }

    std::unique_ptr<TypeInfo> ConstantDeclaration::Check(SymbolTable &table, bool &ok)
    {
        if (table.SymbolAtTop(name))
        { //TODO
            ok = false;
        }
        else
        {
            TypeInfo *lValueType = new LValueType(type->Check(table, ok));
            table.InsertSymbol(name, std::unique_ptr<TypeInfo>(lValueType), true, content);
        }
        return GenType(VOID);
    }

    std::unique_ptr<TypeInfo> ConstantDeclarations::Check(SymbolTable &table, bool &ok)
    {
        for (auto &declaration : constantDeclarations)
            declaration->Check(table, ok);
        return GenType(VOID);
    }

    std::unique_ptr<TypeInfo> VariableDeclaration::Check(SymbolTable &table, bool &ok)
    {
        auto tp(type->Check(table, ok));
        identifiers->Check(table, ok);
        int layer;
        for (std::string id : identifiers->identifiers)
        {
            if (table.SymbolAtTop(id))
            {
                //TODO
                ok = false;
            }
            else
            {
                TypeInfo *lValueType = new LValueType(tp->Copy());
                table.InsertSymbol(id, std::unique_ptr<TypeInfo>(lValueType), false, "");
            }
        }
        return GenType(VOID);
    }

    std::unique_ptr<TypeInfo> VariableDeclarations::Check(SymbolTable &table, bool &ok)
    {
        for (auto &declaration : variableDeclarations)
            declaration->Check(table, ok);
        return GenType(VOID);
    }

    std::unique_ptr<TypeInfo> Parameter::Check(SymbolTable &table, bool &ok)
    {
        identifiers->Check(table, ok);
        TypeInfo *ret;
        if (isRef)
        {
            ret = new RefType(type->Check(table, ok));
        }
        else
        {
            ret = new LValueType(type->Check(table, ok));
        }
        return std::unique_ptr<TypeInfo>(ret);
    }

    std::unique_ptr<TypeInfo> ParameterList::Check(SymbolTable &table, bool &ok)
    {
        std::vector<std::unique_ptr<TypeInfo>> types;
        for (auto &parameter : parameters)
        {
            auto pType(parameter->Check(table, ok));
            for (auto id : parameter->identifiers->identifiers)
            {
                if (table.SymbolAtTop(id))
                {
                    //TODO
                    ok = false;
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

    std::unique_ptr<TypeInfo> Variable::Check(SymbolTable &table, bool &ok)
    {
        bool has;
        int layer;
        auto symbolIt = table.FindSymbol(name, has, layer);

        if (!has)
        {
            //TODO
            ok = false;
            TypeInfo *ret = new RValueType(GenType(VOID));
            return std::unique_ptr<TypeInfo>(ret);
        }
        std::unique_ptr<TypeInfo> &type = symbolIt->second.type;
        auto targetType(((WrapperType *)type.get())->DeWrap());
        if (targetType->GetTypeId() == FUNC)
        {
            if (varPart != nullptr)
            {
                auto argTypes(UniquePtrCast<FuncType>(targetType)->GetArgTypes());
                for (auto &argType : argTypes)
                    varPart->argIsRef.push_back(argType->GetTypeId() == REF);

                if (varPart->isProcedureCall)
                    return type->CalcFuncType(UniquePtrCast<TupleType>(varPart->Check(table, ok)), ok);
                return type->CalcArrayType(UniquePtrCast<TupleType>(varPart->Check(table, ok)), ok);
            }
            TupleType *emptyTuple = new TupleType();
            return type->CalcFuncType(std::unique_ptr<TupleType>(emptyTuple), ok);
        }
        if (varPart != nullptr)
        {
            if (varPart->isProcedureCall)
                return type->CalcFuncType(UniquePtrCast<TupleType>(varPart->Check(table, ok)), ok);
            //TODO: Check Array Range
            return type->CalcArrayType(UniquePtrCast<TupleType>(varPart->Check(table, ok)), ok);
        }
        return type->Copy();
    }

    std::unique_ptr<TypeInfo> VariableList::Check(SymbolTable &table, bool &ok)
    {
        std::vector<std::unique_ptr<TypeInfo>> types;
        for (auto &variable : variables)
        {
            auto type(variable->Check(table, ok));
            if ((type->GetTypeId() != LVALUE) && (type->GetTypeId() != REF))
            {
                //TODO
                ok = false;
            }
            types.push_back(std::move(type));
        }
        TypeInfo *tupleType = new TupleType(std::move(types));
        return std::unique_ptr<TypeInfo>(tupleType);
    }

    std::unique_ptr<TypeInfo> Factor::Check(SymbolTable &table, bool &ok)
    {
        TypeInfo *ret = new RValueType(GenType(VOID));
        return std::unique_ptr<TypeInfo>(ret);
    }

    std::unique_ptr<TypeInfo> ExpressionFactor::Check(SymbolTable &table, bool &ok)
    {
        return expression->Check(table, ok);
    }

    std::unique_ptr<TypeInfo> NumFactor::Check(SymbolTable &table, bool &ok)
    {
        TypeInfo *ret = new RValueType(GenTypeByStr(type));
        return std::unique_ptr<TypeInfo>(ret);
    }

    std::unique_ptr<TypeInfo> InvFactor::Check(SymbolTable &table, bool &ok)
    {
        //TODO type check
        TypeInfo *ret = new RValueType(subFactor->Check(table, ok));
        return std::unique_ptr<TypeInfo>(ret);
    }

    std::unique_ptr<TypeInfo> VariableFactor::Check(SymbolTable &table, bool &ok)
    {
        return variable->Check(table, ok);
    }

    std::unique_ptr<TypeInfo> NotFactor::Check(SymbolTable &table, bool &ok)
    {
        //TODO type check
        TypeInfo *ret = new RValueType(subFactor->Check(table, ok));
        return std::unique_ptr<TypeInfo>(ret);
    }

    std::unique_ptr<TypeInfo> MulOpPart::Check(SymbolTable &table, bool &ok)
    {
        if (followPart == nullptr)
            return secondFactor->Check(table, ok);
        return followPart->Check(table, ok)->CalcType(secondFactor->Check(table, ok), ok);
    }

    std::unique_ptr<TypeInfo> Term::Check(SymbolTable &table, bool &ok)
    {
        if (mulOpPart == nullptr)
            return firstFactor->Check(table, ok);
        return mulOpPart->Check(table, ok)->CalcType(firstFactor->Check(table, ok), ok);
    }

    std::unique_ptr<TypeInfo> AddOpPart::Check(SymbolTable &table, bool &ok)
    {
        if (followPart == nullptr)
            return secondTerm->Check(table, ok);
        return followPart->Check(table, ok)->CalcType(secondTerm->Check(table, ok), ok);
    }

    std::unique_ptr<TypeInfo> SimpleExpression::Check(SymbolTable &table, bool &ok)
    {
        if (addOpPart == nullptr)
            return firstTerm->Check(table, ok);
        return addOpPart->Check(table, ok)->CalcType(firstTerm->Check(table, ok), ok);
    }

    std::unique_ptr<TypeInfo> RelPart::Check(SymbolTable &table, bool &ok)
    {
        return secondExpression->Check(table, ok);
    }

    std::unique_ptr<TypeInfo> Expression::Check(SymbolTable &table, bool &ok)
    {
        if (relPart == nullptr)
            return firstExpression->Check(table, ok);
        firstExpression->Check(table, ok);
        relPart->Check(table, ok);
        //TODO
        TypeInfo *ret = new RValueType(GenType(BOOLEAN));
        return std::unique_ptr<TypeInfo>(ret);
    }

    std::unique_ptr<TypeInfo> ExpressionList::Check(SymbolTable &table, bool &ok)
    {
        std::vector<std::unique_ptr<TypeInfo>> types;
        for (auto &expression : expressions)
        {
            types.push_back(expression->Check(table, ok));
        }
        TypeInfo *tupleType = new TupleType(std::move(types));
        return std::unique_ptr<TypeInfo>(tupleType);
    }

    std::unique_ptr<TypeInfo> VarPart::Check(SymbolTable &table, bool &ok)
    {
        return expressionList->Check(table, ok);
    }

    std::unique_ptr<TypeInfo> Statement::Check(SymbolTable &table, bool &ok)
    {
        return GenType(VOID);
    }

    std::unique_ptr<TypeInfo> VariableAssignStatement::Check(SymbolTable &table, bool &ok)
    {
        variable->Check(table, ok);
        int layer;
        bool has;
        auto &item = table.FindSymbol(variable->name, has, layer);
        if (has && item->second.isConstant)
        {
            //TODO
            ok = false;
        }
        expression->Check(table, ok);
        if (!variable->Check(table, ok)->AssignCompatible(expression->Check(table, ok)))
        {
            //TODO
            ok = false;
        }
        return GenType(VOID);
    }

    std::unique_ptr<TypeInfo> ProcedureCallStatement::Check(SymbolTable &table, bool &ok)
    {
        variable->Check(table, ok);
        return GenType(VOID);
    }

    std::unique_ptr<TypeInfo> SubCompoundStatement::Check(SymbolTable &table, bool &ok)
    {
        compoundStatement->Check(table, ok);
        return GenType(VOID);
    }
    std::unique_ptr<TypeInfo> IfElseStatement::Check(SymbolTable &table, bool &ok)
    {
        ifExpression->Check(table, ok);
        if (thenStatement != nullptr)
            thenStatement->Check(table, ok);
        if (elseStatement != nullptr)
            elseStatement->Check(table, ok);
        return GenType(VOID);
    }
    std::unique_ptr<TypeInfo> ForLoopStatement::Check(SymbolTable &table, bool &ok)
    {
        int layer;
        bool has;
        auto &item = table.FindSymbol(counter, has, layer);
        auto &itemType = item->second.type;
        if (!has)
        {
            //TODO
            ok = false;
        }
        else
        {
            if (item->second.isConstant)
            {
                //TODO
                ok = false;
            }
        }
        if (!itemType->AssignCompatible(initExpression->Check(table, ok)))
        {
            ok = false;
        }
        termiExpression->Check(table, ok);
        if (loopStatement != nullptr)
            loopStatement->Check(table, ok);
        return GenType(VOID);
    }
    std::unique_ptr<TypeInfo> ReadStatement::Check(SymbolTable &table, bool &ok)
    {
        variableList->Check(table, ok);
        return GenType(VOID);
    }
    std::unique_ptr<TypeInfo> WriteStatement::Check(SymbolTable &table, bool &ok)
    {
        auto types = UniquePtrCast<TupleType>(expressionList->Check(table, ok))->GetSubTypes();
        //TODO typeStr
        typeStr = "";
        for (auto &type : types)
        {
            TypeID tp = UniquePtrCast<WrapperType>(type->Copy())->DeWrap()->GetTypeId();
            switch (tp)
            {
            case BOOLEAN:
            case INTEGER:
                typeStr += "\%d";
                break;
            case REAL:
                typeStr += "\%f";
                break;
            case CHAR:
                typeStr += "\%c";
                break;
            }
        }
        return GenType(VOID);
    }
    std::unique_ptr<TypeInfo> StatementList::Check(SymbolTable &table, bool &ok)
    {
        for (auto &statement : statements)
        {
            if (statement != nullptr)
                statement->Check(table, ok);
        }
        return GenType(VOID);
    }
    std::unique_ptr<TypeInfo> CompoundStatement::Check(SymbolTable &table, bool &ok)
    {
        statementList->Check(table, ok);
        return GenType(VOID);
    }

    std::unique_ptr<TypeInfo> SubProgramHead::Check(SymbolTable &table, bool &ok)
    {
        table.InsertSymbol(name, std::unique_ptr<TypeInfo>(), true, "");
        auto retType(GenType(VOID));
        if (returnType != nullptr)
            retType = returnType->Check(table, ok);

        std::unique_ptr<TypeInfo> funcType(
            new FuncType(
                UniquePtrCast<TupleType>(parameters->Check(table, ok)),
                std::move(retType)));

        TypeInfo *lValueType = new LValueType(std::move(funcType));
        std::unique_ptr<TypeInfo> ret(lValueType);
        table.InsertSymbol(name, ret->Copy(), true, "");

        return ret;
    }

    std::unique_ptr<TypeInfo> SubProgramBody::Check(SymbolTable &table, bool &ok)
    {
        constantDeclarations->Check(table, ok);
        variableDeclarations->Check(table, ok);
        compoundStatement->Check(table, ok);
        return GenType(VOID);
    }
    std::unique_ptr<TypeInfo> SubProgram::Check(SymbolTable &table, bool &ok)
    {
        if (table.SymbolAtTop(head->name))
        {
            //TODO
            ok = false;
        }
        table.PushMap();
        table.Step();
        auto funcType(head->Check(table, ok));
        body->Check(table, ok);
        table.PopMap();
        table.InsertSymbol(head->name, std::move(funcType), true, "");

        return GenType(VOID);
    }
    std::unique_ptr<TypeInfo> SubProgramDeclarations::Check(SymbolTable &table, bool &ok)
    {
        for (auto &subProgram : subPrograms)
            subProgram->Check(table, ok);
        return GenType(VOID);
    }

    std::unique_ptr<TypeInfo> ProgramHead::Check(SymbolTable &table, bool &ok)
    {
        table.InsertSymbol(name, std::unique_ptr<TypeInfo>(), true, "");
        identifiers->Check(table, ok);
        for (auto &id : identifiers->identifiers)
        {
            if (table.SymbolAtTop(id))
            {
                //TODO
                ok = false;
            }
            table.InsertSymbol(id, std::unique_ptr<TypeInfo>(), true, "");
        }

        return GenType(VOID);
    }

    std::unique_ptr<TypeInfo> ProgramBody::Check(SymbolTable &table, bool &ok)
    {
        constantDeclarations->Check(table, ok);
        variableDeclarations->Check(table, ok);
        subProgramDeclarations->Check(table, ok);
        compoundStatemnet->Check(table, ok);
        return GenType(VOID);
    }
    std::unique_ptr<TypeInfo> Program::Check(SymbolTable &table, bool &ok)
    {
        table.PushMap();
        table.Step();
        programHead->Check(table, ok);
        programBody->Check(table, ok);
        table.PopMap();
        return GenType(VOID);
    }
}