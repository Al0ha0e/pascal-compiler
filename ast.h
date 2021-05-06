#ifndef AST
#define AST

#include <vector>
#include <stack>
#include <memory>
#include "lexer.h"
#include "types.h"
#include "syntax.h"

namespace PascalAST
{

    struct ASTNode
    {
        virtual std::unique_ptr<TypeInfo> Check(SymbolTable &table) = 0;
    };

    struct AbstractSyntaxTree
    {
        SymbolTable symTable;
        std::unique_ptr<ASTNode> astRoot;

        void Check();
    };

    std::unique_ptr<ASTNode> GenOriAstNode(CompilerFront::Token &token);

    std::unique_ptr<ASTNode> GenAstNode(std::string expressionLeft, std::string expressionFirst, std::vector<std::unique_ptr<ASTNode>> &subNodes);

    template <typename T>
    inline std::unique_ptr<T> Unpack(std::unique_ptr<ASTNode> &node)
    {
        T *retPtr = (T *)node.release();
        return std::unique_ptr<T>(retPtr);
    }

    template <typename T>
    inline std::unique_ptr<ASTNode> Pack(std::unique_ptr<T> &node)
    {
        ASTNode *retPtr = (ASTNode *)node.release();
        return std::unique_ptr<ASTNode>(retPtr);
    }

    struct OriASTNode : public ASTNode
    {
        std::string content;
        std::string info;
        OriASTNode() {}
        OriASTNode(std::string content, std::string info) : content(content), info(info) {}
        std::unique_ptr<TypeInfo> Check(SymbolTable &table);
    };

    struct Identifiers : public ASTNode
    {
        std::vector<std::string> identifiers;
        std::unique_ptr<TypeInfo> Check(SymbolTable &table);
    };

    struct Range : public ASTNode
    {
        int l;
        int r;
        std::unique_ptr<TypeInfo> Check(SymbolTable &table);
    };

    struct Ranges : public ASTNode
    {
        std::vector<std::unique_ptr<Range>> ranges;
        std::unique_ptr<TypeInfo> Check(SymbolTable &table);
    };

    struct TypeDecl : ASTNode
    {
        std::unique_ptr<TypeInfo> Check(SymbolTable &table);
    };

    struct BasicTypeDecl : public TypeDecl
    {
        std::string basicType;
        std::unique_ptr<TypeInfo> Check(SymbolTable &table);
    };

    struct ArrayTypeDecl : public TypeDecl
    {
        std::unique_ptr<Ranges> ranges;
        std::unique_ptr<BasicTypeDecl> type;
        ArrayTypeDecl() {}
        ArrayTypeDecl(std::unique_ptr<Ranges> &&ranges,
                      std::unique_ptr<BasicTypeDecl> &&type) : ranges(std::move(ranges)), type(std::move(type)) {}
        std::unique_ptr<TypeInfo> Check(SymbolTable &table);
    };

    struct ConstantDeclaration : public ASTNode
    {
        std::string name;
        std::unique_ptr<BasicTypeDecl> type;
        std::string content;
        ConstantDeclaration() {}
        ConstantDeclaration(std::string name,
                            std::unique_ptr<BasicTypeDecl> &&type,
                            std::string content) : name(name), type(std::move(type)), content(content) {}
        std::unique_ptr<TypeInfo> Check(SymbolTable &table);
    };

    struct ConstantDeclarations : public ASTNode
    {
        std::vector<std::unique_ptr<ConstantDeclaration>> constantDeclarations;
        std::unique_ptr<TypeInfo> Check(SymbolTable &table);
    };

    struct VariableDeclaration : public ASTNode
    {
        std::unique_ptr<TypeDecl> type;
        std::unique_ptr<Identifiers> identifiers;
        VariableDeclaration() {}
        VariableDeclaration(std::unique_ptr<TypeDecl> &&type,
                            std::unique_ptr<Identifiers> &&identifiers) : type(std::move(type)), identifiers(std::move(identifiers)) {}
        std::unique_ptr<TypeInfo> Check(SymbolTable &table);
    };

    struct VariableDeclarations : public ASTNode
    {
        std::vector<std::unique_ptr<VariableDeclaration>> variableDeclarations;
        std::unique_ptr<TypeInfo> Check(SymbolTable &table);
    };

    struct Parameter : public ASTNode
    {
        bool isRef;
        std::unique_ptr<BasicTypeDecl> type;
        std::unique_ptr<Identifiers> identifiers;
        Parameter() {}
        Parameter(bool isRef,
                  std::unique_ptr<BasicTypeDecl> &&type,
                  std::unique_ptr<Identifiers> &&identifiers) : isRef(isRef), type(std::move(type)), identifiers(std::move(identifiers)) {}
        std::unique_ptr<TypeInfo> Check(SymbolTable &table);
    };

    struct ParameterList : public ASTNode
    {
        std::vector<std::unique_ptr<Parameter>> parameters;
        std::unique_ptr<TypeInfo> Check(SymbolTable &table);
    };

    struct VarPart;
    struct Variable : public ASTNode
    {
        std::string name;
        std::unique_ptr<VarPart> varPart;
        Variable() {}
        Variable(std::string name,
                 std::unique_ptr<VarPart> &&varPart) : name(name), varPart(std::move(varPart)) {}
        std::unique_ptr<TypeInfo> Check(SymbolTable &table);
    };

    struct VariableList : public ASTNode
    {
        std::vector<std::unique_ptr<Variable>> variables;
        void Check();
    };

    struct Factor : public ASTNode
    {
        void Check();
    };

    struct Expression;

    struct ExpressionFactor : public Factor
    {
        std::unique_ptr<Expression> expression;
        ExpressionFactor() {}
        ExpressionFactor(std::unique_ptr<Expression> &&expression) : expression(std::move(expression)) {}
        void Check();
    };

    struct NumFactor : public Factor
    {
        std::string val;
        std::string type;
        void Check();
    };

    struct InvFactor : public Factor
    {
        std::unique_ptr<Factor> subFactor;
        InvFactor() {}
        InvFactor(std::unique_ptr<Factor> &&subFactor) : subFactor(std::move(subFactor)) {}
        void Check();
    };

    struct VariableFactor : public Factor
    {
        std::unique_ptr<Variable> variable;
        VariableFactor() {}
        VariableFactor(std::unique_ptr<Variable> &&variable) : variable(std::move(variable)) {}
        void Check();
    };

    struct NotFactor : public Factor
    {
        std::unique_ptr<Factor> subFactor;
        NotFactor() {}
        NotFactor(std::unique_ptr<Factor> &&subFactor) : subFactor(std::move(subFactor)) {}
        void Check();
    };

    struct Term;

    struct MulOpPart : public ASTNode
    {
        std::string mulOp;
        std::unique_ptr<Factor> secondFactor;
        std::unique_ptr<MulOpPart> followPart;
        MulOpPart() {}
        MulOpPart(std::string mulOp,
                  std::unique_ptr<Factor> &&secondFactor,
                  std::unique_ptr<MulOpPart> &&followPart) : mulOp(mulOp), secondFactor(std::move(secondFactor)), followPart(std::move(followPart)) {}
        void Check();
    };

    struct Term : public ASTNode
    {
        std::unique_ptr<Factor> firstFactor;
        std::unique_ptr<MulOpPart> mulOpPart;

        Term() {}
        Term(std::unique_ptr<Factor> &&firstFactor,
             std::unique_ptr<MulOpPart> &&mulOpPart) : firstFactor(std::move(firstFactor)), mulOpPart(std::move(mulOpPart)) {}
        void Check();
    };

    struct SimpleExpression;

    struct AddOpPart : public ASTNode
    {
        std::string addOp;
        std::unique_ptr<Term> secondTerm;
        std::unique_ptr<AddOpPart> followPart;

        AddOpPart() {}
        AddOpPart(std::string addOp,
                  std::unique_ptr<Term> &&secondTerm,
                  std::unique_ptr<AddOpPart> &&followPart) : addOp(addOp), secondTerm(std::move(secondTerm)), followPart(std::move(followPart)) {}
        void Check();
    };

    struct SimpleExpression : public ASTNode
    {
        std::unique_ptr<Term> firstTerm;
        std::unique_ptr<AddOpPart> addOpPart;

        SimpleExpression() {}
        SimpleExpression(std::unique_ptr<Term> &&firstTerm,
                         std::unique_ptr<AddOpPart> &&addOpPart) : firstTerm(std::move(firstTerm)), addOpPart(std::move(addOpPart)) {}
        void Check();
    };

    struct RelPart : public ASTNode
    {
        std::string relop;
        std::unique_ptr<SimpleExpression> secondExpression;

        RelPart() {}
        RelPart(std::string relop,
                std::unique_ptr<SimpleExpression> &&secondExpression) : relop(relop), secondExpression(std::move(secondExpression)) {}
        void Check();
    };

    struct Expression : public ASTNode
    {
        std::unique_ptr<SimpleExpression> firstExpression;
        std::unique_ptr<RelPart> relPart;

        Expression() {}
        Expression(std::unique_ptr<SimpleExpression> &&firstExpression,
                   std::unique_ptr<RelPart> &&relPart) : firstExpression(std::move(firstExpression)), relPart(std::move(relPart)) {}
        void Check();
    };

    struct ExpressionList : public ASTNode
    {
        std::vector<std::unique_ptr<Expression>> expressions;
        void Check();
    };

    struct VarPart : public ASTNode
    {
        bool isProcedureCall;
        std::unique_ptr<ExpressionList> expressionList;

        VarPart() {}
        VarPart(bool isProcedureCall,
                std::unique_ptr<ExpressionList> &&expressionList) : isProcedureCall(isProcedureCall), expressionList(std::move(expressionList)) {}
        void Check();
    };

    struct CompoundStatement;

    struct Statement : public ASTNode
    {
        void Check();
    };

    struct VariableAssignStatement : public Statement
    {
        std::unique_ptr<Variable> variable;
        std::unique_ptr<Expression> expression;

        VariableAssignStatement() {}
        VariableAssignStatement(std::unique_ptr<Variable> &&variable,
                                std::unique_ptr<Expression> &&expression) : variable(std::move(variable)), expression(std::move(expression)) {}
        void Check();
    };

    struct ProcedureCallStatement : public Statement
    {
        std::unique_ptr<Variable> variable;
        ProcedureCallStatement() {}
        ProcedureCallStatement(std::unique_ptr<Variable> &&variable) : variable(std::move(variable)) {}
        void Check();
    };

    struct SubCompoundStatement : public Statement
    {
        std::unique_ptr<CompoundStatement> compoundStatement;

        SubCompoundStatement() {}
        SubCompoundStatement(std::unique_ptr<CompoundStatement> &&compoundStatement) : compoundStatement(std::move(compoundStatement)) {}
        void Check();
    };

    struct IfElseStatement : public Statement
    {
        std::unique_ptr<Expression> ifExpression;
        std::unique_ptr<Statement> thenStatement;
        std::unique_ptr<Statement> elseStatement;

        IfElseStatement() {}
        IfElseStatement(std::unique_ptr<Expression> &&ifExpression,
                        std::unique_ptr<Statement> &&thenStatement,
                        std::unique_ptr<Statement> &&elseStatement)
            : ifExpression(std::move(ifExpression)), thenStatement(std::move(thenStatement)), elseStatement(std::move(elseStatement)) {}
        void Check();
    };

    struct ForLoopStatement : public Statement
    {
        std::string counter;
        std::unique_ptr<Expression> initExpression;
        std::unique_ptr<Expression> termiExpression;
        std::unique_ptr<Statement> loopStatement;

        ForLoopStatement() {}
        ForLoopStatement(std::string counter,
                         std::unique_ptr<Expression> &&initExpression,
                         std::unique_ptr<Expression> &&termiExpression,
                         std::unique_ptr<Statement> &&loopStatement) : counter(counter), initExpression(std::move(initExpression)),
                                                                       termiExpression(std::move(termiExpression)),
                                                                       loopStatement(std::move(loopStatement)) {}
        void Check();
    };

    struct ReadStatement : public Statement
    {
        std::unique_ptr<VariableList> variableList;
        ReadStatement() {}
        ReadStatement(std::unique_ptr<VariableList> &&variableList) : variableList(std::move(variableList)) {}
        void Check();
    };

    struct WriteStatement : public Statement
    {
        std::unique_ptr<ExpressionList> expressionList;

        WriteStatement() {}
        WriteStatement(std::unique_ptr<ExpressionList> &&expressionList) : expressionList(std::move(expressionList)) {}
        void Check();
    };

    struct StatementList : public ASTNode
    {
        std::vector<std::unique_ptr<Statement>> statements;
        void Check();
    };

    struct CompoundStatement : ASTNode
    {
        std::unique_ptr<StatementList> statementList;

        CompoundStatement() {}
        CompoundStatement(std::unique_ptr<StatementList> &&statementList) : statementList(std::move(statementList)) {}
        void Check();
    };

    struct SubProgramHead : public ASTNode
    {
        std::string name;
        std::unique_ptr<ParameterList> parameters;
        std::unique_ptr<BasicTypeDecl> returnType;

        SubProgramHead() {}
        SubProgramHead(std::string name,
                       std::unique_ptr<ParameterList> &&parameters,
                       std::unique_ptr<BasicTypeDecl> &&returnType) : name(name), parameters(std::move(parameters)), returnType(std::move(returnType)) {}
        void Check();
    };

    struct SubProgramBody : public ASTNode
    {
        std::unique_ptr<ConstantDeclarations> constantDeclarations;
        std::unique_ptr<VariableDeclarations> variableDeclarations;
        std::unique_ptr<CompoundStatement> compoundStatement;
        SubProgramBody() {}
        SubProgramBody(std::unique_ptr<ConstantDeclarations> &&constantDeclarations,
                       std::unique_ptr<VariableDeclarations> &&variableDeclarations,
                       std::unique_ptr<CompoundStatement> &&compoundStatement)
            : constantDeclarations(std::move(constantDeclarations)),
              variableDeclarations(std::move(variableDeclarations)),
              compoundStatement(std::move(compoundStatement)) {}
        void Check();
    };

    struct SubProgram : public ASTNode
    {
        std::unique_ptr<SubProgramHead> head;
        std::unique_ptr<SubProgramBody> body;

        SubProgram() {}
        SubProgram(std::unique_ptr<SubProgramHead> &&head,
                   std::unique_ptr<SubProgramBody> &&body) : head(std::move(head)), body(std::move(body)) {}
        void Check();
    };

    struct SubProgramDeclarations : public ASTNode
    {
        std::vector<std::unique_ptr<SubProgram>> subPrograms;
        void Check();
    };

    struct ProgramHead : ASTNode
    {
        std::string name;
        std::unique_ptr<Identifiers> identifiers;
        ProgramHead() {}
        ProgramHead(std::string name, std::unique_ptr<Identifiers> &&ids) : identifiers(std::move(ids)) {}
        void Check();
    };

    struct ProgramBody : ASTNode
    {
        std::unique_ptr<ConstantDeclarations> constantDeclarations;
        std::unique_ptr<VariableDeclarations> variableDeclarations;
        std::unique_ptr<SubProgramDeclarations> subProgramDeclarations;
        std::unique_ptr<CompoundStatement> compoundStatemnet;

        ProgramBody() {}
        ProgramBody(std::unique_ptr<ConstantDeclarations> &&constantDeclarations,
                    std::unique_ptr<VariableDeclarations> &&variableDeclarations,
                    std::unique_ptr<SubProgramDeclarations> &&subProgramDeclarations,
                    std::unique_ptr<CompoundStatement> &&compoundStatemnet) : constantDeclarations(std::move(constantDeclarations)),
                                                                              variableDeclarations(std::move(variableDeclarations)),
                                                                              subProgramDeclarations(std::move(subProgramDeclarations)),
                                                                              compoundStatemnet(std::move(compoundStatemnet)) {}
        void Check();
    };

    struct Program : ASTNode
    {
        std::unique_ptr<ProgramHead> programHead;
        std::unique_ptr<ProgramBody> programBody;
        Program() {}
        Program(std::unique_ptr<ProgramHead> &&head, std::unique_ptr<ProgramBody> &&body) : programHead(std::move(head)), programBody(std::move(body)) {}
        void Check();
    };

}

#endif