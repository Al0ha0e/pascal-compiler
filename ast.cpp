#include "ast.h"

namespace PascalAST
{
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

    std::unique_ptr<ASTNode> GenAstNode(std::string expressionLeft, std::string expressionFirst, std::vector<std::unique_ptr<ASTNode>> subNodes)
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
                return std::unique_ptr<ASTNode>();
            }
            if (expressionFirst == "(")
            {
                //program_head_89-->( idlist )
                return std::move(subNodes[0]);
            }
        }
        if (expressionLeft == "const_declarations")
        {
            if (expressionFirst == "EPS")
            {
                //const_declarations-->EPS
                return std::unique_ptr<ASTNode>();
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
                return std::unique_ptr<ASTNode>();
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
                idlist->identifiers.push_back(Unpack<OriASTNode>(subNodes[0])->content);
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
                idlist->identifiers.push_back(Unpack<OriASTNode>(subNodes[1])->content);
                return Pack(idlist);
            }
        }
        if (expressionLeft == "const_declaration")
        {
            if (expressionFirst == "id")
            {
                //const_declaration-->id = const_value const_declaration_80
                BasicType *bType = new BasicType();
                //TODO bType->basicType =
                ConstantDeclaration *const_declaration = new ConstantDeclaration(
                    Unpack<OriASTNode>(subNodes[0])->content,
                    std::unique_ptr<BasicType>(bType),
                    Unpack<OriASTNode>(subNodes[2])->content);
                auto constantDeclarations(Unpack<ConstantDeclarations>(subNodes[3]));
                constantDeclarations->constantDeclarations.push_back(std::unique_ptr<ConstantDeclaration>(const_declaration));
                return Pack(constantDeclarations);
            }
        }
        if (expressionLeft == "var_declaration")
        {
            if (expressionFirst == "idlist")
            {
                //var_declaration-->idlist : type var_declaration_81
                VariableDeclaration *variableDeclaration = new VariableDeclaration(
                    Unpack<Type>(subNodes[2]),
                    Unpack<Identifiers>(subNodes[0]));
                auto variableDeclarations(Unpack<VariableDeclarations>(subNodes[3]));
                variableDeclarations->variableDeclarations.push_back(std::unique_ptr<VariableDeclaration>(variableDeclaration));
                return Pack(variableDeclarations);
            }
        }
        if (expressionLeft == "subprogram_declarations_79")
        {
            if (expressionFirst == "EPS")
            {
                //subprogram_declarations_79-->EPS
                ASTNode *subProgramDeclarations = new SubProgramDeclarations;
                return std::unique_ptr<ASTNode>(subProgramDeclarations);
            }
            if (expressionFirst == "subprogram")
            {
                //subprogram_declarations_79-->subprogram ; subprogram_declarations_79
                auto subProgramDeclarations(Unpack<SubProgramDeclarations>(subNodes[2]));
                subProgramDeclarations->subPrograms.push_back(Unpack<SubProgram>(subNodes[0]));
                return Pack(subProgramDeclarations);
            }
        }
        if (expressionLeft == "statement_list")
        {
            if (expressionFirst == "statement")
            {
                //statement_list-->statement statement_list_84
                auto statementList(Unpack<StatementList>(subNodes[1]));
                statementList->statements.push_back(Unpack<Statement>(subNodes[0]));
                return Pack(statementList);
            }
        }
        if (expressionLeft == "const_value")
        {
            if (expressionFirst == "+")
            {
                //const_value-->+ num
                OriASTNode *oriAstNode = new OriASTNode();
                std::string preContent = Unpack<OriASTNode>(subNodes[1])->content;
                //TODO oriAstNode->content =
                return std::unique_ptr<ASTNode>(oriAstNode);
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
                std::string preContent = Unpack<OriASTNode>(subNodes[1])->content;
                //TODO oriAstNode->content =
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
                BasicType *bType = new BasicType();
                //TODO bType->basicType =
                ConstantDeclaration *constantDeclaration = new ConstantDeclaration(
                    Unpack<OriASTNode>(subNodes[1])->content,
                    std::unique_ptr<BasicType>(bType),
                    Unpack<OriASTNode>(subNodes[3])->content);
                auto constantDeclarations(Unpack<ConstantDeclarations>(subNodes[4]));
                constantDeclarations->constantDeclarations.push_back(std::unique_ptr<ConstantDeclaration>(constantDeclaration));
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
                ArrayType *arrayType = new ArrayType(
                    Unpack<Ranges>(subNodes[2]),
                    Unpack<BasicType>(subNodes[5]));
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
                    Unpack<Type>(subNodes[3]),
                    Unpack<Identifiers>(subNodes[1]));
                auto variableDeclarations(Unpack<VariableDeclarations>(subNodes[4]));
                variableDeclarations->variableDeclarations.push_back(std::unique_ptr<VariableDeclaration>(variableDeclaration));
                return Pack(variableDeclarations);
            }
        }
        if (expressionLeft == "basic_type")
        {
            if (expressionFirst == "integer")
            {
                //basic_type-->integer
                BasicType *bType = new BasicType();
                bType->basicType = Unpack<OriASTNode>(subNodes[0])->content;
                return std::unique_ptr<ASTNode>((ASTNode *)bType);
            }
            if (expressionFirst == "real")
            {
                //basic_type-->real
                BasicType *bType = new BasicType();
                bType->basicType = Unpack<OriASTNode>(subNodes[0])->content;
                return std::unique_ptr<ASTNode>((ASTNode *)bType);
            }
            if (expressionFirst == "boolean")
            {
                //basic_type-->boolean
                BasicType *bType = new BasicType();
                bType->basicType = Unpack<OriASTNode>(subNodes[0])->content;
                return std::unique_ptr<ASTNode>((ASTNode *)bType);
            }
            if (expressionFirst == "char")
            {
                //basic_type-->char
                BasicType *bType = new BasicType();
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
                //TODO
                // range->l = Unpack<OriASTNode>(subNodes[0])->content;
                // range->r = Unpack<OriASTNode>(subNodes[2])->content;
                auto periods(Unpack<Ranges>(subNodes[3]));
                periods->ranges.push_back(std::unique_ptr<Range>(range));
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
                //TODO
                // range->l = Unpack<OriASTNode>(subNodes[1])->content;
                // range->r = Unpack<OriASTNode>(subNodes[3])->content;
                auto periods(Unpack<Ranges>(subNodes[4]));
                periods->ranges.push_back(std::unique_ptr<Range>(range));
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
                    std::unique_ptr<BasicType>());
                return std::unique_ptr<ASTNode>(subProgramHead);
            }
            if (expressionFirst == "function")
            {
                //subprogram_head-->function id formal_parameter : basic_type
                BasicType *basicType = new BasicType();
                basicType->basicType = Unpack<OriASTNode>(subNodes[4])->content;

                ASTNode *subProgramHead = new SubProgramHead(
                    Unpack<OriASTNode>(subNodes[1])->content,
                    Unpack<ParameterList>(subNodes[2]),
                    std::unique_ptr<BasicType>(basicType));

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
                parameterList->parameters.push_back(Unpack<Parameter>(subNodes[0]));
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
                parameterList->parameters.push_back(Unpack<Parameter>(subNodes[1]));
                return Pack(parameterList);
            }
        }
        if (expressionLeft == "var_parameter")
        {
            if (expressionFirst == "var")
            {
                //var_parameter-->var value_parameter
                auto parameter(Unpack<Parameter>(subNodes[0]));
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
                    Unpack<BasicType>(subNodes[2]),
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
                //TODO
            }
            if (expressionFirst == "if")
            {
                //statement-->if expression then statement else_part
            }
            if (expressionFirst == "for")
            {
                //statement-->for id assignop expression to expression do statement
            }
            if (expressionFirst == "read")
            {
                //statement-->read ( variable_list )
            }
            if (expressionFirst == "write")
            {
                //statement-->write ( expression_list )
            }
        }
        if (expressionLeft == "statement_list_84")
        {
            if (expressionFirst == "EPS")
            {
                //statement_list_84-->EPS
            }
            if (expressionFirst == ";")
            {
                //statement_list_84-->; statement statement_list_84
            }
        }
        if (expressionLeft == "variable")
        {
            if (expressionFirst == "id")
            {
                //variable-->id id_varpart
            }
        }
        if (expressionLeft == "statement_90")
        {
            if (expressionFirst == "EPS")
            {
                //statement_90-->EPS
            }
            if (expressionFirst == "assignop")
            {
                //statement_90-->assignop expression
            }
        }
        if (expressionLeft == "expression")
        {
            if (expressionFirst == "simple_expression")
            {
                //expression-->simple_expression expression_91
            }
        }
        if (expressionLeft == "else_part")
        {
            if (expressionFirst == "EPS")
            {
                //else_part-->EPS
            }
            if (expressionFirst == "else")
            {
                //else_part-->else statement
            }
        }
        if (expressionLeft == "variable_list")
        {
            if (expressionFirst == "variable")
            {
                //variable_list-->variable variable_list_85
            }
        }
        if (expressionLeft == "expression_list")
        {
            if (expressionFirst == "expression")
            {
                //expression_list-->expression expression_list_86
            }
        }
        if (expressionLeft == "id_varpart")
        {
            if (expressionFirst == "EPS")
            {
                //id_varpart-->EPS
            }
            if (expressionFirst == "[")
            {
                //id_varpart-->[ expression_list ]
            }
            if (expressionFirst == "(")
            {
                //id_varpart-->( expression_list )
            }
        }
        if (expressionLeft == "simple_expression")
        {
            if (expressionFirst == "term")
            {
                //simple_expression-->term simple_expression_87
            }
        }
        if (expressionLeft == "expression_91")
        {
            if (expressionFirst == "EPS")
            {
                //expression_91-->EPS
            }
            if (expressionFirst == "=")
            {
                //expression_91-->= simple_expression
            }
            if (expressionFirst == "relop")
            {
                //expression_91-->relop simple_expression
            }
        }
        if (expressionLeft == "variable_list_85")
        {
            if (expressionFirst == "EPS")
            {
                //variable_list_85-->EPS
            }
            if (expressionFirst == ",")
            {
                //variable_list_85-->, variable variable_list_85
            }
        }
        if (expressionLeft == "expression_list_86")
        {
            if (expressionFirst == "EPS")
            {
                //expression_list_86-->EPS
            }
            if (expressionFirst == ",")
            {
                //expression_list_86-->, expression expression_list_86
            }
        }
        if (expressionLeft == "term")
        {
            if (expressionFirst == "factor")
            {
                //term-->factor term_88
            }
        }
        if (expressionLeft == "simple_expression_87")
        {
            if (expressionFirst == "EPS")
            {
                //simple_expression_87-->EPS
            }
            if (expressionFirst == "+")
            {
                //simple_expression_87-->+ term simple_expression_87
            }
            if (expressionFirst == "-")
            {
                //simple_expression_87-->- term simple_expression_87
            }
            if (expressionFirst == "or")
            {
                //simple_expression_87-->or term simple_expression_87
            }
        }
        if (expressionLeft == "factor")
        {
            if (expressionFirst == "num")
            {
                //factor-->num
            }
            if (expressionFirst == "-")
            {
                //factor-->- factor
            }
            if (expressionFirst == "(")
            {
                //factor-->( expression )
            }
            if (expressionFirst == "variable")
            {
                //factor-->variable
            }
            if (expressionFirst == "not")
            {
                //factor-->not factor
            }
        }
        if (expressionLeft == "term_88")
        {
            if (expressionFirst == "EPS")
            {
                //term_88-->EPS
                return std::unique_ptr<Term>();
            }
            if (expressionFirst == "mulop")
            {
                //term_88-->mulop factor term_88
            }
        }
    }
}