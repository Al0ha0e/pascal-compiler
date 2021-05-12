#include "ast.h"

#include <string>
#include <iostream>

namespace PascalAST
{
    void Term::Rotate()
    {
        if (mulOpPart == nullptr)
            return;

        MulOpPart *mulPartP = new MulOpPart(mulOpPart->mulOp, std::move(firstFactor), std::unique_ptr<MulOpPart>());
        std::unique_ptr<MulOpPart> curMulPart(mulPartP);
        while (mulOpPart->followPart != nullptr)
        {
            mulPartP = new MulOpPart(
                mulOpPart->followPart->mulOp,
                std::move(mulOpPart->secondFactor),
                std::move(curMulPart));
            curMulPart = std::unique_ptr<MulOpPart>(mulPartP);
            mulOpPart = std::move(mulOpPart->followPart);
        }
        firstFactor = std::move(mulOpPart->secondFactor);
        mulOpPart = std::move(curMulPart);
    }

    void SimpleExpression::Rotate()
    {
        if (addOpPart == nullptr)
            return;
        firstTerm->Rotate();
        AddOpPart *addPartP = new AddOpPart(addOpPart->addOp, std::move(firstTerm), std::unique_ptr<AddOpPart>());
        std::unique_ptr<AddOpPart> curAddPart(addPartP);
        while (addOpPart->followPart != nullptr)
        {
            addOpPart->secondTerm->Rotate();
            addPartP = new AddOpPart(
                addOpPart->followPart->addOp,
                std::move(addOpPart->secondTerm),
                std::move(curAddPart));
            curAddPart = std::unique_ptr<AddOpPart>(addPartP);
            addOpPart = std::move(addOpPart->followPart);
        }
        addOpPart->secondTerm->Rotate();
        firstTerm = std::move(addOpPart->secondTerm);
        addOpPart = std::move(curAddPart);
    }

    std::unique_ptr<ASTNode> GenOriAstNode(CompilerFront::Token &token)
    {
        std::string tokenType = token.type;
        if (tokenType == "relop")
        {
            ASTNode *oriASTNode = new OriASTNode(token.content, token.content);
            return std::unique_ptr<ASTNode>(oriASTNode);
        }
        if (tokenType == "digits")
        {
            ASTNode *oriASTNode = new OriASTNode(token.content, std::string("int"));
            return std::unique_ptr<ASTNode>(oriASTNode);
        }
        if (tokenType == "float")
        {
            ASTNode *oriASTNode = new OriASTNode(token.content, std::string("float"));
            return std::unique_ptr<ASTNode>(oriASTNode);
        }
        if (tokenType == "mulop")
        {
            ASTNode *oriASTNode = new OriASTNode(token.type, token.content);
            return std::unique_ptr<ASTNode>(oriASTNode);
        }
        if (tokenType == "id")
        {
            std::cout << "ID CONTENT " << token.content << std::endl;
            ASTNode *oriASTNode = new OriASTNode(token.content, token.content);
            return std::unique_ptr<ASTNode>(oriASTNode);
        }
        ASTNode *oriASTNode = new OriASTNode(token.type, token.type);
        return std::unique_ptr<ASTNode>(oriASTNode);
    }

    std::unique_ptr<ASTNode> GenAstNode(std::string expressionLeft, std::string expressionFirst, std::vector<std::unique_ptr<ASTNode>> &subNodes)
    {
        if (expressionLeft == "programstruct")
        {
            if (expressionFirst == "program_head")
            {
                //programstruct-->program_head ; program_body .
                ASTNode *program = new Program(Unpack<ProgramHead>(subNodes[0]), Unpack<ProgramBody>(subNodes[2]));
                return std::unique_ptr<ASTNode>(program);
            }
        }
        if (expressionLeft == "program_head")
        {
            if (expressionFirst == "program")
            {
                //program_head-->program id program_head_89
                ASTNode *head = new ProgramHead(Unpack<OriASTNode>(subNodes[1])->content, Unpack<Identifiers>(subNodes[2]));
                return std::unique_ptr<ASTNode>(head);
            }
        }
        if (expressionLeft == "program_body")
        {
            if (expressionFirst == "const_declarations")
            {
                //program_body-->const_declarations var_declarations subprogram_declarations compound_statement
                ASTNode *body = new ProgramBody(
                    Unpack<ConstantDeclarations>(subNodes[0]),
                    Unpack<VariableDeclarations>(subNodes[1]),
                    Unpack<SubProgramDeclarations>(subNodes[2]),
                    Unpack<CompoundStatement>(subNodes[3]));
                return std::unique_ptr<ASTNode>(body);
            }
        }
        if (expressionLeft == "program_head_89")
        {
            if (expressionFirst == "EPS")
            {
                //program_head_89-->EPS
                ASTNode *idlist = new Identifiers();
                return std::unique_ptr<ASTNode>(idlist);
            }
            if (expressionFirst == "(")
            {
                //program_head_89-->( idlist )
                return std::move(subNodes[1]);
            }
        }
        if (expressionLeft == "const_declarations")
        {
            if (expressionFirst == "EPS")
            {
                //const_declarations-->EPS
                ASTNode *constantDeclarations = new ConstantDeclarations();
                return std::unique_ptr<ASTNode>(constantDeclarations);
            }
            if (expressionFirst == "const")
            {
                //const_declarations-->const const_declaration ;
                return std::move(subNodes[1]);
            }
        }
        if (expressionLeft == "var_declarations")
        {
            if (expressionFirst == "EPS")
            {
                //var_declarations-->EPS
                ASTNode *variableDeclarations = new VariableDeclarations();
                return std::unique_ptr<ASTNode>(variableDeclarations);
            }
            if (expressionFirst == "var")
            {
                //var_declarations-->var var_declaration ;
                return std::move(subNodes[1]);
            }
        }
        if (expressionLeft == "subprogram_declarations")
        {
            if (expressionFirst == "EPS")
            {
                //subprogram_declarations-->EPS subprogram_declarations_79
                return std::move(subNodes[0]);
            }
        }
        if (expressionLeft == "compound_statement")
        {
            if (expressionFirst == "begin")
            {
                //compound_statement-->begin statement_list end
                ASTNode *compound_statement = new CompoundStatement(Unpack<StatementList>(subNodes[1]));
                return std::unique_ptr<ASTNode>(compound_statement);
            }
        }
        if (expressionLeft == "idlist")
        {
            if (expressionFirst == "id")
            {
                //idlist-->id idlist_78
                auto idlist(Unpack<Identifiers>(subNodes[1]));
                idlist->identifiers.insert(
                    idlist->identifiers.begin(),
                    Unpack<OriASTNode>(subNodes[0])->content);
                return Pack(idlist);
            }
        }
        if (expressionLeft == "idlist_78")
        {
            if (expressionFirst == "EPS")
            {
                //idlist_78-->EPS
                ASTNode *idlist = new Identifiers();
                return std::unique_ptr<ASTNode>(idlist);
            }
            if (expressionFirst == ",")
            {
                //idlist_78-->, id idlist_78
                auto idlist(Unpack<Identifiers>(subNodes[2]));
                idlist->identifiers.insert(idlist->identifiers.begin(), Unpack<OriASTNode>(subNodes[1])->content);
                return Pack(idlist);
            }
        }
        if (expressionLeft == "const_declaration")
        {
            if (expressionFirst == "id")
            {
                //const_declaration-->id = const_value const_declaration_80
                BasicTypeDecl *bType = new BasicTypeDecl();
                auto constValue(Unpack<OriASTNode>(subNodes[2]));
                bType->basicType = constValue->info;
                ConstantDeclaration *const_declaration = new ConstantDeclaration(
                    Unpack<OriASTNode>(subNodes[0])->content,
                    std::unique_ptr<BasicTypeDecl>(bType),
                    constValue->content);
                auto constantDeclarations(Unpack<ConstantDeclarations>(subNodes[3]));
                constantDeclarations->constantDeclarations.insert(
                    constantDeclarations->constantDeclarations.begin(),
                    std::unique_ptr<ConstantDeclaration>(const_declaration));
                return Pack(constantDeclarations);
            }
        }
        if (expressionLeft == "var_declaration")
        {
            if (expressionFirst == "idlist")
            {
                //var_declaration-->idlist : type var_declaration_81
                VariableDeclaration *variableDeclaration = new VariableDeclaration(
                    Unpack<TypeDecl>(subNodes[2]),
                    Unpack<Identifiers>(subNodes[0]));
                auto variableDeclarations(Unpack<VariableDeclarations>(subNodes[3]));
                variableDeclarations->variableDeclarations.insert(
                    variableDeclarations->variableDeclarations.begin(),
                    std::unique_ptr<VariableDeclaration>(variableDeclaration));
                return Pack(variableDeclarations);
            }
        }
        if (expressionLeft == "subprogram_declarations_79")
        {
            if (expressionFirst == "EPS")
            {
                //subprogram_declarations_79-->EPS
                ASTNode *subProgramDeclarations = new SubProgramDeclarations();
                return std::unique_ptr<ASTNode>(subProgramDeclarations);
            }
            if (expressionFirst == "subprogram")
            {
                //subprogram_declarations_79-->subprogram ; subprogram_declarations_79
                auto subProgramDeclarations(Unpack<SubProgramDeclarations>(subNodes[2]));
                subProgramDeclarations->subPrograms.insert(
                    subProgramDeclarations->subPrograms.begin(),
                    Unpack<SubProgram>(subNodes[0]));
                return Pack(subProgramDeclarations);
            }
        }
        if (expressionLeft == "statement_list")
        {
            if (expressionFirst == "statement")
            {
                //statement_list-->statement statement_list_84
                auto statementList(Unpack<StatementList>(subNodes[1]));
                statementList->statements.insert(statementList->statements.begin(), Unpack<Statement>(subNodes[0]));
                return Pack(statementList);
            }
        }
        if (expressionLeft == "const_value")
        {
            if (expressionFirst == "+")
            {
                //const_value-->+ num
                return std::move(subNodes[1]);
            }
            if (expressionFirst == "num")
            {
                //const_value-->num
                return std::move(subNodes[0]);
            }
            if (expressionFirst == "-")
            {
                //const_value-->- num
                OriASTNode *oriAstNode = new OriASTNode();
                auto preNum(Unpack<OriASTNode>(subNodes[1]));
                oriAstNode->info = preNum->info;
                oriAstNode->content = oriAstNode->info == "float"
                                          ? std::to_string(-std::stof(preNum->content))
                                          : std::to_string(-std::stoi(preNum->content));
                return std::unique_ptr<ASTNode>(oriAstNode);
            }
            if (expressionFirst == "'")
            {
                //const_value-->' letter '
                return std::move(subNodes[1]);
            }
        }
        if (expressionLeft == "const_declaration_80")
        {
            if (expressionFirst == "EPS")
            {
                //const_declaration_80-->EPS
                ASTNode *constantDeclarations = new ConstantDeclarations();
                return std::unique_ptr<ASTNode>(constantDeclarations);
            }
            if (expressionFirst == ";")
            {
                //const_declaration_80-->; id = const_value const_declaration_80
                auto constValue(Unpack<OriASTNode>(subNodes[3]));
                BasicTypeDecl *bType = new BasicTypeDecl();
                bType->basicType = constValue->info;
                ConstantDeclaration *constantDeclaration = new ConstantDeclaration(
                    Unpack<OriASTNode>(subNodes[1])->content,
                    std::unique_ptr<BasicTypeDecl>(bType),
                    constValue->content);
                auto constantDeclarations(Unpack<ConstantDeclarations>(subNodes[4]));
                constantDeclarations->constantDeclarations.insert(
                    constantDeclarations->constantDeclarations.begin(),
                    std::unique_ptr<ConstantDeclaration>(constantDeclaration));
                return Pack(constantDeclarations);
            }
        }
        if (expressionLeft == "num")
        {
            if (expressionFirst == "digits")
            {
                //num-->digits
                return std::move(subNodes[0]);
            }
            if (expressionFirst == "float")
            {
                //num-->float
                return std::move(subNodes[0]);
            }
        }
        if (expressionLeft == "type")
        {
            if (expressionFirst == "basic_type")
            {
                //type-->basic_type
                return std::move(subNodes[0]);
            }
            if (expressionFirst == "array")
            {
                //type-->array [ period ] of basic_type
                ArrayTypeDecl *arrayType = new ArrayTypeDecl(
                    Unpack<Ranges>(subNodes[2]),
                    Unpack<BasicTypeDecl>(subNodes[5]));
                return std::unique_ptr<ASTNode>((ASTNode *)arrayType);
            }
        }
        if (expressionLeft == "var_declaration_81")
        {
            if (expressionFirst == "EPS")
            {
                //var_declaration_81-->EPS
                VariableDeclarations *variableDeclarations = new VariableDeclarations();
                return std::unique_ptr<ASTNode>((ASTNode *)variableDeclarations);
            }
            if (expressionFirst == ";")
            {
                //var_declaration_81-->; idlist : type var_declaration_81
                VariableDeclaration *variableDeclaration = new VariableDeclaration(
                    Unpack<TypeDecl>(subNodes[3]),
                    Unpack<Identifiers>(subNodes[1]));
                auto variableDeclarations(Unpack<VariableDeclarations>(subNodes[4]));
                variableDeclarations->variableDeclarations.insert(
                    variableDeclarations->variableDeclarations.begin(),
                    std::unique_ptr<VariableDeclaration>(variableDeclaration));
                return Pack(variableDeclarations);
            }
        }
        if (expressionLeft == "basic_type")
        {
            if (expressionFirst == "integer")
            {
                //basic_type-->integer
                BasicTypeDecl *bType = new BasicTypeDecl();
                bType->basicType = Unpack<OriASTNode>(subNodes[0])->content;
                return std::unique_ptr<ASTNode>((ASTNode *)bType);
            }
            if (expressionFirst == "real")
            {
                //basic_type-->real
                BasicTypeDecl *bType = new BasicTypeDecl();
                bType->basicType = Unpack<OriASTNode>(subNodes[0])->content;
                return std::unique_ptr<ASTNode>((ASTNode *)bType);
            }
            if (expressionFirst == "boolean")
            {
                //basic_type-->boolean
                BasicTypeDecl *bType = new BasicTypeDecl();
                bType->basicType = Unpack<OriASTNode>(subNodes[0])->content;
                return std::unique_ptr<ASTNode>((ASTNode *)bType);
            }
            if (expressionFirst == "char")
            {
                //basic_type-->char
                BasicTypeDecl *bType = new BasicTypeDecl();
                bType->basicType = Unpack<OriASTNode>(subNodes[0])->content;
                return std::unique_ptr<ASTNode>((ASTNode *)bType);
            }
        }
        if (expressionLeft == "period")
        {
            if (expressionFirst == "digits")
            {
                //period-->digits .. digits period_82
                Range *range = new Range();
                range->l = std::stoi(Unpack<OriASTNode>(subNodes[0])->content);
                range->r = std::stoi(Unpack<OriASTNode>(subNodes[2])->content);
                auto periods(Unpack<Ranges>(subNodes[3]));
                periods->ranges.insert(periods->ranges.begin(), std::unique_ptr<Range>(range));
                return Pack(periods);
            }
        }
        if (expressionLeft == "period_82")
        {
            if (expressionFirst == "EPS")
            {
                //period_82-->EPS
                ASTNode *ranges = new Ranges();
                return std::unique_ptr<ASTNode>(ranges);
            }
            if (expressionFirst == ",")
            {
                //period_82-->, digits .. digits period_82
                Range *range = new Range();
                range->l = std::stoi(Unpack<OriASTNode>(subNodes[1])->content);
                range->r = std::stoi(Unpack<OriASTNode>(subNodes[3])->content);
                auto periods(Unpack<Ranges>(subNodes[4]));
                periods->ranges.insert(periods->ranges.begin(), std::unique_ptr<Range>(range));
                return Pack(periods);
            }
        }
        if (expressionLeft == "subprogram")
        {
            if (expressionFirst == "subprogram_head")
            {
                //subprogram-->subprogram_head ; subprogram_body
                ASTNode *subProgram = new SubProgram(
                    Unpack<SubProgramHead>(subNodes[0]),
                    Unpack<SubProgramBody>(subNodes[2]));
                return std::unique_ptr<ASTNode>(subProgram);
            }
        }
        if (expressionLeft == "subprogram_head")
        {
            if (expressionFirst == "procedure")
            {
                //subprogram_head-->procedure id formal_parameter
                ASTNode *subProgramHead = new SubProgramHead(
                    Unpack<OriASTNode>(subNodes[1])->content,
                    Unpack<ParameterList>(subNodes[2]),
                    std::unique_ptr<BasicTypeDecl>());
                return std::unique_ptr<ASTNode>(subProgramHead);
            }
            if (expressionFirst == "function")
            {
                //subprogram_head-->function id formal_parameter : basic_type
                ASTNode *subProgramHead = new SubProgramHead(
                    Unpack<OriASTNode>(subNodes[1])->content,
                    Unpack<ParameterList>(subNodes[2]),
                    Unpack<BasicTypeDecl>(subNodes[4]));
                return std::unique_ptr<ASTNode>(subProgramHead);
            }
        }
        if (expressionLeft == "subprogram_body")
        {
            if (expressionFirst == "const_declarations")
            {
                //subprogram_body-->const_declarations var_declarations compound_statement
                ASTNode *subProgramBody = new SubProgramBody(
                    Unpack<ConstantDeclarations>(subNodes[0]),
                    Unpack<VariableDeclarations>(subNodes[1]),
                    Unpack<CompoundStatement>(subNodes[2]));
                return std::unique_ptr<ASTNode>(subProgramBody);
            }
        }
        if (expressionLeft == "formal_parameter")
        {
            if (expressionFirst == "EPS")
            {
                //formal_parameter-->EPS
                ParameterList *parameterList = new ParameterList();
                return std::unique_ptr<ASTNode>(parameterList);
            }
            if (expressionFirst == "(")
            {
                //formal_parameter-->( parameter_list )
                return std::move(subNodes[1]);
            }
        }
        if (expressionLeft == "parameter_list")
        {
            if (expressionFirst == "parameter")
            {
                //parameter_list-->parameter parameter_list_83
                auto parameterList(Unpack<ParameterList>(subNodes[1]));
                parameterList->parameters.insert(parameterList->parameters.begin(), Unpack<Parameter>(subNodes[0]));
                return Pack(parameterList);
            }
        }
        if (expressionLeft == "parameter")
        {
            if (expressionFirst == "var_parameter")
            {
                //parameter-->var_parameter
                return std::move(subNodes[0]);
            }
            if (expressionFirst == "value_parameter")
            {
                //parameter-->value_parameter
                return std::move(subNodes[0]);
            }
        }
        if (expressionLeft == "parameter_list_83")
        {
            if (expressionFirst == "EPS")
            {
                //parameter_list_83-->EPS
                ParameterList *parameterList = new ParameterList();
                return std::unique_ptr<ASTNode>(parameterList);
            }
            if (expressionFirst == ";")
            {
                //parameter_list_83-->; parameter parameter_list_83
                auto parameterList(Unpack<ParameterList>(subNodes[2]));
                parameterList->parameters.insert(parameterList->parameters.begin(), Unpack<Parameter>(subNodes[1]));
                return Pack(parameterList);
            }
        }
        if (expressionLeft == "var_parameter")
        {
            if (expressionFirst == "var")
            {
                //var_parameter-->var value_parameter
                auto parameter(Unpack<Parameter>(subNodes[1]));
                parameter->isRef = true;
                return Pack(parameter);
            }
        }
        if (expressionLeft == "value_parameter")
        {
            if (expressionFirst == "idlist")
            {
                //value_parameter-->idlist : basic_type
                ASTNode *parameter = new Parameter(
                    false,
                    Unpack<BasicTypeDecl>(subNodes[2]),
                    Unpack<Identifiers>(subNodes[0]));
                return std::unique_ptr<ASTNode>(parameter);
            }
        }
        if (expressionLeft == "statement")
        {
            if (expressionFirst == "EPS")
            {
                //statement-->EPS
                return std::unique_ptr<ASTNode>();
            }
            if (expressionFirst == "compound_statement")
            {
                //statement-->compound_statement
                ASTNode *subCompoundStatement = new SubCompoundStatement(Unpack<CompoundStatement>(subNodes[0]));
                return std::unique_ptr<ASTNode>(subCompoundStatement);
            }
            if (expressionFirst == "variable")
            {
                //statement-->variable statement_90
                auto variable(Unpack<Variable>(subNodes[0]));
                auto expression(Unpack<Expression>(subNodes[1]));
                if (expression == nullptr)
                {
                    if (variable->varPart != nullptr && !variable->varPart->isProcedureCall)
                    {
                        //TODO: Error
                    }
                    ASTNode *procedureCallStatement = new ProcedureCallStatement(std::move(variable));
                    return std::unique_ptr<ASTNode>(procedureCallStatement);
                }
                ASTNode *variableAssignStatement = new VariableAssignStatement(std::move(variable), std::move(expression));
                return std::unique_ptr<ASTNode>(variableAssignStatement);
            }
            if (expressionFirst == "if")
            {
                //statement-->if expression then statement else_part
                ASTNode *ifElseStatement = new IfElseStatement(
                    Unpack<Expression>(subNodes[1]),
                    Unpack<Statement>(subNodes[3]),
                    Unpack<Statement>(subNodes[4]));
                return std::unique_ptr<ASTNode>(ifElseStatement);
            }
            if (expressionFirst == "for")
            {
                //statement-->for id assignop expression to expression do statement
                ASTNode *forLoopStatement = new ForLoopStatement(
                    Unpack<OriASTNode>(subNodes[1])->content,
                    Unpack<Expression>(subNodes[3]),
                    Unpack<Expression>(subNodes[5]),
                    Unpack<Statement>(subNodes[7]));
                return std::unique_ptr<ASTNode>(forLoopStatement);
            }
            if (expressionFirst == "read")
            {
                //statement-->read ( variable_list )
                ASTNode *readStatement = new ReadStatement(
                    Unpack<VariableList>(subNodes[2]));
                return std::unique_ptr<ASTNode>(readStatement);
            }
            if (expressionFirst == "write")
            {
                //statement-->write ( expression_list )
                ASTNode *writeStatement = new WriteStatement(
                    Unpack<ExpressionList>(subNodes[2]));
                return std::unique_ptr<ASTNode>(writeStatement);
            }
        }
        if (expressionLeft == "statement_list_84")
        {
            if (expressionFirst == "EPS")
            {
                //statement_list_84-->EPS
                ASTNode *statementList = new StatementList();
                return std::unique_ptr<ASTNode>(statementList);
            }
            if (expressionFirst == ";")
            {
                //statement_list_84-->; statement statement_list_84
                auto statementList(Unpack<StatementList>(subNodes[2]));
                statementList->statements.insert(statementList->statements.begin(), Unpack<Statement>(subNodes[1]));
                return Pack(statementList);
            }
        }
        if (expressionLeft == "variable")
        {
            if (expressionFirst == "id")
            {
                //variable-->id id_varpart
                ASTNode *variable = new Variable(
                    Unpack<OriASTNode>(subNodes[0])->content,
                    Unpack<VarPart>(subNodes[1]));
                return std::unique_ptr<ASTNode>(variable);
            }
        }
        if (expressionLeft == "statement_90")
        {
            if (expressionFirst == "EPS")
            {
                //statement_90-->EPS
                return std::unique_ptr<ASTNode>();
            }
            if (expressionFirst == "assignop")
            {
                //statement_90-->assignop expression
                return std::move(subNodes[1]);
            }
        }
        if (expressionLeft == "expression")
        {
            if (expressionFirst == "simple_expression")
            {
                //expression-->simple_expression expression_91
                ASTNode *expression = new Expression(
                    Unpack<SimpleExpression>(subNodes[0]),
                    Unpack<RelPart>(subNodes[1]));
                return std::unique_ptr<ASTNode>(expression);
            }
        }
        if (expressionLeft == "else_part")
        {
            if (expressionFirst == "EPS")
            {
                //else_part-->EPS
                return std::unique_ptr<ASTNode>();
            }
            if (expressionFirst == "else")
            {
                //else_part-->else statement
                return std::move(subNodes[1]);
            }
        }
        if (expressionLeft == "variable_list")
        {
            if (expressionFirst == "variable")
            {
                //variable_list-->variable variable_list_85
                auto variableList(Unpack<VariableList>(subNodes[1]));
                variableList->variables.insert(variableList->variables.begin(), Unpack<Variable>(subNodes[0]));
                return Pack(variableList);
            }
        }
        if (expressionLeft == "expression_list")
        {
            if (expressionFirst == "expression")
            {
                //expression_list-->expression expression_list_86
                auto expressionList(Unpack<ExpressionList>(subNodes[1]));
                expressionList->expressions.insert(expressionList->expressions.begin(), Unpack<Expression>(subNodes[0]));
                return Pack(expressionList);
            }
        }
        if (expressionLeft == "id_varpart")
        {
            if (expressionFirst == "EPS")
            {
                //id_varpart-->EPS
                return std::unique_ptr<ASTNode>();
            }
            if (expressionFirst == "[")
            {
                //id_varpart-->[ expression_list ]
                ASTNode *varpart = new VarPart(
                    false,
                    Unpack<ExpressionList>(subNodes[1]));
                return std::unique_ptr<ASTNode>(varpart);
            }
            if (expressionFirst == "(")
            {
                //id_varpart-->( expression_list )
                ASTNode *varpart = new VarPart(
                    true,
                    Unpack<ExpressionList>(subNodes[1]));
                return std::unique_ptr<ASTNode>(varpart);
            }
        }
        if (expressionLeft == "simple_expression")
        {
            if (expressionFirst == "term")
            {
                //simple_expression-->term simple_expression_87
                ASTNode *simpleExpression = new SimpleExpression(
                    Unpack<Term>(subNodes[0]),
                    Unpack<AddOpPart>(subNodes[1]));
                ((SimpleExpression *)simpleExpression)->Rotate();
                return std::unique_ptr<ASTNode>(simpleExpression);
            }
        }
        if (expressionLeft == "expression_91")
        {
            if (expressionFirst == "EPS")
            {
                //expression_91-->EPS
                return std::unique_ptr<ASTNode>();
            }
            if (expressionFirst == "=")
            {
                //expression_91-->= simple_expression
                ASTNode *relPart = new RelPart(
                    Unpack<OriASTNode>(subNodes[0])->content,
                    Unpack<SimpleExpression>(subNodes[1]));
                return std::unique_ptr<ASTNode>(relPart);
            }
            if (expressionFirst == "relop")
            {
                //expression_91-->relop simple_expression
                ASTNode *relPart = new RelPart(
                    Unpack<OriASTNode>(subNodes[0])->content,
                    Unpack<SimpleExpression>(subNodes[1]));
                return std::unique_ptr<ASTNode>(relPart);
            }
        }
        if (expressionLeft == "variable_list_85")
        {
            if (expressionFirst == "EPS")
            {
                //variable_list_85-->EPS
                ASTNode *variableList = new VariableList();
                return std::unique_ptr<ASTNode>(variableList);
            }
            if (expressionFirst == ",")
            {
                //variable_list_85-->, variable variable_list_85
                auto variableList(Unpack<VariableList>(subNodes[2]));
                variableList->variables.insert(variableList->variables.begin(), Unpack<Variable>(subNodes[1]));
                return Pack(variableList);
            }
        }
        if (expressionLeft == "expression_list_86")
        {
            if (expressionFirst == "EPS")
            {
                //expression_list_86-->EPS
                ASTNode *expressionList = new ExpressionList();
                return std::unique_ptr<ASTNode>(expressionList);
            }
            if (expressionFirst == ",")
            {
                //expression_list_86-->, expression expression_list_86
                auto expressionList(Unpack<ExpressionList>(subNodes[2]));
                expressionList->expressions.insert(expressionList->expressions.begin(), Unpack<Expression>(subNodes[1]));
                return Pack(expressionList);
            }
        }
        if (expressionLeft == "term")
        {
            if (expressionFirst == "factor")
            {
                //term-->factor term_88
                ASTNode *term = new Term(
                    Unpack<Factor>(subNodes[0]),
                    Unpack<MulOpPart>(subNodes[1]));
                return std::unique_ptr<ASTNode>(term);
            }
        }
        if (expressionLeft == "simple_expression_87")
        {
            if (expressionFirst == "EPS")
            {
                //simple_expression_87-->EPS
                return std::unique_ptr<ASTNode>();
            }
            if (expressionFirst == "+")
            {
                //simple_expression_87-->+ term simple_expression_87
                ASTNode *addOpPart = new AddOpPart(
                    Unpack<OriASTNode>(subNodes[0])->content,
                    Unpack<Term>(subNodes[1]),
                    Unpack<AddOpPart>(subNodes[2]));
                return std::unique_ptr<ASTNode>(addOpPart);
            }
            if (expressionFirst == "-")
            {
                //simple_expression_87-->- term simple_expression_87
                ASTNode *addOpPart = new AddOpPart(
                    Unpack<OriASTNode>(subNodes[0])->content,
                    Unpack<Term>(subNodes[1]),
                    Unpack<AddOpPart>(subNodes[2]));
                return std::unique_ptr<ASTNode>(addOpPart);
            }
            if (expressionFirst == "or")
            {
                //simple_expression_87-->or term simple_expression_87
                ASTNode *addOpPart = new AddOpPart(
                    Unpack<OriASTNode>(subNodes[0])->content,
                    Unpack<Term>(subNodes[1]),
                    Unpack<AddOpPart>(subNodes[2]));
                return std::unique_ptr<ASTNode>(addOpPart);
            }
        }
        if (expressionLeft == "factor")
        {
            if (expressionFirst == "num")
            {
                //factor-->num
                NumFactor *numFactor = new NumFactor();
                auto oriASTNode(Unpack<OriASTNode>(subNodes[0]));
                numFactor->val = oriASTNode->content;
                numFactor->type = oriASTNode->info;
                return std::unique_ptr<ASTNode>((ASTNode *)numFactor);
            }
            if (expressionFirst == "-")
            {
                //factor-->- factor
                ASTNode *invFactor = new InvFactor(
                    Unpack<Factor>(subNodes[1]));
                return std::unique_ptr<ASTNode>(invFactor);
            }
            if (expressionFirst == "(")
            {
                //factor-->( expression )
                ASTNode *expressionFactor = new ExpressionFactor(
                    Unpack<Expression>(subNodes[1]));
                return std::unique_ptr<ASTNode>(expressionFactor);
            }
            if (expressionFirst == "variable")
            {
                //factor-->variable
                ASTNode *variableFactor = new VariableFactor(
                    Unpack<Variable>(subNodes[0]));
                return std::unique_ptr<ASTNode>(variableFactor);
            }
            if (expressionFirst == "not")
            {
                //factor-->not factor
                ASTNode *notFactor = new NotFactor(
                    Unpack<Factor>(subNodes[1]));
                return std::unique_ptr<ASTNode>(notFactor);
            }
        }
        if (expressionLeft == "term_88")
        {
            if (expressionFirst == "EPS")
            {
                //term_88-->EPS
                return std::unique_ptr<ASTNode>();
            }
            if (expressionFirst == "mulop")
            {
                //term_88-->mulop factor term_88
                ASTNode *mulOpPart = new MulOpPart(
                    Unpack<OriASTNode>(subNodes[0])->content,
                    Unpack<Factor>(subNodes[1]),
                    Unpack<MulOpPart>(subNodes[2]));
                return std::unique_ptr<ASTNode>(mulOpPart);
            }
        }
        return std::unique_ptr<ASTNode>();
    }
}