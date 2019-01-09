//2014B4A70867P Karan Makhija
//Typechecking for not declaring variables, for arithmetic expressions 
//and for assignment of output parameters implemented

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <math.h>

#include "symboltable.h"



HashTree * typeCheck(AstNode * astRoot, HashTree * parent)
{
	if(astRoot == NULL)
		return parent;


	AstNode * temp;
	AstNode * left;
	AstNode * right;
	HashTree * hashtemp;
	HashBucket * bucktemp;
	if(astRoot->gram.type == NONTERMINAL && astRoot->gram.value.nonTerm == start)
	{
		temp = astRoot->child;

		while(temp!=NULL)
		{
			parent = typeCheck(temp,parent);
			temp = temp->link;
		}
		return parent;

	}
	if(astRoot->gram.type == NONTERMINAL && astRoot->gram.value.nonTerm == functionDef)
	{
		hashtemp = parent->child;

		while(strcmp(hashtemp->func,astRoot->child->link->link->token->value)!=0)
			hashtemp = hashtemp->link;

		temp = astRoot->child->link->link->link->link;
		while(temp!=NULL)
		{
			hashtemp = typeCheck(temp,hashtemp);
			temp= temp->link;		
		}
		return parent;
	}
	else if(astRoot->gram.type == TERMINAL && astRoot->gram.value.Term == ID)
	{

		int x = searchHKey(parent,astRoot,1);
		if(x==0)
		{
			printf("Line:%d identifier %s not declared\n",astRoot->token->lineNo,astRoot->token->value);
			
		}
		else
		{
			bucktemp = findbuck(parent,astRoot,1);
			astRoot->datatype = bucktemp->dtype;

		}
		return parent;	

	}
	else if(astRoot->gram.type == NONTERMINAL && astRoot->gram.value.nonTerm == assginmentStmtType1)
	{
		left = astRoot->child;
		right = astRoot->child->link->link;
		parent = typeCheck(left,parent);
		parent = typeCheck(right,parent);
		if(left->datatype == -1)
		{
			left->datatype = right->datatype;
		}
		else if(left->datatype != right->datatype)
		{
			printf("Line:%d type mismatch\n",left->token->lineNo);
		}
		if(strcmp(parent->func,"_main")!=0)
		{
			temp = parent->onode->child;
			while(temp!=NULL)
			{
				if(strcmp(temp->token->value,left->token->value)==0)
					temp->assigned = 1;

				temp = temp->link; 
			}

		}

		return parent;	

	}
	else if(astRoot->gram.type == TERMINAL && astRoot->gram.value.Term == END)
	{
		temp = parent->onode->child;
		while(temp!=NULL)
		{
			if(temp->link->assigned==0)
				printf("Line:%d function parameter %s is not assigned\n",temp->link->token->lineNo,temp->link->token->value);

			temp = temp->link->link;

		}
		//parent = parent->parent;
		return parent;

	}
	else if(astRoot->gram.type == NONTERMINAL && (astRoot->gram.value.nonTerm == arithmeticExpression || astRoot->gram.value.nonTerm == arithmeticTerm))
	{
		left = astRoot->child;
		parent = typeCheck(left,parent);
		//printf("%d\n",left->datatype);
		if(astRoot->child->link!=NULL)
		{
			right = astRoot->child->link->link;
			parent = typeCheck(right,parent);
			if(right->datatype==-1)
			{
				astRoot->datatype = left->datatype;
				return parent;
			}
			else if(left->datatype==-1)
			{
				astRoot->datatype = right->datatype;
				return parent;
			}	
			else if(left->datatype != right->datatype)
			{
				printf("Line:%d type mismatch\n",left->link->token->lineNo);
				
			}
			
		}
		astRoot->datatype = left->datatype;
		return parent;
	}
	else if(astRoot->gram.type == NONTERMINAL && astRoot->gram.value.nonTerm == var)
	{
		left = astRoot->child;
		parent = typeCheck(left,parent);

		if(left->link != NULL)
		{
			astRoot->datatype = 0;
		}
		else
		{
			astRoot->datatype = left->datatype;
		}
		return parent;

	}
	else if(astRoot->gram.type == TERMINAL && astRoot->gram.value.Term == NUM)
	{
		astRoot->datatype = 0; return parent;
	}	
	else if(astRoot->gram.type == TERMINAL && astRoot->gram.value.Term == RNUM)
	{
		astRoot->datatype = 1; return parent;
	}		
	else if(astRoot->gram.type == TERMINAL && astRoot->gram.value.Term == STR)
	{
		astRoot->datatype = 2; return parent;
	}	
	else if(astRoot->gram.type == NONTERMINAL && astRoot->gram.value.nonTerm == matrix)
	{
		astRoot->datatype = 3; return parent;
	}
	else
	{
		temp = astRoot->child;
		while(temp!=NULL)
		{
			parent = typeCheck(temp,parent);
			temp = temp->link;
		}
		return parent;

	}
}


/*int main()
{
	
	LoadGrammar("mygrammar.txt");


	computeFirst();
	//printFirst();
	computeFollow();
	//printFollow();
	computeParseTable();
	//printParseTable();

	ParseTreeNode * parseRoot;
	int errorflag =0;
	parseRoot = parseInputSourceCode("testcase5.txt",&errorflag);
	printParseTree(parseRoot,"output.txt");
	printf("\n\n\n");
	printParseCount();	
	printf("\n\n\n");
	AstNode * astRoot;
	astRoot = constructAstTree(parseRoot,NULL);
	printAst(astRoot);
	printf("\n\n\n");
	printAstCount();
	

	HashTree * mainroot = (HashTree *)malloc(sizeof(HashTree));
	strcpy(mainroot->func,"_main");
	mainroot->sscope = -1;
	mainroot->escope = -1;
	mainroot->inode = NULL;
	mainroot->onode = NULL;
	mainroot->child = NULL;
	mainroot->parent = NULL;
	mainroot->link = NULL;
	mainroot->ptr = createHashTable();

	mainroot = addEntriesAndTypeCheck(astRoot,mainroot);
	printSymboltable(mainroot,1);
	printf("\n\n\n");
	mainroot = typeCheck(astRoot,mainroot);



	return 0;

}
*/

