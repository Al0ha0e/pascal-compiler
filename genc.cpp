#include "ast.h"

namespace PascalAST
{

    std::string ASTNode::GenCCode(SymbolTable &table, bool isRef)
    {
        return "";
    }

    std::string Identifiers::GenCCode(SymbolTable &table, bool isRef)
    {
        std::string ret;
        for (auto &id : identifiers)
            ret += id + ",";
        if (ret.size())
            ret.pop_back();
        return ret;
    }

    std::string BasicTypeDecl::GenCCode(SymbolTable &table, bool isRef)
    {
        if (basicType == "real")
            return "float";
        if (basicType == "boolean")
            return "int";
        return basicType;
    }

    std::string ConstantDeclaration::GenCCode(SymbolTable &table, bool isRef)
    {
        std::string ret =
            "const " + type->GenCCode(table, isRef) + " " + name + " = " + content + ";\n";
        return ret;
    }

    std::string ConstantDeclarations::GenCCode(SymbolTable &table, bool isRef)
    {
        std::string ret;
        for (auto &decl : constantDeclarations)
            ret += decl->GenCCode(table, isRef);
        return ret;
    }

    std::string VariableDeclaration::GenCCode(SymbolTable &table, bool isRef)
    {
        std::string ret;
        if (typeid(type) == typeid(BasicTypeDecl))
        {
            ret = type->GenCCode(table, isRef) + " " + identifiers->GenCCode(table, isRef) + ";\n";
        }
        else if (typeid(type) == typeid(ArrayTypeDecl))
        {
            ArrayTypeDecl *arrType = (ArrayTypeDecl *)type.get();
            ret = arrType->type->GenCCode(table, isRef) + " ";
            for (auto &id : identifiers->identifiers)
            {
                ret += id;
                for (auto &range : arrType->ranges->ranges)
                {
                    ret += "[";
                    ret += std::to_string(range->r - range->l + 1) + "],";
                }
            }
            ret[ret.length() - 1] = ';';
            ret += "\n";
        }
        return ret;
    }

    std::string VariableDeclarations::GenCCode(SymbolTable &table, bool isRef)
    {
        std::string ret;
        for (auto &decl : variableDeclarations)
            ret += decl->GenCCode(table, isRef);
        return ret;
    }

    std::string Parameter::GenCCode(SymbolTable &table, bool isRef)
    {
        std::string ret;
        for (auto &id : identifiers->identifiers)
        {
            ret += type->GenCCode(table, isRef) + " ";
            ret += isRef ? "*" : "";
            ret += id + ",";
        }
        return ret;
    }

    std::string ParameterList::GenCCode(SymbolTable &table, bool isRef)
    {
        std::string ret;
        for (auto &param : parameters)
            ret += param->GenCCode(table, isRef);
        if (ret.size())
            ret.pop_back();
        return ret;
    }

    std::string Variable::GenCCode(SymbolTable &table, bool isRef)
    {
        std::string ret;
        bool has;
        int layer;
        auto &type = table.FindSymbol(name, has, layer)->second.type;
        auto targetType(((WrapperType *)type.get())->DeWrap());
        if (varPart != nullptr)
        {
            ret = varPart->GenCCode(table, isRef);
        }
        else
        {
            if (targetType->GetTypeId() == FUNC)
            {
                ret = name + "()";
            }
            else
            {
                if (type->GetTypeId() == REF)
                {
                    ret = isRef ? name : std::string("(*") + name + ")";
                }
                else if (type->GetTypeId() == LVALUE)
                {
                    ret = isRef ? std::string("(&") + name + ")" : name;
                }
            }
        }
        return ret;
    }

    std::string VariableList::GenCCode(SymbolTable &table, bool isRef)
    {
        std::string ret = "\"";
        bool has;
        int layer;
        for (auto &var : variables)
        {
            auto &type = table.FindSymbol(var->name, has, layer)->second.type;
            auto targetType(((WrapperType *)type.get())->DeWrap());
            int tp = targetType->GetTypeId();
            switch (tp)
            {
            case BOOLEAN:
            case INTEGER:
                ret += "\%d";
                break;
            case REAL:
                ret += "\%f";
                break;
            case CHAR:
                ret += "\%c";
                break;
            }
        }
        ret += "\"";
        for (auto &var : variables)
            ret += std::string(",") + var->GenCCode(table, isRef);
        return ret;
    }

    std::string ExpressionFactor::GenCCode(SymbolTable &table, bool isRef)
    {
        std::string ret = "(";
        return ret + expression->GenCCode(table, isRef) + ")";
    }

    std::string NumFactor::GenCCode(SymbolTable &table, bool isRef)
    {
        return val;
    }

    std::string InvFactor::GenCCode(SymbolTable &table, bool isRef)
    {
        std::string ret = "-(";
        return ret + subFactor->GenCCode(table, false) + ")";
    }

    std::string VariableFactor::GenCCode(SymbolTable &table, bool isRef)
    {
        return variable->GenCCode(table, isRef);
    }

    std::string NotFactor::GenCCode(SymbolTable &table, bool isRef)
    {
        std::string ret = "!(";
        return ret + subFactor->GenCCode(table, false) + ")";
    }

    std::string MulOpPart::GenCCode(SymbolTable &table, bool isRef)
    {
        std::string ret = secondFactor->GenCCode(table, (followPart == nullptr) & isRef) + mulOp;
        if (followPart != nullptr)
            ret += followPart->GenCCode(table, false);
        return ret;
    }

    std::string Term::GenCCode(SymbolTable &table, bool isRef)
    {
        std::string ret;
        if (mulOpPart != nullptr)
            ret = mulOpPart->GenCCode(table, false);
        ret += firstFactor->GenCCode(table, (mulOpPart == nullptr) & isRef);
    }

    std::string AddOpPart::GenCCode(SymbolTable &table, bool isRef)
    {
        std::string ret = secondTerm->GenCCode(table, (followPart == nullptr) & isRef) + addOp;
        if (followPart != nullptr)
            ret += followPart->GenCCode(table, false);
        return ret;
    }

    std::string SimpleExpression::GenCCode(SymbolTable &table, bool isRef)
    {
        std::string ret;
        if (addOpPart != nullptr)
            ret = addOpPart->GenCCode(table, false);
        ret += firstTerm->GenCCode(table, (addOpPart == nullptr) & isRef);
    }

    std::string RelPart::GenCCode(SymbolTable &table, bool isRef)
    {
        std::string ret = relop;
        if (ret == "=")
        {
            ret = "==";
        }
        else if (ret == "<>")
        {
            ret = "!=";
        }
        return ret + secondExpression->GenCCode(table, false);
    }

    std::string Expression::GenCCode(SymbolTable &table, bool isRef)
    {
        std::string ret = firstExpression->GenCCode(table, (relPart == nullptr) & isRef);
        if (relPart != nullptr)
            ret += relPart->GenCCode(table, false);
        return ret;
    }

    std::string ExpressionList::GenCCode(SymbolTable &table, bool isRef)
    {
        std::string ret;
        for (auto &expression : expressions)
            ret += expression->GenCCode(table, isRef) + ",";
        if (ret.size())
            ret.pop_back();
        return ret;
    }

    std::string VarPart::GenCCode(SymbolTable &table, bool isRef)
    {
        std::string ret;
        if (isProcedureCall)
        {
            ret = "(";
            std::string argCode;
            for (int i = 0; i < argIsRef.size(); i++)
            {
                argCode += expressionList->expressions[i]->GenCCode(table, argIsRef[i]);
                argCode += ",";
            }
            if (argCode.length())
                argCode.pop_back();
            ret += argCode + ")";
        }
        else
        {
            for (auto &expression : expressionList->expressions)
            {
                ret += "[";
                ret += expression->GenCCode(table, false) + "]";
            }
        }
        return ret;
    }

    std::string VariableAssignStatement::GenCCode(SymbolTable &table, bool isRef)
    {
        return variable->GenCCode(table, false) + "=" + expression->GenCCode(table, false) + ";\n";
    }

    std::string ProcedureCallStatement::GenCCode(SymbolTable &table, bool isRef)
    {
        return variable->GenCCode(table, false) + ";\n";
    }

    std::string SubCompoundStatement::GenCCode(SymbolTable &table, bool isRef)
    {
        std::string ret = "{\n";
        return ret + compoundStatement->GenCCode(table, isRef) + "}\n";
    }

    std::string IfElseStatement::GenCCode(SymbolTable &table, bool isRef)
    {
        std::string ret = "if(";
        ret += ifExpression->GenCCode(table, false) + "){\n";
        if (thenStatement != nullptr)
            ret += thenStatement->GenCCode(table, false);
        ret += "}\n";
        if (elseStatement != nullptr)
        {
            ret += "else{\n";
            ret += elseStatement->GenCCode(table, false) + "}\n";
        }
        return;
    }

    std::string ForLoopStatement::GenCCode(SymbolTable &table, bool isRef)
    {
        std::string ret = "for(";
        bool has;
        int layer;
        auto &type = table.FindSymbol(counter, has, layer)->second.type;
        bool isRef = type->GetTypeId() == REF;
        std::string counterStr = isRef ? std::string("(*") + counter + ")" : counter;
        ret += counterStr + "=" + initExpression->GenCCode(table, false) + ";";
        ret += counterStr + "<=" + termiExpression->GenCCode(table, false) + ";";
        ret += counterStr + "++){\n";
        if (loopStatement != nullptr)
            ret += loopStatement->GenCCode(table, false);
        ret += "}\n";
        return ret;
    }

    std::string ReadStatement::GenCCode(SymbolTable &table, bool isRef)
    {
        std::string ret = "scanf(";
        ret += variableList->GenCCode(table, true);
        ret += ");\n";
        return ret;
    }

    std::string WriteStatement::GenCCode(SymbolTable &table, bool isRef)
    {
        std::string ret = "printf(";
        ret += typeStr + "," + expressionList->GenCCode(table, false) + ");\n";
        return ret;
    }

    std::string StatementList::GenCCode(SymbolTable &table, bool isRef)
    {
        std::string ret = "";
        for (auto &statement : statements)
            ret += statement->GenCCode(table, isRef);
        return ret;
    }

    std::string CompoundStatement::GenCCode(SymbolTable &table, bool isRef)
    {
        return statementList->GenCCode(table, isRef);
    }

    std::string SubProgramHead::GenCCode(SymbolTable &table, bool isRef)
    {
        std::string ret = returnType == nullptr ? "void " : (returnType->GenCCode(table, isRef) + " ");
        ret += name + "(";
        ret += parameters->GenCCode(table, isRef) + ")\n";
        return ret;
    }

    std::string SubProgramBody::GenCCode(SymbolTable &table, bool isRef)
    {
        std::string ret = constantDeclarations->GenCCode(table, isRef) +
                          variableDeclarations->GenCCode(table, isRef) +
                          compoundStatement->GenCCode(table, isRef);
        return ret;
    }

    std::string SubProgram::GenCCode(SymbolTable &table, bool isRef)
    {
        std::string ret = head->GenCCode(table, isRef) + "{\n";
        ret += body->GenCCode(table, isRef) + "}\n";
        return ret;
    }

    std::string SubProgramDeclarations::GenCCode(SymbolTable &table, bool isRef)
    {
        std::string ret = "";
        for (auto &program : subPrograms)
            ret += program->GenCCode(table, isRef);
        return ret;
    }

    std::string ProgramBody::GenCCode(SymbolTable &table, bool isRef)
    {
        std::string ret = constantDeclarations->GenCCode(table, isRef) +
                          variableDeclarations->GenCCode(table, isRef) +
                          subProgramDeclarations->GenCCode(table, isRef);
        ret += "int main(){\n";
        ret += compoundStatemnet->GenCCode(table, isRef);
        ret += "return 0;\n}";
        return ret;
    }

    std::string Program::GenCCode(SymbolTable &table, bool isRef)
    {
        std::string ret = "#include<stdio.h>\n" + programBody->GenCCode(table, isRef);
        return ret;
    }
}