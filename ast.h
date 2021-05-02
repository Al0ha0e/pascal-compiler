#ifndef AST
#define AST

#include <vector>
#include <memory>

namespace PascalAST
{
    typedef std::vector<std::string> Identifiers;

    enum Type
    {
        VOID,
        INTEGER,
        REAL,
        CHAR,
        ARRAY
    };

    struct Range
    {
        int l;
        int r;
    };

    typedef std::vector<Range> Ranges;

    struct ArrayType
    {
        Ranges ranges;
        Type type;
    };

    struct ConstantDeclaration
    {
        std::string name;
        Type type;
        std::string content;
    };

    typedef std::vector<ConstantDeclaration> ConstantDeclarations;

    struct VariableDeclaration
    {
        Type type;
        Identifiers identifiers;
        std::unique_ptr<ArrayType> arrayType;
    };

    typedef std::vector<VariableDeclaration> VariableDeclarations;

    struct Parameter
    {
        bool isRef;
        Type type;
        Identifiers identifiers;
    };

    typedef std::vector<Parameter> Parameters;

    struct Variable
    {
        std::string name;
        std::unique_ptr<VarPart> varPart;
    };

    typedef std::vector<Variable> VariableList;

    struct Factor
    {
    };

    
    struct Term;

    struct MulOpPart
    {
        std::string addOp;
        std::unique_ptr<Term> secondTerm;
    };

    struct Term
    {
        Factor firstFactor;
        std::unique_ptr<MulOpPart> mulOpPart;
    };

    struct SimpleExpression;

    struct AddOpPart
    {
        std::string addOp;
        std::unique_ptr<SimpleExpression> secondExpression;
    };

    struct SimpleExpression
    {
        Term firstTerm;
        std::unique_ptr<AddOpPart> addOpPart;
    };

    struct RelPart
    {
        std::string relop;
        SimpleExpression secondExpression;
    };

    struct Expression
    {
        SimpleExpression firstExpression;
        std::unique_ptr<RelPart> relPart;
    };

    typedef std::vector<Expression> ExpressionList;

    struct VarPart
    {
        bool isProcedureCall;
        ExpressionList expressionList;
    };

    struct CompoundStatement;

    struct Statement
    {
    };

    struct VariableAssignStatement : Statement
    {
        Variable variable;
        Expression expression;
    };

    struct ProcedureCallStatement : Statement
    {
        Variable variable;
    };

    struct SubCompoundStatement : Statement
    {
        std::unique_ptr<CompoundStatement> compoundStatement;
    };

    struct IfElseStatement : Statement
    {
        Expression ifExpression;
        Statement thenStatement;
        std::unique_ptr<Expression> elseExpression;
    };

    struct ForLoopStatement : Statement
    {
        std::string counter;
        Expression initExpression;
        Expression termiExpression;
        Statement loopStatement;
    };

    struct ReadStatement : Statement
    {
        VariableList variableList;
    };

    struct WriteStatement : Statement
    {
        ExpressionList expressionList;
    };

    typedef std::vector<Statement> StatementList;

    struct CompoundStatement
    {
        StatementList statementList;
    };

    struct SubProgramHead
    {
        std::string name;
        Parameters parameters;
        Type returnType;
    };

    struct SubProgramBody
    {
        ConstantDeclarations constantDeclarations;
        VariableDeclarations variableDeclarations;
        CompoundStatement compoundStatement;
    };

    struct SubProgram
    {
        SubProgramHead head;
    };

    typedef std::vector<SubProgram> SubPrograms;

    struct ProgramHead
    {
        std::string name;
        Identifiers identifiers;
    };

    struct ProgramBody
    {
        ConstantDeclarations constantDeclarations;
        VariableDeclarations variableDeclarations;
    };

    struct Program
    {
        ProgramHead programHead;
        ProgramBody programBody;
    };

}

#endif