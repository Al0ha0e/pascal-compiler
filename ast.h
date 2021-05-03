#ifndef AST
#define AST

#include <vector>
#include <memory>

namespace PascalAST
{

    struct ASTNode
    {
    };

    struct OriASTNode : public ASTNode
    {
        std::string content;
        std::vector<std::unique_ptr<ASTNode>> SubNodes;
    };

    struct Identifiers : public ASTNode
    {
        std::vector<std::string> identifiers;
    };

    struct Range : public ASTNode
    {
        int l;
        int r;
    };

    struct Ranges : public ASTNode
    {
        std::vector<Range> ranges;
    };

    struct Type : ASTNode
    {
    };

    struct BasicType : public Type
    {
        std::string basicType;
    };

    struct ArrayType : public Type
    {
        Ranges ranges;
        BasicType type;
    };

    struct ConstantDeclaration : public ASTNode
    {
        std::string name;
        Type type;
        std::string content;
    };

    struct ConstantDeclarations : public ASTNode
    {
        std::vector<ConstantDeclaration> constantDeclarations;
    };

    struct VariableDeclaration : public ASTNode
    {
        Type type;
        Identifiers identifiers;
        std::unique_ptr<ArrayType> arrayType;
    };

    struct VariableDeclarations : public ASTNode
    {
        std::vector<VariableDeclaration> variableDeclarations;
    };

    struct Parameter : public ASTNode
    {
        bool isRef;
        Type type;
        Identifiers identifiers;
    };

    struct ParameterList : public ASTNode
    {
        std::vector<Parameter> parameters;
    };

    struct Variable : public ASTNode
    {
        std::string name;
        std::unique_ptr<VarPart> varPart;
    };

    struct VariableList : public ASTNode
    {
        std::vector<Variable> variables;
    };

    struct Factor : public ASTNode
    {
    };

    struct Expression;

    struct ExpressionFactor : public Factor
    {
        Expression expression;
    };

    struct NumFactor : public Factor
    {
        std::string val;
    };

    struct InvFactor : public Factor
    {
        std::unique_ptr<Factor> subFactor;
    };

    struct VariableFactor : public Factor
    {
        Variable variable;
    };

    struct NotFactor : public Factor
    {
        std::unique_ptr<Factor> subFactor;
    };

    struct Term;

    struct MulOpPart : public ASTNode
    {
        std::string mulOp;
        std::unique_ptr<Factor> secondFactor;
        std::unique_ptr<MulOpPart> followPart;
    };

    struct Term : public ASTNode
    {
        std::unique_ptr<Factor> firstFactor;
        std::unique_ptr<MulOpPart> mulOpPart;
    };

    struct SimpleExpression;

    struct AddOpPart : public ASTNode
    {
        std::string addOp;
        Term secondTerm;
        std::unique_ptr<AddOpPart> followPart;
    };

    struct SimpleExpression : public ASTNode
    {
        Term firstTerm;
        std::unique_ptr<AddOpPart> addOpPart;
    };

    struct RelPart : public ASTNode
    {
        std::string relop;
        SimpleExpression secondExpression;
    };

    struct Expression : public ASTNode
    {
        SimpleExpression firstExpression;
        std::unique_ptr<RelPart> relPart;
    };

    struct ExpressionList : public ASTNode
    {
        std::vector<Expression> expressions;
    };

    struct VarPart : public ASTNode
    {
        bool isProcedureCall;
        ExpressionList expressionList;
    };

    struct CompoundStatement;

    struct Statement : public ASTNode
    {
    };

    struct VariableAssignStatement : public Statement
    {
        Variable variable;
        Expression expression;
    };

    struct ProcedureCallStatement : public Statement
    {
        Variable variable;
    };

    struct SubCompoundStatement : public Statement
    {
        std::unique_ptr<CompoundStatement> compoundStatement;
    };

    struct IfElseStatement : public Statement
    {
        Expression ifExpression;
        std::unique_ptr<Statement> thenStatement;
        std::unique_ptr<Statement> elseStatement;
    };

    struct ForLoopStatement : public Statement
    {
        std::string counter;
        Expression initExpression;
        Expression termiExpression;
        std::unique_ptr<Statement> loopStatement;
    };

    struct ReadStatement : public Statement
    {
        VariableList variableList;
    };

    struct WriteStatement : public Statement
    {
        ExpressionList expressionList;
    };

    struct StatementList : public ASTNode
    {
        std::vector<std::unique_ptr<Statement>> statements;
    };

    struct CompoundStatement : ASTNode
    {
        StatementList statementList;
    };

    struct SubProgramHead : public ASTNode
    {
        std::string name;
        ParameterList parameters;
        BasicType returnType;
    };

    struct SubProgramBody : public ASTNode
    {
        ConstantDeclarations constantDeclarations;
        VariableDeclarations variableDeclarations;
        CompoundStatement compoundStatement;
    };

    struct SubProgram : public ASTNode
    {
        SubProgramHead head;
        SubProgramBody body;
    };

    struct SubProgramDeclarations : public ASTNode
    {
        std::vector<SubProgram> SubPrograms;
    };

    struct ProgramHead : ASTNode
    {
        std::string name;
        Identifiers identifiers;
    };

    struct ProgramBody : ASTNode
    {
        ConstantDeclarations constantDeclarations;
        VariableDeclarations variableDeclarations;
        SubProgramDeclarations subProgramDeclarations;
        CompoundStatement compoundStatemnet;
    };

    struct Program : ASTNode
    {
        ProgramHead programHead;
        ProgramBody programBody;
    };

}

#endif