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
        std::vector<std::unique_ptr<Range>> ranges;
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
        std::unique_ptr<Ranges> ranges;
        std::unique_ptr<BasicType> type;
        ArrayType() {}
        ArrayType(std::unique_ptr<Ranges> &ranges,
                  std::unique_ptr<BasicType> &type) : ranges(std::move(ranges)), type(std::move(type)) {}
    };

    struct ConstantDeclaration : public ASTNode
    {
        std::string name;
        std::unique_ptr<BasicType> type;
        std::string content;
        ConstantDeclaration() {}
        ConstantDeclaration(std::string &name,
                            std::unique_ptr<BasicType> &type,
                            std::string &content) : name(name), type(std::move(type)), content(content) {}
    };

    struct ConstantDeclarations : public ASTNode
    {
        std::vector<std::unique_ptr<ConstantDeclaration>> constantDeclarations;
    };

    struct VariableDeclaration : public ASTNode
    {
        std::unique_ptr<Type> type;
        std::unique_ptr<Identifiers> identifiers;
        VariableDeclaration() {}
        VariableDeclaration(std::unique_ptr<Type> &type,
                            std::unique_ptr<Identifiers> &identifiers) : type(std::move(type)), identifiers(std::move(identifiers)) {}
    };

    struct VariableDeclarations : public ASTNode
    {
        std::vector<std::unique_ptr<VariableDeclaration>> variableDeclarations;
    };

    struct Parameter : public ASTNode
    {
        bool isRef;
        std::unique_ptr<BasicType> type;
        std::unique_ptr<Identifiers> identifiers;
        Parameter() {}
        Parameter(bool isRef,
                  std::unique_ptr<BasicType> &type,
                  std::unique_ptr<Identifiers> &identifiers) : isRef(isRef), type(std::move(type)), identifiers(std::move(identifiers)) {}
    };

    struct ParameterList : public ASTNode
    {
        std::vector<std::unique_ptr<Parameter>> parameters;
    };

    struct Variable : public ASTNode
    {
        std::string name;
        std::unique_ptr<VarPart> varPart;
        Variable() {}
        Variable(std::string name,
                 std::unique_ptr<VarPart> &varPart) : name(name), varPart(std::move(varPart)) {}
    };

    struct VariableList : public ASTNode
    {
        std::vector<std::unique_ptr<Variable>> variables;
    };

    struct Factor : public ASTNode
    {
    };

    struct Expression;

    struct ExpressionFactor : public Factor
    {
        std::unique_ptr<Expression> expression;
        ExpressionFactor() {}
        ExpressionFactor(std::unique_ptr<Expression> &expression) : expression(std::move(expression)) {}
    };

    struct NumFactor : public Factor
    {
        std::string val;
    };

    struct InvFactor : public Factor
    {
        std::unique_ptr<Factor> subFactor;
        InvFactor() {}
        InvFactor(std::unique_ptr<Factor> &subFactor) : subFactor(std::move(subFactor)) {}
    };

    struct VariableFactor : public Factor
    {
        std::unique_ptr<Variable> variable;
        VariableFactor() {}
        VariableFactor(std::unique_ptr<Variable> &variable) : variable(std::move(variable)) {}
    };

    struct NotFactor : public Factor
    {
        std::unique_ptr<Factor> subFactor;
        NotFactor() {}
        NotFactor(std::unique_ptr<Factor> &subFactor) : subFactor(std::move(subFactor)) {}
    };

    struct Term;

    struct MulOpPart : public ASTNode
    {
        std::string mulOp;
        std::unique_ptr<Factor> secondFactor;
        std::unique_ptr<MulOpPart> followPart;
        MulOpPart() {}
        MulOpPart(std::string &mulOp,
                  std::unique_ptr<Factor> &secondFactor,
                  std::unique_ptr<MulOpPart> &followPart) : mulOp(mulOp), secondFactor(std::move(secondFactor)), followPart(std::move(followPart)) {}
    };

    struct Term : public ASTNode
    {
        std::unique_ptr<Factor> firstFactor;
        std::unique_ptr<MulOpPart> mulOpPart;

        Term() {}
        Term(std::unique_ptr<Factor> &firstFactor,
             std::unique_ptr<MulOpPart> &mulOpPart) : firstFactor(std::move(firstFactor)), mulOpPart(std::move(mulOpPart)) {}
    };

    struct SimpleExpression;

    struct AddOpPart : public ASTNode
    {
        std::string addOp;
        std::unique_ptr<Term> secondTerm;
        std::unique_ptr<AddOpPart> followPart;

        AddOpPart() {}
        AddOpPart(std::string &addOp,
                  std::unique_ptr<Term> &secondTerm,
                  std::unique_ptr<AddOpPart> &followPart) : addOp(addOp), secondTerm(std::move(secondTerm)), followPart(std::move(followPart)) {}
    };

    struct SimpleExpression : public ASTNode
    {
        std::unique_ptr<Term> firstTerm;
        std::unique_ptr<AddOpPart> addOpPart;

        SimpleExpression() {}
        SimpleExpression(std::unique_ptr<Term> &firstTerm,
                         std::unique_ptr<AddOpPart> &addOpPart) : firstTerm(std::move(firstTerm)), addOpPart(std::move(addOpPart)) {}
    };

    struct RelPart : public ASTNode
    {
        std::string relop;
        std::unique_ptr<SimpleExpression> secondExpression;

        RelPart() {}
        RelPart(std::string &relop,
                std::unique_ptr<SimpleExpression> &secondExpression) : relop(relop), secondExpression(std::move(secondExpression)) {}
    };

    struct Expression : public ASTNode
    {
        std::unique_ptr<SimpleExpression> firstExpression;
        std::unique_ptr<RelPart> relPart;

        Expression() {}
        Expression(std::unique_ptr<SimpleExpression> &firstExpression,
                   std::unique_ptr<RelPart> &relPart) : firstExpression(std::move(firstExpression)), relPart(std::move(relPart)) {}
    };

    struct ExpressionList : public ASTNode
    {
        std::vector<std::unique_ptr<Expression>> expressions;
    };

    struct VarPart : public ASTNode
    {
        bool isProcedureCall;
        std::unique_ptr<ExpressionList> expressionList;

        VarPart() {}
        VarPart(bool isProcedureCall,
                std::unique_ptr<ExpressionList> &expressionList) : isProcedureCall(isProcedureCall), expressionList(std::move(expressionList)) {}
    };

    struct CompoundStatement;

    struct Statement : public ASTNode
    {
    };

    struct VariableAssignStatement : public Statement
    {
        std::unique_ptr<Variable> variable;
        std::unique_ptr<Expression> expression;

        VariableAssignStatement() {}
        VariableAssignStatement(std::unique_ptr<Variable> &variable,
                                std::unique_ptr<Expression> &expression) : variable(std::move(variable)), expression(std::move(expression)) {}
    };

    struct ProcedureCallStatement : public Statement
    {
        std::unique_ptr<Variable> variable;
        ProcedureCallStatement() {}
        ProcedureCallStatement(std::unique_ptr<Variable> &variable) : variable(std::move(variable)) {}
    };

    struct SubCompoundStatement : public Statement
    {
        std::unique_ptr<CompoundStatement> compoundStatement;

        SubCompoundStatement() {}
        SubCompoundStatement(std::unique_ptr<CompoundStatement> &compoundStatement) : compoundStatement(std::move(compoundStatement)) {}
    };

    struct IfElseStatement : public Statement
    {
        std::unique_ptr<Expression> ifExpression;
        std::unique_ptr<Statement> thenStatement;
        std::unique_ptr<Statement> elseStatement;

        IfElseStatement() {}
        IfElseStatement(std::unique_ptr<Expression> &ifExpression,
                        std::unique_ptr<Statement> &thenStatement,
                        std::unique_ptr<Statement> &elseStatement)
            : ifExpression(std::move(ifExpression)), thenStatement(std::move(thenStatement)), elseStatement(std::move(elseStatement)) {}
    };

    struct ForLoopStatement : public Statement
    {
        std::string counter;
        std::unique_ptr<Expression> initExpression;
        std::unique_ptr<Expression> termiExpression;
        std::unique_ptr<Statement> loopStatement;

        ForLoopStatement() {}
        ForLoopStatement(std::string &counter,
                         std::unique_ptr<Expression> &initExpression,
                         std::unique_ptr<Expression> &termiExpression,
                         std::unique_ptr<Statement> &loopStatement) : counter(counter), initExpression(std::move(initExpression)),
                                                                      termiExpression(std::move(termiExpression)),
                                                                      loopStatement(std::move(loopStatement)) {}
    };

    struct ReadStatement : public Statement
    {
        std::unique_ptr<VariableList> variableList;
        ReadStatement() {}
        ReadStatement(std::unique_ptr<VariableList> &variableList) : variableList(std::move(variableList)) {}
    };

    struct WriteStatement : public Statement
    {
        std::unique_ptr<ExpressionList> expressionList;

        WriteStatement() {}
        WriteStatement(std::unique_ptr<ExpressionList> &expressionList) : expressionList(std::move(expressionList)) {}
    };

    struct StatementList : public ASTNode
    {
        std::vector<std::unique_ptr<Statement>> statements;
    };

    struct CompoundStatement : ASTNode
    {
        std::unique_ptr<StatementList> statementList;

        CompoundStatement() {}
        CompoundStatement(std::unique_ptr<StatementList> &statementList) : statementList(std::move(statementList)) {}
    };

    struct SubProgramHead : public ASTNode
    {
        std::string name;
        std::unique_ptr<ParameterList> parameters;
        std::unique_ptr<BasicType> returnType;

        SubProgramHead() {}
        SubProgramHead(std::string &name,
                       std::unique_ptr<ParameterList> &parameters,
                       std::unique_ptr<BasicType> &returnType) : name(name), parameters(std::move(parameters)), returnType(std::move(returnType)) {}
    };

    struct SubProgramBody : public ASTNode
    {
        std::unique_ptr<ConstantDeclarations> constantDeclarations;
        std::unique_ptr<VariableDeclarations> variableDeclarations;
        std::unique_ptr<CompoundStatement> compoundStatement;
        SubProgramBody() {}
        SubProgramBody(std::unique_ptr<ConstantDeclarations> &constantDeclarations,
                       std::unique_ptr<VariableDeclarations> &variableDeclarations,
                       std::unique_ptr<CompoundStatement> &compoundStatement)
            : constantDeclarations(std::move(constantDeclarations)),
              variableDeclarations(std::move(variableDeclarations)),
              compoundStatement(std::move(compoundStatement)) {}
    };

    struct SubProgram : public ASTNode
    {
        std::unique_ptr<SubProgramHead> head;
        std::unique_ptr<SubProgramBody> body;

        SubProgram() {}
        SubProgram(std::unique_ptr<SubProgramHead> &head,
                   std::unique_ptr<SubProgramBody> &body) : head(std::move(head)), body(std::move(body)) {}
    };

    struct SubProgramDeclarations : public ASTNode
    {
        std::vector<std::unique_ptr<SubProgram>> subPrograms;
    };

    struct ProgramHead : ASTNode
    {
        std::string name;
        std::unique_ptr<Identifiers> identifiers;
        ProgramHead() {}
        ProgramHead(std::string name, std::unique_ptr<Identifiers> &ids) : identifiers(std::move(ids)) {}
    };

    struct ProgramBody : ASTNode
    {
        std::unique_ptr<ConstantDeclarations> constantDeclarations;
        std::unique_ptr<VariableDeclarations> variableDeclarations;
        std::unique_ptr<SubProgramDeclarations> subProgramDeclarations;
        std::unique_ptr<CompoundStatement> compoundStatemnet;

        ProgramBody() {}
        ProgramBody(std::unique_ptr<ConstantDeclarations> &constantDeclarations,
                    std::unique_ptr<VariableDeclarations> &variableDeclarations,
                    std::unique_ptr<SubProgramDeclarations> &subProgramDeclarations,
                    std::unique_ptr<CompoundStatement> &compoundStatemnet) : constantDeclarations(std::move(constantDeclarations)),
                                                                             variableDeclarations(std::move(variableDeclarations)),
                                                                             subProgramDeclarations(std::move(subProgramDeclarations)),
                                                                             compoundStatemnet(std::move(compoundStatemnet)) {}
    };

    struct Program : ASTNode
    {
        std::unique_ptr<ProgramHead> programHead;
        std::unique_ptr<ProgramBody> programBody;
        Program() {}
        Program(std::unique_ptr<ProgramHead> &head, std::unique_ptr<ProgramBody> &body) : programHead(std::move(head)), programBody(std::move(body)) {}
    };

}

#endif