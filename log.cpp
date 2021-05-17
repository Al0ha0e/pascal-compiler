#include "ast.h"
namespace PascalAST
{
    void OriASTNode::Show()
    {
        std::cout << "OriASTNode " << content << " " << info << std::endl;
    }

    void Identifiers::Show()
    {
        std::cout << "Identifiers ";
        for (auto &id : identifiers)
            std::cout << id << " ";
        std::cout << std::endl;
    }

    void Range::Show()
    {
        std::cout << "Range " << l << " " << r << std::endl;
    }

    void Ranges::Show()
    {
        std::cout << "Ranges" << std::endl;
        for (auto &range : ranges)
            range->Show();
    }

    void TypeDecl::Show()
    {
        std::cout << "TypeDecl" << std::endl;
    }

    void BasicTypeDecl::Show()
    {
        std::cout << "BasicTypeDecl " << basicType << std::endl;
    }

    void ArrayTypeDecl::Show()
    {
        std::cout << "ArrayTypeDecl" << std::endl;
        ranges->Show();
        type->Show();
    }

    void ConstantDeclaration::Show()
    {
        std::cout << "ConstantDeclaration " << name << " ";
        type->Show();
        std::cout << content << std::endl;
    }

    void ConstantDeclarations::Show()
    {
        std::cout << "ConstantDeclarations" << std::endl;
        for (auto &decl : constantDeclarations)
            decl->Show();
    }

    void VariableDeclaration::Show()
    {
        std::cout << "VariableDeclaration" << std::endl;
        type->Show();
        identifiers->Show();
    }

    void VariableDeclarations::Show()
    {
        std::cout << "VariableDeclarations" << std::endl;
        for (auto &decl : variableDeclarations)
            decl->Show();
    }

    void Parameter::Show()
    {
        std::cout << "Parameter " << isRef << std::endl;
        type->Show();
        identifiers->Show();
    }

    void ParameterList::Show()
    {
        std::cout << "ParameterList" << std::endl;
        for (auto &param : parameters)
            param->Show();
    }

    void Variable::Show()
    {
        std::cout << "Variable " << name << std::endl;
        if (varPart != nullptr)
            varPart->Show();
    }

    void VariableList::Show()
    {
        std::cout << "VariableList" << std::endl;
        for (auto &var : variables)
            var->Show();
    }

    void Factor::Show()
    {
        std::cout << "Factor" << std::endl;
    }

    void ExpressionFactor::Show()
    {
        std::cout << "ExpressionFactor " << std::endl;
        expression->Show();
    }

    void NumFactor::Show()
    {
        std::cout << "NumFactor " << val << " " << type << std::endl;
    }

    void InvFactor::Show()
    {
        std::cout << "InvFactor" << std::endl;
        subFactor->Show();
    }

    void VariableFactor::Show()
    {
        std::cout << "VariableFactor" << std::endl;
        variable->Show();
    }

    void NotFactor::Show()
    {
        std::cout << "NotFactor" << std::endl;
        subFactor->Show();
    }

    void MulOpPart::Show()
    {
        std::cout << "MulOpPart " << mulOp << std::endl;
        secondFactor->Show();
        if (followPart != nullptr)
            followPart->Show();
    }

    void Term::Show()
    {
        std::cout << "Term" << std::endl;
        firstFactor->Show();
        if (mulOpPart != nullptr)
            mulOpPart->Show();
    }

    void AddOpPart::Show()
    {
        std::cout << "AddOpPart " << addOp << std::endl;
        secondTerm->Show();
        if (followPart != nullptr)
            followPart->Show();
    }

    void SimpleExpression::Show()
    {
        std::cout << "SimpleExpression" << std::endl;
        firstTerm->Show();
        if (addOpPart != nullptr)
            addOpPart->Show();
    }

    void RelPart::Show()
    {
        std::cout << "RelPart " << relop << std::endl;
        secondExpression->Show();
    }

    void Expression::Show()
    {
        std::cout << "Expression" << std::endl;
        firstExpression->Show();
        if (relPart != nullptr)
            relPart->Show();
    }

    void ExpressionList::Show()
    {
        std::cout << "ExpressionList" << std::endl;
        for (auto &expression : expressions)
            expression->Show();
    }

    void VarPart::Show()
    {
        std::cout << "VarPart " << isProcedureCall << std::endl;
        expressionList->Show();
    }

    void Statement::Show()
    {
        std::cout << "Statement" << std::endl;
    }

    void VariableAssignStatement::Show()
    {
        std::cout << "VariableAssignStatement" << std::endl;
        variable->Show();
        expression->Show();
    }

    void ProcedureCallStatement::Show()
    {
        std::cout << "ProcedureCallStatement" << std::endl;
        variable->Show();
    }

    void SubCompoundStatement::Show()
    {
        std::cout << "SubCompoundStatement" << std::endl;
        compoundStatement->Show();
    }

    void IfElseStatement::Show()
    {
        std::cout << "IfElseStatement" << std::endl;
        ifExpression->Show();
        if (thenStatement != nullptr)
            thenStatement->Show();
        if (elseStatement != nullptr)
            elseStatement->Show();
    }

    void ForLoopStatement::Show()
    {
        std::cout << "ForLoopStatement " << counter << std::endl;
        initExpression->Show();
        termiExpression->Show();
        if (loopStatement != nullptr)
            loopStatement->Show();
    }

    void WhileStatement::Show()
    {
        std::cout << "WhileStatement " << std::endl;
        termiExpression->Show();
        if (loopStatement != nullptr)
            loopStatement->Show();
    }

    void ReadStatement::Show()
    {
        std::cout << "ReadStatement" << std::endl;
        variableList->Show();
    }

    void WriteStatement::Show()
    {
        std::cout << "WriteStatement" << std::endl;
        expressionList->Show();
    }

    void StatementList::Show()
    {
        std::cout << "StatementList" << std::endl;
        for (auto &statement : statements)
            if (statement != nullptr)
                statement->Show();
    }

    void CompoundStatement::Show()
    {
        std::cout << "CompoundStatement" << std::endl;
        statementList->Show();
    }

    void SubProgramHead::Show()
    {
        std::cout << "SubProgramHead " << name << std::endl;
        parameters->Show();
        if (returnType != nullptr)
            returnType->Show();
    }

    void SubProgramBody::Show()
    {
        std::cout << "SubProgramBody" << std::endl;
        constantDeclarations->Show();
        variableDeclarations->Show();
        compoundStatement->Show();
    }

    void SubProgram::Show()
    {
        std::cout << "SubProgram" << std::endl;
        head->Show();
        body->Show();
    }

    void SubProgramDeclarations::Show()
    {
        for (auto &program : subPrograms)
            program->Show();
    }

    void ProgramHead::Show()
    {
        std::cout << "ProgramHead " << name;
        identifiers->Show();
    }

    void ProgramBody::Show()
    {
        std::cout << "ProgramBody" << std::endl;
        constantDeclarations->Show();
        variableDeclarations->Show();
        subProgramDeclarations->Show();
        compoundStatemnet->Show();
    }

    void Program::Show()
    {
        std::cout << "Program" << std::endl;
        programHead->Show();
        programBody->Show();
    }

}