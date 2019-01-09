//2014B4A70867P Karan Makhija

#ifndef PARSERDEF_H
#define PARSERDEF_H

#include "lexerDef.h"
#include <stdio.h>

#define numTerminal 40
#define	numNonTerminal 45
#define numRules 87
#define maxRHS 12
#define maxRuleChar 150


typedef enum{
AND,
ASSIGNOP,
CL,
COMMA,
DIV,
DOLLAR,
ELSE,
END,
ENDIF,
EPSILON,
EQ,
FUNCTION,
FUNID,
GE,
GT,
ID,
IF,
INT,
LE,
LT,
MAIN,
MATRIX,
MINUS,
MUL,
NE,
NOT,
NUM,
OP,
OR,
PLUS,
PRINT,
READ,
REAL,
RNUM,
SEMICOLON,
SIZE,
SQC,
SQO,
STR,
STRING
}Terminal;




typedef enum{
arithmeticExpression,
arithmeticFactor,
arithmeticTerm,
assginmentStmtType1,
assginmentStmtType2,
booleanExpression,
constrainedVars,
declarationStmt,
elseStmt,
funCallStmt,
functionDef,
ifStmt,
inputParameterList,
ioStmt,
leftHandSideListVar,
leftHandSideSingleVar,
listVar,
logicalOp,
mainFunction,
matrix,
matrixElement,
moreIds,
moreNums,
moreRows,
moreStmtsAndFunctionDefs,
operatorHighPrecedence,
operatorLowPrecedence,
otherStmts,
parameterList,
relationalOp,
remainArithmeticExpression,
remainArithmeticTerm,
remainingList,
rightHandSideType1,
rightHandSideType2,
row,
rows,
sizeExpression,
start,
stmt,
stmtOrFunctionDef,
stmtsAndFunctionDefs,
type,
var,
varList
}Nonterminal;

typedef union
{
	Nonterminal nonTerm;
	Terminal Term;

}Element;

typedef enum{
	TERMINAL,
	NONTERMINAL

}Nodetype;

typedef struct{
	Element value;
	Nodetype type;
	int numRHS;
}Node;



struct ParseTreeNode{
	tokenInfo *token;
	Node gram;
	int childCount;
	struct ParseTreeNode * parent;
	struct ParseTreeNode ** children;
};

typedef struct ParseTreeNode ParseTreeNode;

#endif