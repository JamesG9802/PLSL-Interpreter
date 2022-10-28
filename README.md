# PLSL-Interpreter
Lexical Analyzer &amp; interpreter for Pascal-Like Language
1.  Prog ::= PROGRAM IDENT; DeclBlock ProgBody  
2.  DeclBlock ::= VAR {DeclStmt;} 
3.  DeclStmt ::= Ident {, Ident} : (Integer | Real | String) 
4.  ProgBody ::= BEGIN {Stmt;} END 
5.  Stmt ::= AssigStmt | IfStmt | WriteLnStmt  
6.  WriteLnStmt ::= WRITELN (ExprList) 
7.  IfStmt ::= IF ( LogicExpr ) THEN Stmt [ELSE Stmt] 
8.  AssignStmt ::= Var := Expr 
9.  ExprList ::= Expr {, Expr} 
10. Expr ::= Term {(+|-) Term} 
11. Term ::= SFactor {( * | / ) SFactor}  
12. SFactor ::= [(+ | -)] Factor  
13. LogicExpr ::= Expr (= | > | <) Expr 
14. Var ::= IDENT 
15. Factor ::= IDENT | ICONST | RCONST | SCONST | (Expr) 
