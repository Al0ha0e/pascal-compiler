#include "ast.h"

namespace PascalAST
{
    //TODO indentation

    inline std::string ConvCRelop(std::string s)
    {
        if (s == "=")
            return "==";
        if (s == "<>")
            return "!=";
        return s;
    }

    inline std::string ConvCAddop(std::string s)
    {
        if (s == "or")
            return "|";
        return s;
    }

    inline std::string ConvCMulop(std::string s)
    {
        if (s == "div")
            return "/";
        if (s == "and")
            return "&";
        if (s == "mod")
            return "%";
        return s;
    }

    std::string ASTNode::GenCCode(SymbolTable &table, bool isRef)
    {
        return "";
    }

    std::string Identifiers::GenCCode(SymbolTable &table, bool isRef)
    {
        std::cout << "Identifiers" << std::endl;
        std::string ret;
        for (auto &id : identifiers)
            ret += id + ",";
        if (ret.size())
            ret.pop_back();
        std::cout << "^^^^^^^^^^^^^^^^^^^^^^^^^^^^" << std::endl;
        std::cout << ret << std::endl;
        std::cout << "^^^^^^^^^^^^^^^^^^^^^^^^^^^^" << std::endl;
        return ret;
    }

    std::string BasicTypeDecl::GenCCode(SymbolTable &table, bool isRef)
    {
        std::cout << "BasicTypeDecl" << std::endl;
        if (basicType == "real")
            return "float";
        if (basicType == "boolean")
            return "int";
        if (basicType == "integer")
            return "int";
        return basicType;
    }

    std::string ConstantDeclaration::GenCCode(SymbolTable &table, bool isRef)
    {
        std::cout << "ConstantDeclaration" << std::endl;
        std::string ret =
            "const " + type->GenCCode(table, isRef) + " " + name + " = " + content + ";\n";
        return ret;
    }

    std::string ConstantDeclarations::GenCCode(SymbolTable &table, bool isRef)
    {
        std::cout << "ConstantDeclarations" << std::endl;
        std::string ret;
        for (auto &decl : constantDeclarations)
            ret += decl->GenCCode(table, isRef);
        std::cout << "^^^^^^^^^^^^^^^^^^^^^^^^^^^^" << std::endl;
        std::cout << ret << std::endl;
        std::cout << "^^^^^^^^^^^^^^^^^^^^^^^^^^^^" << std::endl;
        return ret;
    }

    std::string VariableDeclaration::GenCCode(SymbolTable &table, bool isRef)
    {
        std::cout << "VariableDeclaration" << std::endl;
        std::string ret;
        if (typeid(*(type.get())) == typeid(BasicTypeDecl))
        {
            std::cout << "???" << std::endl;
            ret = type->GenCCode(table, isRef);
            ret += " " + identifiers->GenCCode(table, isRef) + ";\n";
        }
        else if (typeid(*(type.get())) == typeid(ArrayTypeDecl))
        {
            ArrayTypeDecl *arrType = (ArrayTypeDecl *)type.get();
            ret = arrType->type->GenCCode(table, isRef) + " ";
            for (auto &id : identifiers->identifiers)
            {
                ret += id;
                for (auto &range : arrType->ranges->ranges)
                {
                    ret += "[";
                    ret += std::to_string(range->r - range->l + 1) + "]";
                }
                ret += ",";
            }
            ret[ret.length() - 1] = ';';
            ret += "\n";
        }
        return ret;
    }

    std::string VariableDeclarations::GenCCode(SymbolTable &table, bool isRef)
    {
        std::cout << "VariableDeclarations" << std::endl;
        std::string ret;
        for (auto &decl : variableDeclarations)
            ret += decl->GenCCode(table, isRef);
        std::cout << "^^^^^^^^^^^^^^^^^^^^^^^^^^^^" << std::endl;
        std::cout << ret << std::endl;
        std::cout << "^^^^^^^^^^^^^^^^^^^^^^^^^^^^" << std::endl;
        return ret;
    }

    std::string Parameter::GenCCode(SymbolTable &table, bool isRef)
    {
        std::cout << "Parameter" << std::endl;
        std::string ret;
        for (auto &id : identifiers->identifiers)
        {
            ret += type->GenCCode(table, isRef) + " ";
            ret += this->isRef ? "*" : "";
            ret += id + ",";
        }
        return ret;
    }

    std::string ParameterList::GenCCode(SymbolTable &table, bool isRef)
    {
        std::cout << "ParameterList" << std::endl;
        std::string ret;
        for (auto &param : parameters)
            ret += param->GenCCode(table, isRef);
        if (ret.size())
            ret.pop_back();
        std::cout << "^^^^^^^^^^^^^^^^^^^^^^^^^^^^" << std::endl;
        std::cout << ret << std::endl;
        std::cout << "^^^^^^^^^^^^^^^^^^^^^^^^^^^^" << std::endl;
        return ret;
    }

    std::string Variable::GenCCode(SymbolTable &table, bool isRef)
    {
        std::cout << "Variable" << std::endl;
        std::string ret;
        bool has;
        int layer;
        auto &type = table.FindSymbol(name, has, layer)->second.type;
        auto targetType(((WrapperType *)type.get())->DeWrap());
        if (varPart != nullptr)
        {
            ret = name + varPart->GenCCode(table, isRef);
            if ((!varPart->isProcedureCall) && isRef)
                ret = std::string("(&(") + ret + "))";
        }
        else
        {
            if (targetType->GetTypeId() == FUNC)
            {
                if (isAssignLeft && table.SymbolAtTop(name))
                {
                    ret = name + "_ret";
                }
                else
                {
                    ret = name + "()";
                }
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
        std::cout << "VariableList" << std::endl;
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
        std::cout << "^^^^^^^^^^^^^^^^^^^^^^^^^^^^" << std::endl;
        std::cout << ret << std::endl;
        std::cout << "^^^^^^^^^^^^^^^^^^^^^^^^^^^^" << std::endl;
        return ret;
    }

    std::string ExpressionFactor::GenCCode(SymbolTable &table, bool isRef)
    {
        std::cout << "ExpressionFactor" << std::endl;
        std::string ret = "(";
        return ret + expression->GenCCode(table, isRef) + ")";
    }

    std::string NumFactor::GenCCode(SymbolTable &table, bool isRef)
    {
        std::cout << "NumFactor" << std::endl;
        return val;
    }

    std::string InvFactor::GenCCode(SymbolTable &table, bool isRef)
    {
        std::cout << "InvFactor" << std::endl;
        std::string ret = "-(";
        return ret + subFactor->GenCCode(table, false) + ")";
    }

    std::string VariableFactor::GenCCode(SymbolTable &table, bool isRef)
    {
        std::cout << "VariableFactor" << std::endl;
        return variable->GenCCode(table, isRef);
    }

    std::string NotFactor::GenCCode(SymbolTable &table, bool isRef)
    {
        std::cout << "NotFactor" << std::endl;
        std::string ret = "!(";
        return ret + subFactor->GenCCode(table, false) + ")";
    }

    std::string MulOpPart::GenCCode(SymbolTable &table, bool isRef)
    {
        std::cout << "MulOpPart" << std::endl;
        std::string ret = std::string(" ") + ConvCMulop(mulOp) + " ";
        ret += secondFactor->GenCCode(table, (followPart == nullptr) & isRef);

        if (followPart != nullptr)
            ret += followPart->GenCCode(table, false);
        return ret;
    }

    std::string Term::GenCCode(SymbolTable &table, bool isRef)
    {
        std::cout << "Term" << std::endl;
        std::string ret;
        ret = firstFactor->GenCCode(table, (mulOpPart == nullptr) & isRef);
        if (mulOpPart != nullptr)
            ret += mulOpPart->GenCCode(table, false);
        return ret;
    }

    std::string AddOpPart::GenCCode(SymbolTable &table, bool isRef)
    {
        std::cout << "AddOpPart" << std::endl;
        std::string ret = std::string(" ") + ConvCAddop(addOp) + " ";
        ret += secondTerm->GenCCode(table, (followPart == nullptr) & isRef);
        if (followPart != nullptr)
            ret += followPart->GenCCode(table, false);
        return ret;
    }

    std::string SimpleExpression::GenCCode(SymbolTable &table, bool isRef)
    {
        std::cout << "SimpleExpression" << std::endl;
        std::string ret;
        ret = firstTerm->GenCCode(table, (addOpPart == nullptr) & isRef);
        if (addOpPart != nullptr)
            ret += addOpPart->GenCCode(table, false);
        return ret;
    }

    std::string RelPart::GenCCode(SymbolTable &table, bool isRef)
    {
        std::cout << "RelPart" << std::endl;
        std::string ret = ConvCRelop(relop);
        return ret + secondExpression->GenCCode(table, false);
    }

    std::string Expression::GenCCode(SymbolTable &table, bool isRef)
    {
        std::cout << "Expression" << std::endl;
        std::string ret = firstExpression->GenCCode(table, (relPart == nullptr) & isRef);
        if (relPart != nullptr)
            ret += relPart->GenCCode(table, false);
        return ret;
    }

    std::string ExpressionList::GenCCode(SymbolTable &table, bool isRef)
    {
        std::cout << "ExpressionList" << std::endl;
        std::string ret;
        for (auto &expression : expressions)
            ret += expression->GenCCode(table, isRef) + ",";
        if (ret.size())
            ret.pop_back();
        std::cout << "^^^^^^^^^^^^^^^^^^^^^^^^^^^^" << std::endl;
        std::cout << ret << std::endl;
        std::cout << "^^^^^^^^^^^^^^^^^^^^^^^^^^^^" << std::endl;
        return ret;
    }

    std::string VarPart::GenCCode(SymbolTable &table, bool isRef)
    {
        std::cout << "VarPart" << std::endl;
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
            return ret;
        }
        for (int i = 0; i < indexOffset.size(); i++)
        {
            ret += "[(";
            ret += expressionList->expressions[i]->GenCCode(table, false);
            ret += std::string(") - (") + std::to_string(indexOffset[i]) + ")]";
        }
        return ret;
    }

    std::string VariableAssignStatement::GenCCode(SymbolTable &table, bool isRef)
    {
        std::cout << "VariableAssignStatement" << std::endl;
        return variable->GenCCode(table, false) + " = " + expression->GenCCode(table, false) + ";\n";
    }

    std::string ProcedureCallStatement::GenCCode(SymbolTable &table, bool isRef)
    {
        std::cout << "ProcedureCallStatement" << std::endl;
        return variable->GenCCode(table, false) + ";\n";
    }

    std::string SubCompoundStatement::GenCCode(SymbolTable &table, bool isRef)
    {
        std::cout << "SubCompoundStatement" << std::endl;
        std::string ret = "{\n";
        return ret + compoundStatement->GenCCode(table, isRef) + "}\n";
    }

    std::string IfElseStatement::GenCCode(SymbolTable &table, bool isRef)
    {
        std::cout << "IfElseStatement" << std::endl;
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
        return ret;
    }

    std::string ForLoopStatement::GenCCode(SymbolTable &table, bool isRef)
    {
        std::cout << "ForLoopStatement" << std::endl;
        std::string ret = "for(";
        bool has;
        int layer;
        auto &type = table.FindSymbol(counter, has, layer)->second.type;
        std::string counterStr = type->GetTypeId() == REF
                                     ? std::string("(*") + counter + ")"
                                     : counter;
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
        std::cout << "ReadStatement" << std::endl;
        std::string ret = "scanf(";
        ret += variableList->GenCCode(table, true);
        ret += ");\n";
        return ret;
    }

    std::string WriteStatement::GenCCode(SymbolTable &table, bool isRef)
    {
        std::cout << "WriteStatement" << std::endl;
        std::string ret = "printf(";
        ret += typeStr + "," + expressionList->GenCCode(table, false) + ");\n";
        return ret;
    }

    std::string StatementList::GenCCode(SymbolTable &table, bool isRef)
    {
        std::cout << "StatementList" << std::endl;
        std::string ret = "";
        for (auto &statement : statements)
        {
            if (statement != nullptr)
                ret += statement->GenCCode(table, isRef);
        }
        std::cout << "^^^^^^^^^^^^^^^^^^^^^^^^^^^^" << std::endl;
        std::cout << ret << std::endl;
        std::cout << "^^^^^^^^^^^^^^^^^^^^^^^^^^^^" << std::endl;
        return ret;
    }

    std::string CompoundStatement::GenCCode(SymbolTable &table, bool isRef)
    {
        std::cout << "CompoundStatement" << std::endl;
        return statementList->GenCCode(table, isRef);
    }

    std::string SubProgramHead::GenCCode(SymbolTable &table, bool isRef)
    {
        std::cout << "SubProgramHead" << std::endl;
        std::string ret = returnType == nullptr ? "void " : (returnType->GenCCode(table, isRef) + " ");
        ret += name + "(";
        ret += parameters->GenCCode(table, isRef) + ")\n";
        std::cout << "^^^^^^^^^^^^^^^^^^^^^^^^^^^^" << std::endl;
        std::cout << ret << std::endl;
        std::cout << "^^^^^^^^^^^^^^^^^^^^^^^^^^^^" << std::endl;
        return ret;
    }

    std::string SubProgramBody::GenCCode(SymbolTable &table, bool isRef)
    {
        std::cout << "SubProgramBody" << std::endl;
        std::string ret = constantDeclarations->GenCCode(table, isRef);
        ret += variableDeclarations->GenCCode(table, isRef);
        ret += compoundStatement->GenCCode(table, isRef);
        return ret;
    }

    std::string SubProgram::GenCCode(SymbolTable &table, bool isRef)
    {
        std::cout << "SubProgram" << std::endl;
        table.Step();
        std::string ret = head->GenCCode(table, isRef) + "{\n";
        std::string returnId = head->name + "_ret";
        if (head->returnType != nullptr)
            ret += head->returnType->GenCCode(table, isRef) + " " + returnId + ";\n";
        ret += body->GenCCode(table, isRef);
        if (head->returnType != nullptr)
            ret += std::string("return ") + returnId + ";\n";
        ret += "}\n";
        table.PopMap();
        return ret;
    }

    std::string SubProgramDeclarations::GenCCode(SymbolTable &table, bool isRef)
    {
        std::cout << "SubProgramDeclarations" << std::endl;
        std::string ret = "";
        for (auto &program : subPrograms)
            ret += program->GenCCode(table, isRef);
        std::cout << "^^^^^^^^^^^^^^^^^^^^^^^^^^^^" << std::endl;
        std::cout << ret << std::endl;
        std::cout << "^^^^^^^^^^^^^^^^^^^^^^^^^^^^" << std::endl;
        return ret;
    }

    std::string ProgramBody::GenCCode(SymbolTable &table, bool isRef)
    {
        std::cout << "ProgramBody " << std::endl;
        std::string ret = constantDeclarations->GenCCode(table, isRef);
        ret += variableDeclarations->GenCCode(table, isRef);
        ret += subProgramDeclarations->GenCCode(table, isRef);
        ret += "int main(){\n";
        ret += compoundStatemnet->GenCCode(table, isRef);
        ret += "return 0;\n}";
        return ret;
    }

    std::string Program::GenCCode(SymbolTable &table, bool isRef)
    {
        std::cout << "Program" << std::endl;
        table.Step();
        std::string ret = "#include<stdio.h>\n" + programBody->GenCCode(table, isRef);
        table.PopMap();
        return ret;
    }
}