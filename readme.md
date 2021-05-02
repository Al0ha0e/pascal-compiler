ref: [https://www.freepascal.org/docs-html/ref/ref.html#refch13.html](https://www.freepascal.org/docs-html/ref/ref.html#refch13.html)
##### Token类型
- id
- ( 
- ) 
- , 
- ; 
- . 
- : 
- [ 
- ]
- ' 
- ..
- assignop
  - :=
- =
- relop
  - >,<,<=,>=,<>
- +
- -
- or
- mulop
  - *,/,div,mod,and
- unop
  - not
- program,const,var,procedure,function,begin,end,array,of,integer,real,boolean,char,if,then,else,for,to,do,read,write
- number constant
- char constant

##### 对原文法进行的修改

- 合并variable与procedure_call，二者区别在语义分析时利用类型信息解决
```
statement_list statement_list ; statement
statement_list statement
statement variable assignop expression
statement procedure_call
statement compound_statement
statement if expression then statement else_part
statement for id assignop expression to expression do statement
statement read ( variable_list  )
statement write ( expression_list )
statement EPS
```
改为
```
statement_list statement_list statement
statement variable assignop expression ;
statement procedure_call ;
statement compound_statement ;
statement if expression then statement else_part ;
statement for id assignop expression to expression do statement ;
statement read ( variable_list  ) ;
statement write ( expression_list ) ;
statement EPS
```
- 原语句if表达式存在二义性

##### 文法改变带来的问题
- 结合性改变：在AST中改回来