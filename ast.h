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
        virtual std::unique_ptr<TypeInfo> Check(SymbolTable &table, bool &ok) = 0;
        virtual void Show() = 0;
        virtual std::string GenCCode(SymbolTable &table, bool isRef);
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
        return UniquePtrCast<T>(node);
        // T *retPtr = (T *)node.release();
        // return std::unique_ptr<T>(retPtr);
    }

    template <typename T>
    inline std::unique_ptr<ASTNode> Pack(std::unique_ptr<T> &node)
    {
        return UniquePtrCast<ASTNode>(node);
        // ASTNode *retPtr = (ASTNode *)node.release();
        // return std::unique_ptr<ASTNode>(retPtr);
    }

    struct OriASTNode : public ASTNode
    {
        std::string content;
        std::string info;
        OriASTNode() {}
        OriASTNode(std::string content, std::string info) : content(content), info(info) {}

        std::unique_ptr<TypeInfo> Check(SymbolTable &table, bool &ok);
        void Show();
    };

    struct Identifiers : public ASTNode
    {
        std::vector<std::string> identifiers;

        std::unique_ptr<TypeInfo> Check(SymbolTable &table, bool &ok);
        void Show();
        virtual std::string GenCCode(SymbolTable &table, bool isRef) override;
    };

    struct Range : public ASTNode
    {
        int l;
        int r;
        std::unique_ptr<TypeInfo> Check(SymbolTable &table, bool &ok);

        void Show();
    };

    struct Ranges : public ASTNode
    {
        std::vector<std::unique_ptr<Range>> ranges;
        std::unique_ptr<TypeInfo> Check(SymbolTable &table, bool &ok);

        void Show();
    };

    struct TypeDecl : ASTNode
    {
        std::unique_ptr<TypeInfo> Check(SymbolTable &table, bool &ok);

        void Show();
    };

    struct BasicTypeDecl : public TypeDecl
    {
        std::string basicType;
        std::unique_ptr<TypeInfo> Check(SymbolTable &table, bool &ok);

        void Show();
        virtual std::string GenCCode(SymbolTable &table, bool isRef) override;
    };

    struct ArrayTypeDecl : public TypeDecl
    {
        std::unique_ptr<Ranges> ranges;
        std::unique_ptr<BasicTypeDecl> type;
        ArrayTypeDecl() {}
        ArrayTypeDecl(std::unique_ptr<Ranges> &&ranges,
                      std::unique_ptr<BasicTypeDecl> &&type) : ranges(std::move(ranges)), type(std::move(type)) {}
        std::unique_ptr<TypeInfo> Check(SymbolTable &table, bool &ok);

        void Show();
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

        std::unique_ptr<TypeInfo> Check(SymbolTable &table, bool &ok);
        void Show();
        virtual std::string GenCCode(SymbolTable &table, bool isRef) override;
    };

    struct ConstantDeclarations : public ASTNode
    {
        std::vector<std::unique_ptr<ConstantDeclaration>> constantDeclarations;

        std::unique_ptr<TypeInfo> Check(SymbolTable &table, bool &ok);
        void Show();
        virtual std::string GenCCode(SymbolTable &table, bool isRef) override;
    };

    struct VariableDeclaration : public ASTNode
    {
        std::unique_ptr<TypeDecl> type;
        std::unique_ptr<Identifiers> identifiers;
        VariableDeclaration() {}
        VariableDeclaration(std::unique_ptr<TypeDecl> &&type,
                            std::unique_ptr<Identifiers> &&identifiers) : type(std::move(type)), identifiers(std::move(identifiers)) {}

        std::unique_ptr<TypeInfo> Check(SymbolTable &table, bool &ok);
        void Show();
        virtual std::string GenCCode(SymbolTable &table, bool isRef) override;
    };

    struct VariableDeclarations : public ASTNode
    {
        std::vector<std::unique_ptr<VariableDeclaration>> variableDeclarations;

        std::unique_ptr<TypeInfo> Check(SymbolTable &table, bool &ok);
        void Show();
        virtual std::string GenCCode(SymbolTable &table, bool isRef) override;
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

        std::unique_ptr<TypeInfo> Check(SymbolTable &table, bool &ok);
        void Show();
        virtual std::string GenCCode(SymbolTable &table, bool isRef) override;
    };

    struct ParameterList : public ASTNode
    {
        std::vector<std::unique_ptr<Parameter>> parameters;

        std::unique_ptr<TypeInfo> Check(SymbolTable &table, bool &ok);
        void Show();
        virtual std::string GenCCode(SymbolTable &table, bool isRef) override;
    };

    struct VarPart;
    struct Variable : public ASTNode
    {
        std::string name;
        std::unique_ptr<VarPart> varPart; //MAY NULL
        Variable() {}
        Variable(std::string name,
                 std::unique_ptr<VarPart> &&varPart) : name(name), varPart(std::move(varPart)) {}

        std::unique_ptr<TypeInfo> Check(SymbolTable &table, bool &ok);
        void Show();
        virtual std::string GenCCode(SymbolTable &table, bool isRef) override;
    };

    struct VariableList : public ASTNode
    {
        std::vector<std::unique_ptr<Variable>> variables;

        std::unique_ptr<TypeInfo> Check(SymbolTable &table, bool &ok);
        void Show();
        virtual std::string GenCCode(SymbolTable &table, bool isRef) override;
    };

    struct Factor : public ASTNode
    {
        std::unique_ptr<TypeInfo> Check(SymbolTable &table, bool &ok);

        void Show();
    };

    struct Expression;

    struct ExpressionFactor : public Factor
    {
        std::unique_ptr<Expression> expression;
        ExpressionFactor() {}
        ExpressionFactor(std::unique_ptr<Expression> &&expression) : expression(std::move(expression)) {}

        std::unique_ptr<TypeInfo> Check(SymbolTable &table, bool &ok);
        void Show();
        virtual std::string GenCCode(SymbolTable &table, bool isRef) override;
    };

    struct NumFactor : public Factor
    {
        std::string val;
        std::string type;

        std::unique_ptr<TypeInfo> Check(SymbolTable &table, bool &ok);
        void Show();
        virtual std::string GenCCode(SymbolTable &table, bool isRef) override;
    };

    struct InvFactor : public Factor
    {
        std::unique_ptr<Factor> subFactor;
        InvFactor() {}
        InvFactor(std::unique_ptr<Factor> &&subFactor) : subFactor(std::move(subFactor)) {}

        std::unique_ptr<TypeInfo> Check(SymbolTable &table, bool &ok);
        void Show();
        virtual std::string GenCCode(SymbolTable &table, bool isRef) override;
    };

    struct VariableFactor : public Factor
    {
        std::unique_ptr<Variable> variable;
        VariableFactor() {}
        VariableFactor(std::unique_ptr<Variable> &&variable) : variable(std::move(variable)) {}

        std::unique_ptr<TypeInfo> Check(SymbolTable &table, bool &ok);
        void Show();
        virtual std::string GenCCode(SymbolTable &table, bool isRef) override;
    };

    struct NotFactor : public Factor
    {
        std::unique_ptr<Factor> subFactor;
        NotFactor() {}
        NotFactor(std::unique_ptr<Factor> &&subFactor) : subFactor(std::move(subFactor)) {}

        std::unique_ptr<TypeInfo> Check(SymbolTable &table, bool &ok);
        void Show();
        virtual std::string GenCCode(SymbolTable &table, bool isRef) override;
    };

    struct Term;

    struct MulOpPart : public ASTNode
    {
        std::string mulOp;
        std::unique_ptr<Factor> secondFactor;
        std::unique_ptr<MulOpPart> followPart; //MAY NULL
        MulOpPart() {}
        MulOpPart(std::string mulOp,
                  std::unique_ptr<Factor> &&secondFactor,
                  std::unique_ptr<MulOpPart> &&followPart) : mulOp(mulOp), secondFactor(std::move(secondFactor)), followPart(std::move(followPart)) {}

        std::unique_ptr<TypeInfo> Check(SymbolTable &table, bool &ok);
        void Show();
        virtual std::string GenCCode(SymbolTable &table, bool isRef) override;
    };

    struct Term : public ASTNode
    {
        std::unique_ptr<Factor> firstFactor;
        std::unique_ptr<MulOpPart> mulOpPart; //MAY NULL

        Term() {}
        Term(std::unique_ptr<Factor> &&firstFactor,
             std::unique_ptr<MulOpPart> &&mulOpPart) : firstFactor(std::move(firstFactor)), mulOpPart(std::move(mulOpPart)) {}

        std::unique_ptr<TypeInfo> Check(SymbolTable &table, bool &ok);
        void Rotate();
        void Show();
        virtual std::string GenCCode(SymbolTable &table, bool isRef) override;
    };

    struct SimpleExpression;

    struct AddOpPart : public ASTNode
    {
        std::string addOp;
        std::unique_ptr<Term> secondTerm;
        std::unique_ptr<AddOpPart> followPart; //MAY NULL

        AddOpPart() {}
        AddOpPart(std::string addOp,
                  std::unique_ptr<Term> &&secondTerm,
                  std::unique_ptr<AddOpPart> &&followPart) : addOp(addOp), secondTerm(std::move(secondTerm)), followPart(std::move(followPart)) {}

        std::unique_ptr<TypeInfo> Check(SymbolTable &table, bool &ok);
        void Show();
        virtual std::string GenCCode(SymbolTable &table, bool isRef) override;
    };

    struct SimpleExpression : public ASTNode
    {
        std::unique_ptr<Term> firstTerm;
        std::unique_ptr<AddOpPart> addOpPart; //MAY NULL

        SimpleExpression() {}
        SimpleExpression(std::unique_ptr<Term> &&firstTerm,
                         std::unique_ptr<AddOpPart> &&addOpPart) : firstTerm(std::move(firstTerm)), addOpPart(std::move(addOpPart)) {}

        std::unique_ptr<TypeInfo> Check(SymbolTable &table, bool &ok);
        void Rotate();
        void Show();
        virtual std::string GenCCode(SymbolTable &table, bool isRef) override;
    };

    struct RelPart : public ASTNode
    {
        std::string relop;
        std::unique_ptr<SimpleExpression> secondExpression;

        RelPart() {}
        RelPart(std::string relop,
                std::unique_ptr<SimpleExpression> &&secondExpression) : relop(relop), secondExpression(std::move(secondExpression)) {}

        std::unique_ptr<TypeInfo> Check(SymbolTable &table, bool &ok);
        void Show();
        virtual std::string GenCCode(SymbolTable &table, bool isRef) override;
    };

    struct Expression : public ASTNode
    {
        std::unique_ptr<SimpleExpression> firstExpression;
        std::unique_ptr<RelPart> relPart; //MAY NULL

        Expression() {}
        Expression(std::unique_ptr<SimpleExpression> &&firstExpression,
                   std::unique_ptr<RelPart> &&relPart) : firstExpression(std::move(firstExpression)), relPart(std::move(relPart)) {}

        std::unique_ptr<TypeInfo> Check(SymbolTable &table, bool &ok);
        void Show();
        virtual std::string GenCCode(SymbolTable &table, bool isRef) override;
    };

    struct ExpressionList : public ASTNode
    {
        std::vector<std::unique_ptr<Expression>> expressions;

        std::unique_ptr<TypeInfo> Check(SymbolTable &table, bool &ok);
        void Show();
        virtual std::string GenCCode(SymbolTable &table, bool isRef) override;
    };

    struct VarPart : public ASTNode
    {
        bool isProcedureCall;
        std::unique_ptr<ExpressionList> expressionList;

        VarPart() {}
        VarPart(bool isProcedureCall,
                std::unique_ptr<ExpressionList> &&expressionList) : isProcedureCall(isProcedureCall), expressionList(std::move(expressionList)) {}

        std::unique_ptr<TypeInfo> Check(SymbolTable &table, bool &ok);
        void Show();
        virtual std::string GenCCode(SymbolTable &table, bool isRef) override;
    };

    struct CompoundStatement;

    struct Statement : public ASTNode
    {
        std::unique_ptr<TypeInfo> Check(SymbolTable &table, bool &ok);
        void Show();
    };

    struct VariableAssignStatement : public Statement
    {
        std::unique_ptr<Variable> variable;
        std::unique_ptr<Expression> expression;

        VariableAssignStatement() {}
        VariableAssignStatement(std::unique_ptr<Variable> &&variable,
                                std::unique_ptr<Expression> &&expression) : variable(std::move(variable)), expression(std::move(expression)) {}

        std::unique_ptr<TypeInfo> Check(SymbolTable &table, bool &ok);
        void Show();
        virtual std::string GenCCode(SymbolTable &table, bool isRef) override;
    };

    struct ProcedureCallStatement : public Statement
    {
        std::unique_ptr<Variable> variable;
        ProcedureCallStatement() {}
        ProcedureCallStatement(std::unique_ptr<Variable> &&variable) : variable(std::move(variable)) {}

        std::unique_ptr<TypeInfo> Check(SymbolTable &table, bool &ok);
        void Show();
        virtual std::string GenCCode(SymbolTable &table, bool isRef) override;
    };

    struct SubCompoundStatement : public Statement
    {
        std::unique_ptr<CompoundStatement> compoundStatement;

        SubCompoundStatement() {}
        SubCompoundStatement(std::unique_ptr<CompoundStatement> &&compoundStatement) : compoundStatement(std::move(compoundStatement)) {}

        std::unique_ptr<TypeInfo> Check(SymbolTable &table, bool &ok);
        void Show();
        virtual std::string GenCCode(SymbolTable &table, bool isRef) override;
    };

    struct IfElseStatement : public Statement
    {
        std::unique_ptr<Expression> ifExpression;
        std::unique_ptr<Statement> thenStatement; //MAY NULL
        std::unique_ptr<Statement> elseStatement; //MAY NULL

        IfElseStatement() {}
        IfElseStatement(std::unique_ptr<Expression> &&ifExpression,
                        std::unique_ptr<Statement> &&thenStatement,
                        std::unique_ptr<Statement> &&elseStatement)
            : ifExpression(std::move(ifExpression)), thenStatement(std::move(thenStatement)), elseStatement(std::move(elseStatement)) {}

        std::unique_ptr<TypeInfo> Check(SymbolTable &table, bool &ok);
        void Show();
        virtual std::string GenCCode(SymbolTable &table, bool isRef) override;
    };

    struct ForLoopStatement : public Statement
    {
        std::string counter;
        std::unique_ptr<Expression> initExpression;
        std::unique_ptr<Expression> termiExpression;
        std::unique_ptr<Statement> loopStatement; //MAY NULL

        ForLoopStatement() {}
        ForLoopStatement(std::string counter,
                         std::unique_ptr<Expression> &&initExpression,
                         std::unique_ptr<Expression> &&termiExpression,
                         std::unique_ptr<Statement> &&loopStatement) : counter(counter), initExpression(std::move(initExpression)),
                                                                       termiExpression(std::move(termiExpression)),
                                                                       loopStatement(std::move(loopStatement)) {}
        std::unique_ptr<TypeInfo> Check(SymbolTable &table, bool &ok);
        void Show();
        virtual std::string GenCCode(SymbolTable &table, bool isRef) override;
    };

    struct ReadStatement : public Statement
    {
        std::unique_ptr<VariableList> variableList;
        ReadStatement() {}
        ReadStatement(std::unique_ptr<VariableList> &&variableList) : variableList(std::move(variableList)) {}

        std::unique_ptr<TypeInfo> Check(SymbolTable &table, bool &ok);
        void Show();
        virtual std::string GenCCode(SymbolTable &table, bool isRef) override;
    };

    struct WriteStatement : public Statement
    {
        std::unique_ptr<ExpressionList> expressionList;
        std::string typeStr;

        WriteStatement() {}
        WriteStatement(std::unique_ptr<ExpressionList> &&expressionList) : expressionList(std::move(expressionList)) {}

        std::unique_ptr<TypeInfo> Check(SymbolTable &table, bool &ok);
        void Show();
        virtual std::string GenCCode(SymbolTable &table, bool isRef) override;
    };

    struct StatementList : public ASTNode
    {
        std::vector<std::unique_ptr<Statement>> statements; //ELEM MAY NULL

        std::unique_ptr<TypeInfo> Check(SymbolTable &table, bool &ok);
        void Show();
        virtual std::string GenCCode(SymbolTable &table, bool isRef) override;
    };

    struct CompoundStatement : ASTNode
    {
        std::unique_ptr<StatementList> statementList;

        CompoundStatement() {}
        CompoundStatement(std::unique_ptr<StatementList> &&statementList) : statementList(std::move(statementList)) {}

        std::unique_ptr<TypeInfo> Check(SymbolTable &table, bool &ok);
        void Show();
        virtual std::string GenCCode(SymbolTable &table, bool isRef) override;
    };

    struct SubProgramHead : public ASTNode
    {
        std::string name;
        std::unique_ptr<ParameterList> parameters;
        std::unique_ptr<BasicTypeDecl> returnType; //NAY NULL

        SubProgramHead() {}
        SubProgramHead(std::string name,
                       std::unique_ptr<ParameterList> &&parameters,
                       std::unique_ptr<BasicTypeDecl> &&returnType) : name(name), parameters(std::move(parameters)), returnType(std::move(returnType)) {}

        std::unique_ptr<TypeInfo> Check(SymbolTable &table, bool &ok);
        void Show();
        virtual std::string GenCCode(SymbolTable &table, bool isRef) override;
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

        std::unique_ptr<TypeInfo> Check(SymbolTable &table, bool &ok);
        void Show();
        virtual std::string GenCCode(SymbolTable &table, bool isRef) override;
    };

    struct SubProgram : public ASTNode
    {
        std::unique_ptr<SubProgramHead> head;
        std::unique_ptr<SubProgramBody> body;

        SubProgram() {}
        SubProgram(std::unique_ptr<SubProgramHead> &&head,
                   std::unique_ptr<SubProgramBody> &&body) : head(std::move(head)), body(std::move(body)) {}

        std::unique_ptr<TypeInfo> Check(SymbolTable &table, bool &ok);
        void Show();
        virtual std::string GenCCode(SymbolTable &table, bool isRef) override;
    };

    struct SubProgramDeclarations : public ASTNode
    {
        std::vector<std::unique_ptr<SubProgram>> subPrograms;
        std::unique_ptr<TypeInfo> Check(SymbolTable &table, bool &ok);

        void Show();
        virtual std::string GenCCode(SymbolTable &table, bool isRef) override;
    };

    struct ProgramHead : ASTNode
    {
        std::string name;
        std::unique_ptr<Identifiers> identifiers;
        ProgramHead() {}
        ProgramHead(std::string name, std::unique_ptr<Identifiers> &&ids) : identifiers(std::move(ids)) {}

        std::unique_ptr<TypeInfo> Check(SymbolTable &table, bool &ok);
        void Show();
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
        std::unique_ptr<TypeInfo> Check(SymbolTable &table, bool &ok);
        void Show();
        virtual std::string GenCCode(SymbolTable &table, bool isRef) override;
    };

    struct Program : ASTNode
    {
        std::unique_ptr<ProgramHead> programHead;
        std::unique_ptr<ProgramBody> programBody;
        Program() {}
        Program(std::unique_ptr<ProgramHead> &&head, std::unique_ptr<ProgramBody> &&body) : programHead(std::move(head)), programBody(std::move(body)) {}

        std::unique_ptr<TypeInfo> Check(SymbolTable &table, bool &ok);
        void Show();
        virtual std::string GenCCode(SymbolTable &table, bool isRef) override;
    };

}

#endif