compiler: driver.c lexer.c parser.c ast.c symboltable.c typecheck.c
	gcc -o compiler driver.c lexer.c parser.c ast.c symboltable.c typecheck.c -I.