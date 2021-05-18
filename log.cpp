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

    void OriASTNode::FormatShow(int level)
    {
        std::cout << std::string(2 * level, ' ');
        std::cout << "OriASTNode " << content << " " << info << std::endl;
    }

    void Identifiers::FormatShow(int level)
    {
        std::cout << std::string(2 * level, ' ');
        std::cout << "Identifiers ";
        for (auto &id : identifiers)
            std::cout << id << " ";
        std::cout << std::endl;
    }

    void Range::FormatShow(int level)
    {
        std::cout << std::string(2 * level, ' ');
        std::cout << "Range " << l << " " << r << std::endl;
    }

    void Ranges::FormatShow(int level)
    {
        std::cout << std::string(2 * level, ' ');
        std::cout << "Ranges" << std::endl;
        for (auto &range : ranges)
            range->FormatShow(level + 1);
    }

    void TypeDecl::FormatShow(int level)
    {
        std::cout << std::string(2 * level, ' ');
        std::cout << "TypeDecl" << std::endl;
    }

    void BasicTypeDecl::FormatShow(int level)
    {
        std::cout << std::string(2 * level, ' ');
        std::cout << "BasicTypeDecl " << basicType << std::endl;
    }

    void ArrayTypeDecl::FormatShow(int level)
    {
        std::cout << std::string(2 * level, ' ');
        std::cout << "ArrayTypeDecl" << std::endl;
        ranges->FormatShow(level + 1);
        type->FormatShow(level + 1);
    }

    void ConstantDeclaration::FormatShow(int level)
    {
        std::cout << std::string(2 * level, ' ');
        std::cout << "ConstantDeclaration " << name << " " << content << std::endl;
        type->FormatShow(level + 1);
    }

    void ConstantDeclarations::FormatShow(int level)
    {
        std::cout << std::string(2 * level, ' ');
        std::cout << "ConstantDeclarations" << std::endl;
        for (auto &decl : constantDeclarations)
            decl->FormatShow(level + 1);
    }

    void VariableDeclaration::FormatShow(int level)
    {
        std::cout << std::string(2 * level, ' ');
        std::cout << "VariableDeclaration" << std::endl;
        type->FormatShow(level + 1);
        identifiers->FormatShow(level + 1);
    }

    void VariableDeclarations::FormatShow(int level)
    {
        std::cout << std::string(2 * level, ' ');
        std::cout << "VariableDeclarations" << std::endl;
        for (auto &decl : variableDeclarations)
            decl->FormatShow(level + 1);
    }

    void Parameter::FormatShow(int level)
    {
        std::cout << std::string(2 * level, ' ');
        std::cout << "Parameter " << isRef << std::endl;
        type->FormatShow(level + 1);
        identifiers->FormatShow(level + 1);
    }

    void ParameterList::FormatShow(int level)
    {
        std::cout << std::string(2 * level, ' ');
        std::cout << "ParameterList" << std::endl;
        for (auto &param : parameters)
            param->FormatShow(level + 1);
    }

    void Variable::FormatShow(int level)
    {
        std::cout << std::string(2 * level, ' ');
        std::cout << "Variable " << name << std::endl;
        if (varPart != nullptr)
            varPart->FormatShow(level + 1);
    }

    void VariableList::FormatShow(int level)
    {
        std::cout << std::string(2 * level, ' ');
        std::cout << "VariableList" << std::endl;
        for (auto &var : variables)
            var->FormatShow(level + 1);
    }

    void Factor::FormatShow(int level)
    {
        std::cout << std::string(2 * level, ' ');
        std::cout << "Factor" << std::endl;
    }

    void ExpressionFactor::FormatShow(int level)
    {
        std::cout << std::string(2 * level, ' ');
        std::cout << "ExpressionFactor " << std::endl;
        expression->FormatShow(level + 1);
    }

    void NumFactor::FormatShow(int level)
    {
        std::cout << std::string(2 * level, ' ');
        std::cout << "NumFactor " << val << " " << type << std::endl;
    }

    void InvFactor::FormatShow(int level)
    {
        std::cout << std::string(2 * level, ' ');
        std::cout << "InvFactor" << std::endl;
        subFactor->FormatShow(level + 1);
    }

    void VariableFactor::FormatShow(int level)
    {
        std::cout << std::string(2 * level, ' ');
        std::cout << "VariableFactor" << std::endl;
        variable->FormatShow(level + 1);
    }

    void NotFactor::FormatShow(int level)
    {
        std::cout << std::string(2 * level, ' ');
        std::cout << "NotFactor" << std::endl;
        subFactor->FormatShow(level + 1);
    }

    void MulOpPart::FormatShow(int level)
    {
        std::cout << std::string(2 * level, ' ');
        std::cout << "MulOpPart " << mulOp << std::endl;
        secondFactor->FormatShow(level + 1);
        if (followPart != nullptr)
            followPart->FormatShow(level + 1);
    }

    void Term::FormatShow(int level)
    {
        std::cout << std::string(2 * level, ' ');
        std::cout << "Term" << std::endl;
        firstFactor->FormatShow(level + 1);
        if (mulOpPart != nullptr)
            mulOpPart->FormatShow(level + 1);
    }

    void AddOpPart::FormatShow(int level)
    {
        std::cout << std::string(2 * level, ' ');
        std::cout << "AddOpPart " << addOp << std::endl;
        secondTerm->FormatShow(level + 1);
        if (followPart != nullptr)
            followPart->FormatShow(level + 1);
    }

    void SimpleExpression::FormatShow(int level)
    {
        std::cout << std::string(2 * level, ' ');
        std::cout << "SimpleExpression" << std::endl;
        firstTerm->FormatShow(level + 1);
        if (addOpPart != nullptr)
            addOpPart->FormatShow(level + 1);
    }

    void RelPart::FormatShow(int level)
    {
        std::cout << std::string(2 * level, ' ');
        std::cout << "RelPart " << relop << std::endl;
        secondExpression->FormatShow(level + 1);
    }

    void Expression::FormatShow(int level)
    {
        std::cout << std::string(2 * level, ' ');
        std::cout << "Expression" << std::endl;
        firstExpression->FormatShow(level + 1);
        if (relPart != nullptr)
            relPart->FormatShow(level + 1);
    }

    void ExpressionList::FormatShow(int level)
    {
        std::cout << std::string(2 * level, ' ');
        std::cout << "ExpressionList" << std::endl;
        for (auto &expression : expressions)
            expression->FormatShow(level + 1);
    }

    void VarPart::FormatShow(int level)
    {
        std::cout << std::string(2 * level, ' ');
        std::cout << "VarPart " << isProcedureCall << std::endl;
        expressionList->FormatShow(level + 1);
    }

    void Statement::FormatShow(int level)
    {
        std::cout << std::string(2 * level, ' ');
        std::cout << "Statement" << std::endl;
    }

    void VariableAssignStatement::FormatShow(int level)
    {
        std::cout << std::string(2 * level, ' ');
        std::cout << "VariableAssignStatement" << std::endl;
        variable->FormatShow(level + 1);
        expression->FormatShow(level + 1);
    }

    void ProcedureCallStatement::FormatShow(int level)
    {
        std::cout << std::string(2 * level, ' ');
        std::cout << "ProcedureCallStatement" << std::endl;
        variable->FormatShow(level + 1);
    }

    void SubCompoundStatement::FormatShow(int level)
    {
        std::cout << std::string(2 * level, ' ');
        std::cout << "SubCompoundStatement" << std::endl;
        compoundStatement->FormatShow(level + 1);
    }

    void IfElseStatement::FormatShow(int level)
    {
        std::cout << std::string(2 * level, ' ');
        std::cout << "IfElseStatement" << std::endl;
        ifExpression->FormatShow(level + 1);
        if (thenStatement != nullptr)
            thenStatement->FormatShow(level + 1);
        if (elseStatement != nullptr)
            elseStatement->FormatShow(level + 1);
    }

    void ForLoopStatement::FormatShow(int level)
    {
        std::cout << std::string(2 * level, ' ');
        std::cout << "ForLoopStatement " << counter << std::endl;
        initExpression->FormatShow(level + 1);
        termiExpression->FormatShow(level + 1);
        if (loopStatement != nullptr)
            loopStatement->FormatShow(level + 1);
    }

    void WhileStatement::FormatShow(int level)
    {
        std::cout << std::string(2 * level, ' ');
        std::cout << "WhileStatement " << std::endl;
        termiExpression->FormatShow(level + 1);
        if (loopStatement != nullptr)
            loopStatement->FormatShow(level + 1);
    }

    void ReadStatement::FormatShow(int level)
    {
        std::cout << std::string(2 * level, ' ');
        std::cout << "ReadStatement" << std::endl;
        variableList->FormatShow(level + 1);
    }

    void WriteStatement::FormatShow(int level)
    {
        std::cout << std::string(2 * level, ' ');
        std::cout << "WriteStatement" << std::endl;
        expressionList->FormatShow(level + 1);
    }

    void StatementList::FormatShow(int level)
    {
        std::cout << std::string(2 * level, ' ');
        std::cout << "StatementList" << std::endl;
        for (auto &statement : statements)
            if (statement != nullptr)
                statement->FormatShow(level + 1);
    }

    void CompoundStatement::FormatShow(int level)
    {
        std::cout << std::string(2 * level, ' ');
        std::cout << "CompoundStatement" << std::endl;
        statementList->FormatShow(level + 1);
    }

    void SubProgramHead::FormatShow(int level)
    {
        std::cout << std::string(2 * level, ' ');
        std::cout << "SubProgramHead " << name << std::endl;
        parameters->FormatShow(level + 1);
        if (returnType != nullptr)
            returnType->FormatShow(level + 1);
    }

    void SubProgramBody::FormatShow(int level)
    {
        std::cout << std::string(2 * level, ' ');
        std::cout << "SubProgramBody" << std::endl;
        constantDeclarations->FormatShow(level + 1);
        variableDeclarations->FormatShow(level + 1);
        compoundStatement->FormatShow(level + 1);
    }

    void SubProgram::FormatShow(int level)
    {
        std::cout << std::string(2 * level, ' ');
        std::cout << "SubProgram" << std::endl;
        head->FormatShow(level + 1);
        body->FormatShow(level + 1);
    }

    void SubProgramDeclarations::FormatShow(int level)
    {
        std::cout << std::string(2 * level, ' ');
        for (auto &program : subPrograms)
            program->FormatShow(level + 1);
    }

    void ProgramHead::FormatShow(int level)
    {
        std::cout << std::string(2 * level, ' ');
        std::cout << "ProgramHead " << name << std::endl;
        identifiers->FormatShow(level + 1);
    }

    void ProgramBody::FormatShow(int level)
    {
        std::cout << std::string(2 * level, ' ');
        std::cout << "ProgramBody" << std::endl;
        constantDeclarations->FormatShow(level + 1);
        variableDeclarations->FormatShow(level + 1);
        subProgramDeclarations->FormatShow(level + 1);
        compoundStatemnet->FormatShow(level + 1);
    }

    void Program::FormatShow(int level)
    {
        std::cout << std::string(2 * level, ' ');
        std::cout << "Program" << std::endl;
        programHead->FormatShow(level + 1);
        programBody->FormatShow(level + 1);
    }

}