#include "ast.h"

namespace PascalAST
{
    bool AbstractSyntaxTree::Check()
    {
        bool ok = true;
        astRoot->Check(symTable, ok);
        return ok;
    }

    std::unique_ptr<TypeInfo> OriASTNode::Check(SymbolTable &table, bool &ok)
    {
        //std::cout << "OriASTNode" << std::endl;
        //std::cout << "OriASTNode OVER " << ok << std::endl;
        return GenType(VOID);
    }

    std::unique_ptr<TypeInfo> Identifiers::Check(SymbolTable &table, bool &ok)
    {
        //std::cout << "Identifiers" << std::endl;
        //std::cout << "Identifiers OVER " << ok << std::endl;
        return GenType(VOID);
    }

    std::unique_ptr<TypeInfo> Range::Check(SymbolTable &table, bool &ok)
    {
        //std::cout << "Range" << std::endl;
        if (l > r)
        {
            ok = false;
            logErrMsg(stLine, stColumn, "Error: Upper bound of range is less than lower bound");
        }
        //std::cout << "Range OVER " << ok << std::endl;
        return GenType(VOID);
    }

    std::unique_ptr<TypeInfo> Ranges::Check(SymbolTable &table, bool &ok)
    {
        //std::cout << "Ranges" << std::endl;
        for (auto &range : ranges)
            range->Check(table, ok);
        //std::cout << "Ranges OVER " << ok << std::endl;
        return GenType(VOID);
    }

    std::unique_ptr<TypeInfo> TypeDecl::Check(SymbolTable &table, bool &ok)
    {
        //std::cout << "TypeDecl" << std::endl;
        //std::cout << "TypeDecl OVER " << ok << std::endl;
        return GenType(VOID);
    }

    std::unique_ptr<TypeInfo> BasicTypeDecl::Check(SymbolTable &table, bool &ok)
    {
        //std::cout << "BasicTypeDecl" << std::endl;
        //std::cout << "BasicTypeDecl OVER " << ok << std::endl;
        return GenTypeByStr(basicType);
    }

    std::unique_ptr<TypeInfo> ArrayTypeDecl::Check(SymbolTable &table, bool &ok)
    {
        //std::cout << "ArrayTypeDecl" << std::endl;
        ranges->Check(table, ok);
        std::vector<std::pair<int, int>> arrRanges;
        for (auto &range : ranges->ranges)
            arrRanges.push_back(std::pair<int, int>(range->l, range->r));

        TypeInfo *arrType = new ArrayType(arrRanges, type->Check(table, ok)->Copy());
        //std::cout << "ArrayTypeDecl OVER " << ok << std::endl;
        return std::unique_ptr<TypeInfo>(arrType);
    }

    std::unique_ptr<TypeInfo> ConstantDeclaration::Check(SymbolTable &table, bool &ok)
    {
        //std::cout << "ConstantDeclaration" << std::endl;
        if (table.SymbolAtTop(name))
        {
            ok = false;
            logErrMsg(stLine, stColumn, std::string("Error: Duplicate identifier \"") + name + "\"");
        }
        else
        {
            TypeInfo *lValueType = new LValueType(type->Check(table, ok));
            table.InsertSymbol(name, std::unique_ptr<TypeInfo>(lValueType), true, content);
        }
        //std::cout << "ConstantDeclaration OVER " << ok << std::endl;
        return GenType(VOID);
    }

    std::unique_ptr<TypeInfo> ConstantDeclarations::Check(SymbolTable &table, bool &ok)
    {
        //std::cout << "ConstantDeclarations" << std::endl;
        for (auto &declaration : constantDeclarations)
            declaration->Check(table, ok);
        //std::cout << "ConstantDeclarations OVER " << ok << std::endl;
        return GenType(VOID);
    }

    std::unique_ptr<TypeInfo> VariableDeclaration::Check(SymbolTable &table, bool &ok)
    {
        //std::cout << "VariableDeclaration" << std::endl;
        auto tp(type->Check(table, ok));
        identifiers->Check(table, ok);
        int layer;
        for (int i = 0; i < identifiers->identifiers.size(); i++)
        {
            std::string id = identifiers->identifiers[i];
            //std::cout << "DECL ID " << id << std::endl;
            if (table.SymbolAtTop(id))
            {
                ok = false;
                logErrMsg(identifiers->stLines[i], identifiers->stColumns[i], std::string("Error: Duplicate identifier \"") + id + "\"");
            }
            else
            {
                TypeInfo *lValueType = new LValueType(tp->Copy());
                table.InsertSymbol(id, std::unique_ptr<TypeInfo>(lValueType), false, "");
            }
        }
        //std::cout << "VariableDeclaration OVER " << ok << std::endl;
        return GenType(VOID);
    }

    std::unique_ptr<TypeInfo> VariableDeclarations::Check(SymbolTable &table, bool &ok)
    {
        //std::cout << "VariableDeclarations" << std::endl;
        for (auto &declaration : variableDeclarations)
            declaration->Check(table, ok);
        //std::cout << "VariableDeclarations OVER " << ok << std::endl;
        return GenType(VOID);
    }

    std::unique_ptr<TypeInfo> Parameter::Check(SymbolTable &table, bool &ok)
    {
        //std::cout << "Parameter" << std::endl;
        identifiers->Check(table, ok);
        TypeInfo *ret;
        if (isRef)
        {
            //std::cout << "REF" << std::endl;
            ret = new RefType(type->Check(table, ok));
        }
        else
        {
            //std::cout << "LVAL" << std::endl;
            ret = new LValueType(type->Check(table, ok));
        }
        //std::cout << "Parameter OVER " << ok << std::endl;
        return std::unique_ptr<TypeInfo>(ret);
    }

    std::unique_ptr<TypeInfo> ParameterList::Check(SymbolTable &table, bool &ok)
    {
        //std::cout << "ParameterList" << std::endl;
        std::vector<std::unique_ptr<TypeInfo>> types;
        for (auto &parameter : parameters)
        {
            auto pType(parameter->Check(table, ok));
            auto &identifiers = parameter->identifiers;
            for (int i = 0; i < identifiers->identifiers.size(); i++)
            {
                std::string id = identifiers->identifiers[i];
                if (table.SymbolAtTop(id))
                {
                    ok = false;
                    logErrMsg(identifiers->stLines[i], identifiers->stColumns[i], std::string("Error: Duplicate identifier \"") + id + "\"");
                }
                else
                {
                    table.InsertSymbol(id, pType->Copy(), false, "");
                }
                types.push_back(pType->Copy());
            }
        }
        TypeInfo *tupleType = new TupleType(std::move(types));
        //std::cout << "ParameterList OVER " << ok << std::endl;
        return std::unique_ptr<TypeInfo>(tupleType);
    }

    std::unique_ptr<TypeInfo> Variable::Check(SymbolTable &table, bool &ok)
    {
        //std::cout << "Variable" << std::endl;
        bool has;
        int layer;
        auto symbolIt = table.FindSymbol(name, has, layer);

        if (!has)
        {
            ok = false;
            logErrMsg(stLine, stColumn, std::string("Error: Identifier not found \"") + name + "\"");

            TypeInfo *ret = new RValueType(GenType(VOID));
            //std::cout << "Variable OVER " << ok << std::endl;
            return std::unique_ptr<TypeInfo>(ret);
        }
        std::unique_ptr<TypeInfo> &type = symbolIt->second.type;
        //std::cout << "VTYPE " << type->GetTypeId() << std::endl;
        auto targetType(((WrapperType *)type.get())->DeWrap());
        //std::cout << "TARGET TYPE " << targetType->GetTypeId() << std::endl;
        if (targetType->GetTypeId() == FUNC)
        {
            if (varPart != nullptr)
            {
                auto argTypes(UniquePtrCast<FuncType>(targetType)->GetArgTypes());
                for (auto &argType : argTypes)
                    varPart->argIsRef.push_back(argType->GetTypeId() == REF);

                if (varPart->isProcedureCall)
                {
                    //std::cout << "Variable OVER " << ok << std::endl;
                    return type->CalcFuncType(UniquePtrCast<TupleType>(varPart->Check(table, ok)), ok);
                }
                //std::cout << "Variable OVER " << ok << std::endl;
                return type->CalcArrayType(UniquePtrCast<TupleType>(varPart->Check(table, ok)), ok);
            }

            if (isAssignLeft && table.SymbolAtTop(name))
            {
                //std::cout << "Variable OVER " << ok << std::endl;
                return ((FuncType *)targetType.get())->RetAsLValue();
            }

            TupleType *emptyTuple = new TupleType();
            //std::cout << "Variable OVER " << ok << std::endl;
            return type->CalcFuncType(std::unique_ptr<TupleType>(emptyTuple), ok);
        }
        if (varPart != nullptr)
        {
            if (varPart->isProcedureCall)
            {
                //std::cout << "Variable OVER " << ok << std::endl;
                return type->CalcFuncType(UniquePtrCast<TupleType>(varPart->Check(table, ok)), ok);
            }
            if (targetType->GetTypeId() == ARRAY)
            {
                varPart->indexOffset = ((ArrayType *)targetType.get())->GetOffset();
            }
            //std::cout << "Variable OVER " << ok << std::endl;
            return type->CalcArrayType(UniquePtrCast<TupleType>(varPart->Check(table, ok)), ok);
        }
        //std::cout << "Variable OVER " << ok << std::endl;
        return type->Copy();
    }

    std::unique_ptr<TypeInfo> VariableList::Check(SymbolTable &table, bool &ok)
    {
        //std::cout << "VariableList" << std::endl;
        std::vector<std::unique_ptr<TypeInfo>> types;
        for (int i = 0; i < variables.size(); i++)
        {
            auto &variable = variables[i];
            auto type(variable->Check(table, ok));
            bool has;
            int layer;
            if (table.FindSymbol(variable->name, has, layer)->second.isConstant)
            {
                ok = false;
                logErrMsg(variable->stLine, variable->stColumn, "Error: Variable identifier expected");
            }
            if ((type->GetTypeId() != LVALUE) && (type->GetTypeId() != REF))
            {
                ok = false;
                logErrMsg(variable->stLine, variable->stColumn, "Error: lvalue/ref expected");
            }
            types.push_back(std::move(type));
        }

        TypeInfo *tupleType = new TupleType(std::move(types));
        //std::cout << "VariableList OVER " << ok << std::endl;
        return std::unique_ptr<TypeInfo>(tupleType);
    }

    std::unique_ptr<TypeInfo> Factor::Check(SymbolTable &table, bool &ok)
    {
        //std::cout << "Factor" << std::endl;
        TypeInfo *ret = new RValueType(GenType(VOID));
        //std::cout << "Factor OVER " << ok << std::endl;
        return std::unique_ptr<TypeInfo>(ret);
    }

    std::unique_ptr<TypeInfo> ExpressionFactor::Check(SymbolTable &table, bool &ok)
    {
        //std::cout << "ExpressionFactor" << std::endl;
        //std::cout << "ExpressionFactor OVER " << ok << std::endl;
        return expression->Check(table, ok);
    }

    std::unique_ptr<TypeInfo> NumFactor::Check(SymbolTable &table, bool &ok)
    {
        //std::cout << "NumFactor" << std::endl;
        TypeInfo *ret = new RValueType(GenTypeByStr(type));
        //std::cout << "NumFactor OVER " << ok << std::endl;
        return std::unique_ptr<TypeInfo>(ret);
    }

    std::unique_ptr<TypeInfo> InvFactor::Check(SymbolTable &table, bool &ok)
    {
        //std::cout << "InvFactor" << std::endl;
        //TODO type check
        TypeInfo *ret = new RValueType(subFactor->Check(table, ok));
        //std::cout << "InvFactor OVER " << ok << std::endl;
        return std::unique_ptr<TypeInfo>(ret);
    }

    std::unique_ptr<TypeInfo> VariableFactor::Check(SymbolTable &table, bool &ok)
    {
        //std::cout << "VariableFactor" << std::endl;
        //std::cout << "VariableFactor OVER " << ok << std::endl;
        return variable->Check(table, ok);
    }

    std::unique_ptr<TypeInfo> NotFactor::Check(SymbolTable &table, bool &ok)
    {
        //std::cout << "NotFactor" << std::endl;
        //TODO type check
        TypeInfo *ret = new RValueType(subFactor->Check(table, ok));
        //std::cout << "NotFactor OVER " << ok << std::endl;
        return std::unique_ptr<TypeInfo>(ret);
    }

    std::unique_ptr<TypeInfo> MulOpPart::Check(SymbolTable &table, bool &ok)
    {
        //std::cout << "MulOpPart" << std::endl;
        if (followPart == nullptr)
        {
            //std::cout << "MulOpPart OVER " << ok << std::endl;
            return secondFactor->Check(table, ok);
        }
        //std::cout << "MulOpPart OVER " << ok << std::endl;
        return followPart->Check(table, ok)->CalcType(secondFactor->Check(table, ok), ok);
    }

    std::unique_ptr<TypeInfo> Term::Check(SymbolTable &table, bool &ok)
    {
        //std::cout << "Term" << std::endl;
        if (mulOpPart == nullptr)
        {
            //std::cout << "Term OVER " << ok << std::endl;
            return firstFactor->Check(table, ok);
        }
        //std::cout << "Term OVER " << ok << std::endl;
        return mulOpPart->Check(table, ok)->CalcType(firstFactor->Check(table, ok), ok);
    }

    std::unique_ptr<TypeInfo> AddOpPart::Check(SymbolTable &table, bool &ok)
    {
        //std::cout << "AddOpPart" << std::endl;
        if (followPart == nullptr)
        {
            //std::cout << "AddOpPart OVER " << ok << std::endl;
            return secondTerm->Check(table, ok);
        }

        //std::cout << "AddOpPart OVER " << ok << std::endl;
        return followPart->Check(table, ok)->CalcType(secondTerm->Check(table, ok), ok);
    }

    std::unique_ptr<TypeInfo> SimpleExpression::Check(SymbolTable &table, bool &ok)
    {
        //std::cout << "SimpleExpression" << std::endl;
        if (addOpPart == nullptr)
        {
            //std::cout << "SimpleExpression OVER " << ok << std::endl;
            return firstTerm->Check(table, ok);
        }
        //std::cout << "SimpleExpression OVER " << ok << std::endl;
        return addOpPart->Check(table, ok)->CalcType(firstTerm->Check(table, ok), ok);
    }

    std::unique_ptr<TypeInfo> RelPart::Check(SymbolTable &table, bool &ok)
    {
        //std::cout << "RelPart" << std::endl;
        //std::cout << "RelPart OVER " << ok << std::endl;
        return secondExpression->Check(table, ok);
    }

    std::unique_ptr<TypeInfo> Expression::Check(SymbolTable &table, bool &ok)
    {
        //std::cout << "Expression" << std::endl;
        if (relPart == nullptr)
        {
            //std::cout << "Expression OVER " << ok << std::endl;
            return firstExpression->Check(table, ok);
        }
        firstExpression->Check(table, ok);
        relPart->Check(table, ok);
        //TODO
        TypeInfo *ret = new RValueType(GenType(BOOLEAN));
        //std::cout << "Expression OVER " << ok << std::endl;
        return std::unique_ptr<TypeInfo>(ret);
    }

    std::unique_ptr<TypeInfo> ExpressionList::Check(SymbolTable &table, bool &ok)
    {
        //std::cout << "ExpressionList" << std::endl;
        std::vector<std::unique_ptr<TypeInfo>> types;
        for (auto &expression : expressions)
        {
            types.push_back(expression->Check(table, ok));
        }
        TypeInfo *tupleType = new TupleType(std::move(types));
        //std::cout << "ExpressionList OVER " << ok << std::endl;
        return std::unique_ptr<TypeInfo>(tupleType);
    }

    std::unique_ptr<TypeInfo> VarPart::Check(SymbolTable &table, bool &ok)
    {
        //std::cout << "VarPart" << std::endl;
        //std::cout << "VarPart OVER " << ok << std::endl;
        return expressionList->Check(table, ok);
    }

    std::unique_ptr<TypeInfo> Statement::Check(SymbolTable &table, bool &ok)
    {
        //std::cout << "Statement" << std::endl;
        //std::cout << "Statement OVER " << ok << std::endl;
        return GenType(VOID);
    }

    std::unique_ptr<TypeInfo> VariableAssignStatement::Check(SymbolTable &table, bool &ok)
    {
        //std::cout << "VariableAssignStatement" << std::endl;
        auto varType(variable->Check(table, ok));
        int layer;
        bool has;
        const auto &item = table.FindSymbol(variable->name, has, layer);
        auto expressionType = expression->Check(table, ok);
        if (has && item->second.isConstant)
        {
            ok = false;
            logErrMsg(variable->stLine, variable->stColumn, "Error: Variable identifier expected");
        }
        else if (!varType->AssignCompatible(expressionType->Copy()))
        {
            ok = false;
            logErrMsg(variable->stLine, variable->stColumn,
                      std::string("Error: type ") + expressionType->ToString() + " cannot be assigned to type " + varType->ToString());
        }
        //std::cout << "VariableAssignStatement OVER " << ok << std::endl;
        return GenType(VOID);
    }

    std::unique_ptr<TypeInfo> ProcedureCallStatement::Check(SymbolTable &table, bool &ok)
    {
        //std::cout << "ProcedureCallStatement" << std::endl;
        variable->Check(table, ok);
        //std::cout << "ProcedureCallStatement OVER " << ok << std::endl;
        return GenType(VOID);
    }

    std::unique_ptr<TypeInfo> SubCompoundStatement::Check(SymbolTable &table, bool &ok)
    {
        //std::cout << "SubCompoundStatement" << std::endl;
        compoundStatement->Check(table, ok);
        //std::cout << "SubCompoundStatement OVER " << ok << std::endl;
        return GenType(VOID);
    }
    std::unique_ptr<TypeInfo> IfElseStatement::Check(SymbolTable &table, bool &ok)
    {
        //std::cout << "IfElseStatement" << std::endl;
        ifExpression->Check(table, ok);
        if (thenStatement != nullptr)
            thenStatement->Check(table, ok);
        if (elseStatement != nullptr)
            elseStatement->Check(table, ok);
        //std::cout << "IfElseStatement OVER " << ok << std::endl;
        return GenType(VOID);
    }
    std::unique_ptr<TypeInfo> ForLoopStatement::Check(SymbolTable &table, bool &ok)
    {
        //std::cout << "ForLoopStatement" << std::endl;
        int layer;
        bool has;
        const auto &item = table.FindSymbol(counter, has, layer);
        //std::cout << "FOUND LAYER " << layer << std::endl;
        if (!has)
        {
            ok = false;
            logErrMsg(stLine, stColumn, std::string("Error: Identifier not found \"") + counter + "\"");
        }
        else
        {
            if (item->second.isConstant)
            {
                ok = false;
                logErrMsg(stLine, stColumn, "Error: Variable identifier expected");
            }
            auto &itemType = item->second.type;
            auto &valType = initExpression->Check(table, ok);
            //std::cout << "FOR LOOP " << itemType->GetTypeId() << std::endl;
            if (!itemType->AssignCompatible(valType->Copy()))
            {
                ok = false;
                logErrMsg(stLine, stColumn,
                          std::string("Error: type ") + valType->ToString() + " cannot be assigned to type " + itemType->ToString());
            }
        }
        termiExpression->Check(table, ok);
        if (loopStatement != nullptr)
            loopStatement->Check(table, ok);
        //std::cout << "ForLoopStatement OVER " << ok << std::endl;
        return GenType(VOID);
    }
    std::unique_ptr<TypeInfo> ReadStatement::Check(SymbolTable &table, bool &ok)
    {
        //std::cout << "ReadStatement" << std::endl;
        variableList->Check(table, ok);
        //std::cout << "ReadStatement OVER " << ok << std::endl;
        return GenType(VOID);
    }
    std::unique_ptr<TypeInfo> WriteStatement::Check(SymbolTable &table, bool &ok)
    {
        //std::cout << "WriteStatement" << std::endl;
        auto types = UniquePtrCast<TupleType>(expressionList->Check(table, ok))->GetSubTypes();
        typeStr = "\"";
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
        typeStr += "\"";
        //std::cout << "WriteStatement OVER " << ok << std::endl;
        return GenType(VOID);
    }
    std::unique_ptr<TypeInfo> StatementList::Check(SymbolTable &table, bool &ok)
    {
        //std::cout << "StatementList" << std::endl;
        for (auto &statement : statements)
        {
            if (statement != nullptr)
                statement->Check(table, ok);
        }
        //std::cout << "StatementList OVER " << ok << std::endl;
        return GenType(VOID);
    }
    std::unique_ptr<TypeInfo> CompoundStatement::Check(SymbolTable &table, bool &ok)
    {
        //std::cout << "CompoundStatement" << std::endl;
        statementList->Check(table, ok);
        //std::cout << "CompoundStatement OVER " << ok << std::endl;
        return GenType(VOID);
    }

    std::unique_ptr<TypeInfo> SubProgramHead::Check(SymbolTable &table, bool &ok)
    {
        //std::cout << "SubProgramHead" << std::endl;
        auto lVoid = std::unique_ptr<TypeInfo>(new LValueType(GenType(VOID)));
        table.InsertSymbol(name, lVoid->Copy(), false, "");
        auto retType(lVoid->Copy());
        if (returnType != nullptr)
        {
            //std::cout << "SubProgramHead OVER " << ok << std::endl;
            retType = returnType->Check(table, ok);
        }
        std::unique_ptr<TypeInfo> funcType(
            new FuncType(
                UniquePtrCast<TupleType>(parameters->Check(table, ok)),
                std::move(retType)));

        TypeInfo *lValueType = new LValueType(std::move(funcType));
        std::unique_ptr<TypeInfo> ret(lValueType);
        table.InsertSymbol(name, ret->Copy(), false, "");
        //std::cout << "SubProgramHead OVER " << ok << std::endl;
        return ret;
    }

    std::unique_ptr<TypeInfo> SubProgramBody::Check(SymbolTable &table, bool &ok)
    {
        //std::cout << "SubProgramBody" << std::endl;
        constantDeclarations->Check(table, ok);
        variableDeclarations->Check(table, ok);
        compoundStatement->Check(table, ok);
        //std::cout << "SubProgramBody OVER " << ok << std::endl;
        return GenType(VOID);
    }
    std::unique_ptr<TypeInfo> SubProgram::Check(SymbolTable &table, bool &ok)
    {
        //std::cout << "SubProgram" << std::endl;
        if (table.SymbolAtTop(head->name))
        {
            ok = false;
            logErrMsg(head->stLine, head->stColumn, std::string("Error: Duplicate identifier \"") + head->name + "\"");
        }
        table.PushMap();
        table.Step();
        auto funcType(head->Check(table, ok));
        body->Check(table, ok);
        table.PopMap();
        table.InsertSymbol(head->name, std::move(funcType), false, "");
        //std::cout << "SubProgram OVER " << ok << std::endl;
        return GenType(VOID);
    }
    std::unique_ptr<TypeInfo> SubProgramDeclarations::Check(SymbolTable &table, bool &ok)
    {
        //std::cout << "SubProgramDeclarations" << std::endl;
        for (auto &subProgram : subPrograms)
            subProgram->Check(table, ok);
        //std::cout << "SubProgramDeclarations OVER " << ok << std::endl;
        return GenType(VOID);
    }

    std::unique_ptr<TypeInfo> ProgramHead::Check(SymbolTable &table, bool &ok)
    {
        //std::cout << "ProgramHead" << std::endl;
        auto lVoid = std::unique_ptr<TypeInfo>(new LValueType(GenType(VOID)));
        table.InsertSymbol(name, lVoid->Copy(), false, "");
        identifiers->Check(table, ok);
        for (int i = 0; i < identifiers->identifiers.size(); i++)
        {
            auto id = identifiers->identifiers[i];
            if (table.SymbolAtTop(id))
            {
                ok = false;
                logErrMsg(identifiers->stLines[i], identifiers->stColumns[i], std::string("Error: Duplicate identifier \"") + id + "\"");
            }
            table.InsertSymbol(id, lVoid->Copy(), false, "");
        }

        //std::cout << "ProgramHead OVER " << ok << std::endl;
        return GenType(VOID);
    }

    std::unique_ptr<TypeInfo> ProgramBody::Check(SymbolTable &table, bool &ok)
    {
        //std::cout << "ProgramBody" << std::endl;
        constantDeclarations->Check(table, ok);
        variableDeclarations->Check(table, ok);
        subProgramDeclarations->Check(table, ok);
        compoundStatemnet->Check(table, ok);
        //std::cout << "ProgramBody OVER " << ok << std::endl;
        return GenType(VOID);
    }
    std::unique_ptr<TypeInfo> Program::Check(SymbolTable &table, bool &ok)
    {
        //std::cout << "Program" << std::endl;
        table.PushMap();
        table.Step();
        programHead->Check(table, ok);
        programBody->Check(table, ok);
        table.PopMap();
        //std::cout << "Program OVER " << ok << std::endl;
        return GenType(VOID);
    }
}