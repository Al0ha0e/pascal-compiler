#include "ast.h"

namespace PascalAST
{

    void Term::Rotate()
    {
        if (mulOpPart == nullptr)
            return;

        MulOpPart *mulPartP = new MulOpPart(mulOpPart->mulOp, std::move(firstFactor), std::unique_ptr<MulOpPart>());
        std::unique_ptr<MulOpPart> curMulPart(mulPartP);
        while (mulOpPart->followPart != nullptr)
        {
            mulPartP = new MulOpPart(
                mulOpPart->followPart->mulOp,
                std::move(mulOpPart->secondFactor),
                std::move(curMulPart));
            curMulPart = std::unique_ptr<MulOpPart>(mulPartP);
            mulOpPart = std::move(mulOpPart->followPart);
        }
        firstFactor = std::move(mulOpPart->secondFactor);
        mulOpPart = std::move(curMulPart);
    }

    void SimpleExpression::Rotate()
    {
        if (addOpPart == nullptr)
            return;
        AddOpPart *addPartP = new AddOpPart(addOpPart->addOp, std::move(firstTerm), std::unique_ptr<AddOpPart>());
        std::unique_ptr<AddOpPart> curAddPart(addPartP);
        while (addOpPart->followPart != nullptr)
        {
            addPartP = new AddOpPart(
                addOpPart->followPart->addOp,
                std::move(addOpPart->secondTerm),
                std::move(curAddPart));
            curAddPart = std::unique_ptr<AddOpPart>(addPartP);
            addOpPart = std::move(addOpPart->followPart);
        }
        firstTerm = std::move(addOpPart->secondTerm);
        addOpPart = std::move(curAddPart);
    }

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
        return GenTypeByStr(basicType);
    }

    std::unique_ptr<TypeInfo> ArrayTypeDecl::Check(SymbolTable &table)
    {
        ranges->Check(table);
        TypeInfo *arrType = new ArrayType(int(ranges->ranges.size()), type->Check(table)->Copy());
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
            if (table.SymbolAtTop(id))
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
        identifiers->Check(table);
        return type->Check(table);
    }

    std::unique_ptr<TypeInfo> ParameterList::Check(SymbolTable &table)
    {
        std::vector<std::unique_ptr<TypeInfo>> types;
        for (int i = 0; i < parameters.size(); i++)
        {
            auto pType(parameters[i]->Check(table));
            for (int j = 0; j < parameters[i]->identifiers->identifiers.size(); j++)
            {
                std::string id = parameters[i]->identifiers->identifiers[j];
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
            return GenType(VOID);
        }
        std::unique_ptr<TypeInfo> &type = symbolIt->second.type;
        if (type->GetTypeId() == FUNC)
        {
            if (varPart != nullptr)
            {
                if (varPart->isProcedureCall)
                    return type->CalcFuncType(UniquePtrCast<TupleType>(varPart->Check(table)));
                return type->CalcArrayType(UniquePtrCast<TupleType>(varPart->Check(table)));
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
        for (int i = 0; i < variables.size(); i++)
        {
            types.push_back(variables[i]->Check(table));
        }
        TypeInfo *tupleType = new TupleType(std::move(types));
        return std::unique_ptr<TypeInfo>(tupleType);
    }

    std::unique_ptr<TypeInfo> Factor::Check(SymbolTable &table)
    {
        return GenType(VOID);
    }

    std::unique_ptr<TypeInfo> ExpressionFactor::Check(SymbolTable &table)
    {
        return expression->Check(table);
    }

    std::unique_ptr<TypeInfo> NumFactor::Check(SymbolTable &table)
    {
        return GenTypeByStr(type);
    }

    std::unique_ptr<TypeInfo> InvFactor::Check(SymbolTable &table)
    {
        //TODO type check
        return subFactor->Check(table);
    }

    std::unique_ptr<TypeInfo> VariableFactor::Check(SymbolTable &table)
    {
        return variable->Check(table);
    }

    std::unique_ptr<TypeInfo> NotFactor::Check(SymbolTable &table)
    {
        //TODO type check
        return subFactor->Check(table);
    }

    std::unique_ptr<TypeInfo> MulOpPart::Check(SymbolTable &table)
    {
        if (followPart == nullptr)
            return secondFactor->Check(table);
        return followPart->Check(table)->CalcType(secondFactor->Check(table));
    }

    std::unique_ptr<TypeInfo> Term::Check(SymbolTable &table)
    {
        Rotate();
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
        Rotate();
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
        return GenType(BOOLEAN);
    }

    std::unique_ptr<TypeInfo> ExpressionList::Check(SymbolTable &table)
    {
        std::vector<std::unique_ptr<TypeInfo>> types;
        for (int i = 0; i < expressions.size(); i++)
        {
            types.push_back(expressions[i]->Check(table));
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
        //TODO
        return GenType(VOID);
    }

    std::unique_ptr<TypeInfo> ProcedureCallStatement::Check(SymbolTable &table)
    {
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
        for (int i = 0; i < statements.size(); i++)
        {
            if (statements[i] != nullptr)
                statements[i]->Check(table);
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
        std::vector<bool> isRef;
        auto &params = parameters->parameters;
        for (int i = 0; i < params.size(); i++)
        {
            auto &ids = params[i]->identifiers->identifiers;
            for (int j = 0; j < ids.size(); j++)
                isRef.push_back(params[i]->isRef);
        }
        FuncType *funcType = new FuncType(
            UniquePtrCast<TupleType>(parameters->Check(table)),
            isRef,
            returnType->Check(table));
        table.InsertSymbol(name, std::unique_ptr<TypeInfo>((TypeInfo *)funcType), true, "");
        return GenType(VOID);
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
        head->Check(table);
        body->Check(table);
        table.PopMap();
        return GenType(VOID);
    }
    std::unique_ptr<TypeInfo> SubProgramDeclarations::Check(SymbolTable &table)
    {
        for (int i = 0; i < subPrograms.size(); i++)
            subPrograms[i]->Check(table);
        return GenType(VOID);
    }

    std::unique_ptr<TypeInfo> ProgramHead::Check(SymbolTable &table)
    {
        identifiers->Check(table);
        //TODO: insert identifiers into table
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
        programHead->Check(table);
        programBody->Check(table);
        table.PopMap();
        return GenType(VOID);
    }
}