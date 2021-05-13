std::unique_ptr<PascalAST::ASTNode> GenAstNode(std::string expressionLeft,         std::string expressionFirst,std::vector<PascalAST::ASTNode> subNodes){
if(expressionLeft=="programstruct"){
if(expressionFirst=="program_head"){
//programstruct-->program_head ; program_body .
}
}
if(expressionLeft=="program_head"){
if(expressionFirst=="program"){
//program_head-->program id program_head_89
}
}
if(expressionLeft=="program_body"){
if(expressionFirst=="const_declarations"){
//program_body-->const_declarations var_declarations subprogram_declarations compound_statement
}
}
if(expressionLeft=="program_head_89"){
if(expressionFirst=="EPS"){
//program_head_89-->EPS
}
if(expressionFirst=="("){
//program_head_89-->( idlist )
}
}
if(expressionLeft=="const_declarations"){
if(expressionFirst=="EPS"){
//const_declarations-->EPS
}
if(expressionFirst=="const"){
//const_declarations-->const const_declaration ;
}
}
if(expressionLeft=="var_declarations"){
if(expressionFirst=="EPS"){
//var_declarations-->EPS
}
if(expressionFirst=="var"){
//var_declarations-->var var_declaration ;
}
}
if(expressionLeft=="subprogram_declarations"){
if(expressionFirst=="EPS"){
//subprogram_declarations-->EPS subprogram_declarations_79
}
}
if(expressionLeft=="compound_statement"){
if(expressionFirst=="begin"){
//compound_statement-->begin statement_list end
}
}
if(expressionLeft=="idlist"){
if(expressionFirst=="id"){
//idlist-->id idlist_78
}
}
if(expressionLeft=="idlist_78"){
if(expressionFirst=="EPS"){
//idlist_78-->EPS
}
if(expressionFirst==","){
//idlist_78-->, id idlist_78
}
}
if(expressionLeft=="const_declaration"){
if(expressionFirst=="id"){
//const_declaration-->id = const_value const_declaration_80
}
}
if(expressionLeft=="var_declaration"){
if(expressionFirst=="idlist"){
//var_declaration-->idlist : type var_declaration_81
}
}
if(expressionLeft=="subprogram_declarations_79"){
if(expressionFirst=="EPS"){
//subprogram_declarations_79-->EPS
}
if(expressionFirst=="subprogram"){
//subprogram_declarations_79-->subprogram ; subprogram_declarations_79
}
}
if(expressionLeft=="statement_list"){
if(expressionFirst=="statement"){
//statement_list-->statement statement_list_84
}
}
if(expressionLeft=="const_value"){
if(expressionFirst=="+"){
//const_value-->+ num
}
if(expressionFirst=="num"){
//const_value-->num
}
if(expressionFirst=="-"){
//const_value-->- num
}
if(expressionFirst=="'"){
//const_value-->' letter '
}
}
if(expressionLeft=="const_declaration_80"){
if(expressionFirst=="EPS"){
//const_declaration_80-->EPS
}
if(expressionFirst==";"){
//const_declaration_80-->; id = const_value const_declaration_80
}
}
if(expressionLeft=="num"){
if(expressionFirst=="digits"){
//num-->digits
}
if(expressionFirst=="float"){
//num-->float
}
}
if(expressionLeft=="type"){
if(expressionFirst=="basic_type"){
//type-->basic_type
}
if(expressionFirst=="array"){
//type-->array [ period ] of basic_type
}
}
if(expressionLeft=="var_declaration_81"){
if(expressionFirst=="EPS"){
//var_declaration_81-->EPS
}
if(expressionFirst==";"){
//var_declaration_81-->; idlist : type var_declaration_81
}
}
if(expressionLeft=="basic_type"){
if(expressionFirst=="integer"){
//basic_type-->integer
}
if(expressionFirst=="real"){
//basic_type-->real
}
if(expressionFirst=="boolean"){
//basic_type-->boolean
}
if(expressionFirst=="char"){
//basic_type-->char
}
}
if(expressionLeft=="period"){
if(expressionFirst=="digits"){
//period-->digits .. digits period_82
}
}
if(expressionLeft=="period_82"){
if(expressionFirst=="EPS"){
//period_82-->EPS
}
if(expressionFirst==","){
//period_82-->, digits .. digits period_82
}
}
if(expressionLeft=="subprogram"){
if(expressionFirst=="subprogram_head"){
//subprogram-->subprogram_head ; subprogram_body
}
}
if(expressionLeft=="subprogram_head"){
if(expressionFirst=="procedure"){
//subprogram_head-->procedure id formal_parameter
}
if(expressionFirst=="function"){
//subprogram_head-->function id formal_parameter : basic_type
}
}
if(expressionLeft=="subprogram_body"){
if(expressionFirst=="const_declarations"){
//subprogram_body-->const_declarations var_declarations compound_statement
}
}
if(expressionLeft=="formal_parameter"){
if(expressionFirst=="EPS"){
//formal_parameter-->EPS
}
if(expressionFirst=="("){
//formal_parameter-->( parameter_list )
}
}
if(expressionLeft=="parameter_list"){
if(expressionFirst=="parameter"){
//parameter_list-->parameter parameter_list_83
}
}
if(expressionLeft=="parameter"){
if(expressionFirst=="var_parameter"){
//parameter-->var_parameter
}
if(expressionFirst=="value_parameter"){
//parameter-->value_parameter
}
}
if(expressionLeft=="parameter_list_83"){
if(expressionFirst=="EPS"){
//parameter_list_83-->EPS
}
if(expressionFirst==";"){
//parameter_list_83-->; parameter parameter_list_83
}
}
if(expressionLeft=="var_parameter"){
if(expressionFirst=="var"){
//var_parameter-->var value_parameter
}
}
if(expressionLeft=="value_parameter"){
if(expressionFirst=="idlist"){
//value_parameter-->idlist : basic_type
}
}
if(expressionLeft=="statement"){
if(expressionFirst=="EPS"){
//statement-->EPS
}
if(expressionFirst=="compound_statement"){
//statement-->compound_statement
}
if(expressionFirst=="variable"){
//statement-->variable statement_90
}
if(expressionFirst=="if"){
//statement-->if expression then statement else_part
}
if(expressionFirst=="for"){
//statement-->for id assignop expression to expression do statement
}
if(expressionFirst=="read"){
//statement-->read ( variable_list )
}
if(expressionFirst=="write"){
//statement-->write ( expression_list )
}
}
if(expressionLeft=="statement_list_84"){
if(expressionFirst=="EPS"){
//statement_list_84-->EPS
}
if(expressionFirst==";"){
//statement_list_84-->; statement statement_list_84
}
}
if(expressionLeft=="variable"){
if(expressionFirst=="id"){
//variable-->id id_varpart
}
}
if(expressionLeft=="statement_90"){
if(expressionFirst=="EPS"){
//statement_90-->EPS
}
if(expressionFirst=="assignop"){
//statement_90-->assignop expression
}
}
if(expressionLeft=="expression"){
if(expressionFirst=="simple_expression"){
//expression-->simple_expression expression_91
}
}
if(expressionLeft=="else_part"){
if(expressionFirst=="EPS"){
//else_part-->EPS
}
if(expressionFirst=="else"){
//else_part-->else statement
}
}
if(expressionLeft=="variable_list"){
if(expressionFirst=="variable"){
//variable_list-->variable variable_list_85
}
}
if(expressionLeft=="expression_list"){
if(expressionFirst=="expression"){
//expression_list-->expression expression_list_86
}
}
if(expressionLeft=="id_varpart"){
if(expressionFirst=="EPS"){
//id_varpart-->EPS
}
if(expressionFirst=="["){
//id_varpart-->[ expression_list ]
}
if(expressionFirst=="("){
//id_varpart-->( expression_list )
}
}
if(expressionLeft=="simple_expression"){
if(expressionFirst=="term"){
//simple_expression-->term simple_expression_87
}
}
if(expressionLeft=="expression_91"){
if(expressionFirst=="EPS"){
//expression_91-->EPS
}
if(expressionFirst=="="){
//expression_91-->= simple_expression
}
if(expressionFirst=="relop"){
//expression_91-->relop simple_expression
}
}
if(expressionLeft=="variable_list_85"){
if(expressionFirst=="EPS"){
//variable_list_85-->EPS
}
if(expressionFirst==","){
//variable_list_85-->, variable variable_list_85
}
}
if(expressionLeft=="expression_list_86"){
if(expressionFirst=="EPS"){
//expression_list_86-->EPS
}
if(expressionFirst==","){
//expression_list_86-->, expression expression_list_86
}
}
if(expressionLeft=="term"){
if(expressionFirst=="factor"){
//term-->factor term_88
}
}
if(expressionLeft=="simple_expression_87"){
if(expressionFirst=="EPS"){
//simple_expression_87-->EPS
}
if(expressionFirst=="+"){
//simple_expression_87-->+ term simple_expression_87
}
if(expressionFirst=="-"){
//simple_expression_87-->- term simple_expression_87
}
if(expressionFirst=="or"){
//simple_expression_87-->or term simple_expression_87
}
}
if(expressionLeft=="factor"){
if(expressionFirst=="num"){
//factor-->num
}
if(expressionFirst=="-"){
//factor-->- factor
}
if(expressionFirst=="("){
//factor-->( expression )
}
if(expressionFirst=="variable"){
//factor-->variable
}
if(expressionFirst=="not"){
//factor-->not factor
}
}
if(expressionLeft=="term_88"){
if(expressionFirst=="EPS"){
//term_88-->EPS
}
if(expressionFirst=="mulop"){
//term_88-->mulop factor term_88
}
}
}
