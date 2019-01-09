//2014B4A70867P Karan Makhija

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <math.h>

#include "ast.h"

/*
const char *stTerm[] ={"AND",
"ASSIGNOP",
"CL",
"COMMA",
"DIV",
"DOLLAR",
"ELSE",
"END",
"ENDIF",
"EPSILON",
"EQ",
"FUNCTION",
"FUNID",
"GE",
"GT",
"ID",
"IF",
"INT",
"LE",
"LT",
"MAIN",
"MATRIX",
"MINUS",
"MUL",
"NE",
"NOT",
"NUM",
"OP",
"OR",
"PLUS",
"PRINT",
"READ",
"REAL",
"RNUM",
"SEMICOLON",
"SIZE",
"SQC",
"SQO",
"STR",
"STRING"
};

const char *stNonTerm[] =
{"arithmeticExpression",
"arithmeticFactor",
"arithmeticTerm",
"assginmentStmtType1",
"assginmentStmtType2",
"booleanExpression",
"constrainedVars",
"declarationStmt",
"elseStmt",
"funCallStmt",
"functionDef",
"ifStmt",
"inputParameterList",
"ioStmt",
"leftHandSideListVar",
"leftHandSideSingleVar",
"listVar",
"logicalOp",
"mainFunction",
"matrix",
"matrixElement",
"moreIds",
"moreNums",
"moreRows",
"moreStmtsAndFunctionDefs",
"operatorHighPrecedence",
"operatorLowPrecedence",
"otherStmts",
"parameterList",
"relationalOp",
"remainArithmeticExpression",
"remainArithmeticTerm",
"remainingList",
"rightHandSideType1",
"rightHandSideType2",
"row",
"rows",
"sizeExpression",
"start",
"stmt",
"stmtOrFunctionDef",
"stmtsAndFunctionDefs",
"type",
"var",
"varList"
};
*/

extern int parsercount;
int astcount = 0;
extern char stTerm[40][100];
extern char stNonTerm[45][100];


AstNode * newNode(ParseTreeNode * root)
{
	astcount++;
	AstNode * temp = (AstNode *)malloc(sizeof(AstNode));
	temp->parent = NULL;
	temp->child = NULL;
	temp->link = NULL;
	temp->startscope = -1;
	temp->endscope = -1;
	temp->datatype = -1;
	temp->assigned = 0;
	temp->gram = root->gram;


	if(root->gram.type==TERMINAL)
	{
		temp->token = root->token;
	}
	else
	{
		temp->token = NULL;

	}
	return temp;

}




//How to construct symbol Table from AST
//NEW Node
//name of the node, when to assign token and gram

AstNode * constructAstTree(ParseTreeNode * parseRoot,AstNode * parent)
{

	if(parseRoot ==NULL)
		return NULL;

	AstNode * root=NULL;
	AstNode * addr=NULL;
	Element tnt;
	

	if(parseRoot->gram.type == NONTERMINAL)
	{
		tnt.nonTerm = parseRoot->gram.value.nonTerm;

		if(tnt.nonTerm == start)
		{
			root = newNode(parseRoot);
			root->parent = parent;
			root->child = constructAstTree(parseRoot->children[0],root);
			return root;

		}
		else if(tnt.nonTerm == mainFunction)
		{
			parent->startscope = parseRoot->children[0]->token->lineNo;
			parent->endscope = parseRoot->children[4]->token->lineNo;
			return constructAstTree(parseRoot->children[3],parent);
		}
		else if((tnt.nonTerm == stmtsAndFunctionDefs || tnt.nonTerm == moreStmtsAndFunctionDefs || tnt.nonTerm == sizeExpression || tnt.nonTerm == rows || tnt.nonTerm==inputParameterList ||tnt.nonTerm == otherStmts) )
		{
			if( parseRoot->children[0]->gram.type==TERMINAL &&  parseRoot->children[0]->gram.value.Term ==EPSILON)
				return NULL;

			addr = constructAstTree(parseRoot->children[0],parent);
			addr->link = constructAstTree(parseRoot->children[1],parent);
			return addr;

		}
		else if(tnt.nonTerm == functionDef)
		{

			//printf("%d\n",parseRoot->children[10]->token->lineNo);
			root = newNode(parseRoot);
			root->parent = parent;
			root->startscope = parseRoot->children[0]->token->lineNo;
			root->endscope = parseRoot->children[11]->token->lineNo;
			
			addr = constructAstTree(parseRoot->children[2],root);
			addr->link = constructAstTree(parseRoot->children[4],root);
			addr->link->link = constructAstTree(parseRoot->children[5],root);
			addr->link->link->link = constructAstTree(parseRoot->children[7],root);
			addr->link->link->link->link = constructAstTree(parseRoot->children[9],root);

			AstNode * temp = addr->link->link->link->link;

			while(temp->link!=NULL)
			{
				temp = temp->link;
			}

			temp->link = constructAstTree(parseRoot->children[10],root);	
			root->child = addr;
			return root;
		}
		else if(tnt.nonTerm == parameterList)
		{
			root = newNode(parseRoot);
			root->parent = parent;
			root->startscope = parent->startscope;
			root->endscope = parent->endscope;
			
			addr = constructAstTree(parseRoot->children[0],root);
			addr->link = constructAstTree(parseRoot->children[1],root);
			addr->link->link = constructAstTree(parseRoot->children[2],root);

			root->child = addr;
			return root;
		}
		else if(tnt.nonTerm == remainingList)
		{
			if( parseRoot->children[0]->gram.type==TERMINAL &&  parseRoot->children[0]->gram.value.Term ==EPSILON)
				return NULL;

			addr = constructAstTree(parseRoot->children[1],parent);
			addr->link = constructAstTree(parseRoot->children[2],parent);
			addr->link->link = constructAstTree(parseRoot->children[3],parent);
			return addr;	
		}
		else if(tnt.nonTerm == declarationStmt)
		{
			
			root = newNode(parseRoot);
			root->parent = parent;
			root->startscope = parent->startscope;
			root->endscope = parent->endscope;
			addr = constructAstTree(parseRoot->children[0],root);

			addr->link = constructAstTree(parseRoot->children[1],root);
			
			root->child = addr;
			return root;

		}
		else if(tnt.nonTerm == varList)
		{
			root = newNode(parseRoot);
			root->parent = parent;
			root->datatype = parent->datatype;
			parent->datatype = -1;
			root->startscope = parent->startscope;
			root->endscope = parent->endscope;
			addr = constructAstTree(parseRoot->children[0],root);
			addr->link = constructAstTree(parseRoot->children[1],root);

			root->child = addr;
			return root;

		}
		else if((tnt.nonTerm == moreIds || tnt.nonTerm ==moreRows|| tnt.nonTerm ==moreNums|| tnt.nonTerm==listVar))
		{
			if( parseRoot->children[0]->gram.type==TERMINAL &&  parseRoot->children[0]->gram.value.Term ==EPSILON)
				return NULL;

			addr = constructAstTree(parseRoot->children[1],parent);
			addr->link = constructAstTree(parseRoot->children[2],parent);
			return addr;	

		}
		else if(tnt.nonTerm == assginmentStmtType1)
		{

			root = newNode(parseRoot);
			root->parent = parent;
			root->startscope = parent->startscope;
			root->endscope = parent->endscope;
			addr = constructAstTree(parseRoot->children[0],root);
			addr->link = constructAstTree(parseRoot->children[1],root);
			addr->link->link = constructAstTree(parseRoot->children[2],root);
			root->child = addr;
			return root;

		}
		else if(tnt.nonTerm == assginmentStmtType2)
		{

			root = newNode(parseRoot);
			root->parent = parent;
			root->startscope = parent->startscope;
			root->endscope = parent->endscope;
			addr = constructAstTree(parseRoot->children[0],root);
			addr->link = constructAstTree(parseRoot->children[1],root);
			addr->link->link = constructAstTree(parseRoot->children[2],root);
			root->child = addr;
			return root;
		}
		else if(tnt.nonTerm == leftHandSideListVar)
		{
			return constructAstTree( parseRoot->children[1],parent);
		}
		else if(tnt.nonTerm == arithmeticExpression)
		{
			root = newNode(parseRoot);
			root->parent = parent;
			root->startscope = parent->startscope;
			root->endscope = parent->endscope;
			addr = constructAstTree(parseRoot->children[0],root);
			addr->link = constructAstTree(parseRoot->children[1],root);

			root->child = addr;
			return root;

		}
		else if((tnt.nonTerm == remainArithmeticExpression || tnt.nonTerm == remainArithmeticTerm))
		{
			if( parseRoot->children[0]->gram.type==TERMINAL &&  parseRoot->children[0]->gram.value.Term ==EPSILON)
				return NULL;

			addr = constructAstTree(parseRoot->children[0],parent);
			addr->link = constructAstTree(parseRoot->children[1],parent);
			addr->link->link = constructAstTree(parseRoot->children[2],parent);
			return addr;	
		}
		else if(tnt.nonTerm == arithmeticTerm)
		{
			root = newNode(parseRoot);
			root->parent = parent;
			root->startscope = parent->startscope;
			root->endscope = parent->endscope;
			addr = constructAstTree(parseRoot->children[0],root);
			addr->link = constructAstTree(parseRoot->children[1],root);

			root->child = addr;
			return root;

		}
		else if(tnt.nonTerm == arithmeticFactor)
		{
			if( parseRoot->children[0]->gram.type ==NONTERMINAL &&  parseRoot->children[0]->gram.value.nonTerm == var)
				return constructAstTree( parseRoot->children[0],parent);

			return constructAstTree( parseRoot->children[1],parent);
		}
		else if(tnt.nonTerm == var)
		{
			if( parseRoot->children[0]->gram.type ==TERMINAL &&  parseRoot->children[0]->gram.value.Term == ID)
			{
				root = newNode(parseRoot);
				root->parent = parent;
				root->startscope = parent->startscope;
				root->endscope = parent->endscope;
				addr = constructAstTree(parseRoot->children[0],root);
				addr->link = constructAstTree(parseRoot->children[1],root);

				root->child = addr;
				return root;
			}
			else
			{
				return constructAstTree( parseRoot->children[0],parent);
			}

		}
		else if(tnt.nonTerm == matrix)
		{
			root = newNode(parseRoot);
			root->parent = parent;
			root->startscope = parent->startscope;
			root->endscope = parent->endscope;
			root->child = constructAstTree(parseRoot->children[1],root);
			return root;

		}
		else if(tnt.nonTerm == row)
		{
			root = newNode(parseRoot);
			root->parent = parent;
			root->startscope = parent->startscope;
			root->endscope = parent->endscope;
			addr = constructAstTree(parseRoot->children[0],root);
			addr->link = constructAstTree(parseRoot->children[1],root);

			root->child = addr;
			return root;

		}
		else if(tnt.nonTerm ==matrixElement)
		{
			if( parseRoot->children[0]->gram.type==TERMINAL &&  parseRoot->children[0]->gram.value.Term ==EPSILON)
				return NULL;

			addr = constructAstTree(parseRoot->children[1],parent);
			addr->link = constructAstTree(parseRoot->children[3],parent);

			return addr;

		}
		else if(tnt.nonTerm == funCallStmt)
		{
			root = newNode(parseRoot);
			root->parent = parent;
			root->startscope = parent->startscope;
			root->endscope = parent->endscope;
			addr = constructAstTree(parseRoot->children[0],root);
			addr->link = constructAstTree(parseRoot->children[2],root);

			root->child = addr;
			return root;

		}
		else if(tnt.nonTerm == ioStmt)
		{
			root = newNode(parseRoot);
			root->parent = parent;
			root->startscope = parent->startscope;
			root->endscope = parent->endscope;
			addr = constructAstTree(parseRoot->children[0],root);
			addr->link = constructAstTree(parseRoot->children[2],root);

			root->child = addr;
			return root;

		}
		else if(tnt.nonTerm == ifStmt)
		{
			root = newNode(parseRoot);
			root->parent = parent;
			root->startscope = parent->startscope;
			root->endscope = parent->endscope;
			addr = constructAstTree(parseRoot->children[0],root);
			addr->link = constructAstTree(parseRoot->children[2],root);
			addr->link->link = constructAstTree(parseRoot->children[4],root);
			addr->link->link->link = constructAstTree(parseRoot->children[5],root);
			AstNode * temp = addr->link->link->link;

			if(temp==NULL)
			{
				addr->link->link->link = constructAstTree(parseRoot->children[6],root);
			}
			else
			{
				while(temp->link!=NULL)
				{
					temp = temp->link;
				}
				temp->link = constructAstTree(parseRoot->children[6],root);	
			}
			
			root->child = addr;
			return root;
		}
		else if(tnt.nonTerm == elseStmt)
		{
			if( parseRoot->children[0]->gram.type==TERMINAL &&  parseRoot->children[0]->gram.value.Term ==EPSILON)
				return NULL;

			root = newNode(parseRoot);
			root->parent = parent;
			root->startscope = parent->startscope;
			root->endscope = parent->endscope;
			addr = constructAstTree(parseRoot->children[0],root);
			addr->link = constructAstTree(parseRoot->children[1],root);
			addr->link->link = constructAstTree(parseRoot->children[2],root);

			root->child = addr;
			return root;
		}
		else if(tnt.nonTerm == booleanExpression)
		{

			root = newNode(parseRoot);
			root->parent = parent;
			root->startscope = parent->startscope;
			root->endscope = parent->endscope;
			
			if( parseRoot->children[0]->gram.type ==TERMINAL &&  parseRoot->children[0]->gram.value.Term == OP)
			{
				addr = constructAstTree(parseRoot->children[1],root);
				addr->link = constructAstTree(parseRoot->children[3],root);
				addr->link->link = constructAstTree(parseRoot->children[5],root);

			}
			else if( parseRoot->children[0]->gram.type ==TERMINAL &&  parseRoot->children[0]->gram.value.Term == NOT)
			{
				addr = constructAstTree(parseRoot->children[0],root);
				addr->link = constructAstTree(parseRoot->children[2],root);

			}
			else
			{
				addr = constructAstTree(parseRoot->children[0],root);
				addr->link = constructAstTree(parseRoot->children[1],root);
				addr->link->link = constructAstTree(parseRoot->children[2],root);

			}
			root->child = addr;
			return root;


		}	
		else 
		{
			return constructAstTree(parseRoot->children[0],parent);
		}

	}
	else
	{
		root = newNode(parseRoot);
		root->parent = parent;
		root->startscope = parent->startscope;
		root->endscope = parent->endscope;

		if(root->gram.value.Term == INT)
		{
			parent->datatype = TYPE_I;
		}
		else if(root->gram.value.Term == REAL)
		{
			parent->datatype = TYPE_R;

		}
		else if(root->gram.value.Term == STRING)
		{
			parent->datatype = TYPE_S;
		}
		else if(root->gram.value.Term == MATRIX)
		{
			parent->datatype = TYPE_M;
		}
		else{}


		if(root->gram.value.Term == ID)
		{
			root->datatype = parent->datatype;
		
		}

		return root;
		
	}	
	
}

void printNode(AstNode * root)
{


	if(root->gram.type ==TERMINAL)
	{
			
		printf("lexeme: %s lineno: %d token: %s parent: %s isLeafNode: yes Node: %s startscope: %d  endscope: %d datatype: %d \n",root->token->value,root->token->lineNo,
			root->token->symbol,stNonTerm[root->parent->gram.value.nonTerm],stTerm[root->gram.value.Term],root->startscope,root->endscope,root->datatype);

	}	
		
	else
	{

		//printf("%d\n",root->gram.value.nonTerm);

		if(root->parent == NULL)
		{

			printf("lexeme: --- lineno: --- token: --- parent: null isLeafNode: no Node: %s startscope: %d  endscope: %d\n",stNonTerm[root->gram.value.nonTerm],root->startscope,root->endscope);

		}
		else
		{

			printf("lexeme: --- lineno: --- token: --- parent: %s isLeafNode: no Node: %s startscope: %d  endscope: %d\n",stNonTerm[root->parent->gram.value.nonTerm],stNonTerm[root->gram.value.nonTerm],root->startscope,root->endscope);

		}	

	}		
	
}


void printAst(AstNode * root)
{
	if(root==NULL)
		return;

	printAst(root->child);
	printNode(root);
	AstNode * temp = root->child;

	if(temp != NULL)
	{
		temp= temp->link;
		while(temp!=NULL)
		{
			printAst(temp);
			temp= temp->link;
		}	
	}

}

void printAstCount()
{
	printf("AST Number of nodes:%d\t",astcount);

	printf("Allocated Memory:%lu Bytes\n\n", astcount*sizeof(AstNode));
	float per = (float)(parsercount*sizeof(ParseTreeNode)-astcount*sizeof(AstNode))/(astcount*sizeof(AstNode));
	per = per*100; 
	printf("Compression percentage: %f\n",per);

}

/*
int main()
{

	LoadGrammar("mygrammar.txt");


	computeFirst();
	//printFirst();
	computeFollow();
	//printFollow();
	computeParseTable();
	//printParseTable();

	
	ParseTreeNode * parseRoot;
	parseRoot = parseInputSourceCode("testcase1.txt");
	printParseTree(parseRoot,"output.txt");
	printf("\n\n\n");
	printParseCount();	
	printf("\n\n\n");
	AstNode * root;
	root = constructAstTree(parseRoot,NULL);
	printAst(root);
	printf("\n\n\n");
	printAstCount();

	
	return 0;

}
*/


