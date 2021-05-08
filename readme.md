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

- 原文法factor里不包括字符常量，暂时不加文法

##### 文法改变带来的问题
- 结合性改变：在AST中改回来

##### 当前任务
- 测试（单元测试、集成测试）
    1. 根据各模块设计相应的测试用例并检查bug
        - lexer parser（很复杂） syntax types
- 整理详细的语义错误类型及其输出
    - 目前计划根据官方文档整理错误类型，然后设计对应的程序，查看输出
- 实现图形化界面
    - 能够直观展示各模块的输出：
        - 记号流、语法树、符号表、编译结果、错误信息
        - 考虑添加单步执行？主要用于parser
- 课程设计报告以及验收ppt
    - 基本要求：程序演示流畅、各成员程序讲解清晰
    - In Addition， 卷

##### 各文件说明
- lexer.h/cpp 词法分析器
- parser.h/cpp 语法分析器
    - tools/ 语法分析工具
        - left_recur.cpp 消除左递归
        - ll1_construct 使用LL（1）分析
- syntax.h/cpp 语义分析
- types.h/cpp 类型检查
- ast.h/cpp 生成语法树
