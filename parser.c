//2014B4A70867P Karan Makhija
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <math.h>


#include "parser.h"

int parsercount = 0;
char stTerm[40][100] ={"AND",
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

char stNonTerm[45][100] =
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



tokenInfo * lastToken = NULL;




Node GrammarRules[numRules][maxRHS+1];

int convertTtoEnum(char * str)
{
	for(int i=0;i<numTerminal;i++)
	{
		if(strcmp(str,stTerm[i])==0)
			return i;
	}
	return -1;	
}

int convertNTtoEnum(char * str)
{
	for(int i=0;i<numNonTerminal;i++)
	{
		if(strcmp(str,stNonTerm[i])==0)
			return i;
	}
	return -1;	
} 

void LoadGrammar(char * filepath)
{
	FILE * fp = fopen(filepath,"r");
	if(fp == NULL)
	{
		fprintf(stderr, "Incorrect filename\n");
		exit(1);
	}
	char str[100];
	int i =0;
	int j =0;
	fscanf(fp,"%s",str);
	while(strcmp(str,"EOF")!=0)
	{
		//printf("%s\n",str);
		if(strcmp(str,"$$") == 0)
		{
			fscanf(fp,"%s",str);
			continue;

		}
		else if(strcmp(str,"@") == 0)
		{
			GrammarRules[i][0].numRHS = j-1;
			i++;
			j=0;
		}	
		else
		{
			int value = convertTtoEnum(str);

			if(value == -1)
			{

				value = convertNTtoEnum(str);
				if(value == -1)
				{
					printf("%s\n",str);
					printf("Invalid Grammar Symbol\n");
					exit(0);
				}
				else
				{
					GrammarRules[i][j].type = NONTERMINAL;
					GrammarRules[i][j].value.nonTerm = value;
					j++;
				}

			}
			else
			{
				GrammarRules[i][j].type = TERMINAL;
				GrammarRules[i][j].value.Term = value;
				j++;
			}	


		}

		fscanf(fp,"%s",str);	
		
	}
	fclose(fp);

}

unsigned long long int First[numNonTerminal];
int visitedFirst[numNonTerminal];

unsigned long long int Follow[numNonTerminal];
int visitedFollow[numNonTerminal];


int ParseTable[numNonTerminal][numTerminal];

void printBinary(unsigned long long int n)
{
	while (n) {
    if (n & 1)
        printf("1");
    else
        printf("0");

   	 	n >>= 1;
	}
	printf("\n");
}

void helperFirst(int next_nt)
{

	if(visitedFirst[next_nt]==1)
		return;
	int i,j;
	int flag = 0;
	//long long for checking epsilon
	unsigned long long int check =1;
	unsigned long long int one =1;
	check = check << 9;
	for(i=0;i<numRules;i++)
	{
		if(GrammarRules[i][0].value.nonTerm ==next_nt)
		{
			//printf("%d\n",i);

			//printf("abc\n");
			for(j=1;j<=GrammarRules[i][0].numRHS;j++)
			{
				if(GrammarRules[i][j].type == TERMINAL)
				{	
					First[next_nt] |= one<<GrammarRules[i][j].value.Term;

					//if(i==4)
					//	printf("%d%d",GrammarRules[i][j].numRHS,GrammarRules[i][j].value.Term);

					if(GrammarRules[i][0].numRHS ==1 && GrammarRules[i][j].value.Term == 9)
						flag =1;

					break;
				}
				else
				{
					int temp = GrammarRules[i][j].value.nonTerm;
					helperFirst(temp);
					//printf("%llu",First[next_nt]);
					First[next_nt] |= First[temp];
					
					if(First[temp] & check)
					{
						continue;
					}
					else
					{
						break;
					}	



				}
				

			}
			//printf("%llu\n",First[next_nt]);
			

		}	
	}
	//printf("%d\n",flag);
	if((First[next_nt] & check ) && flag==0)
	{
		First[next_nt] &= ~(one<<9);
	}
	visitedFirst[next_nt] = 1;
}




void computeFirst()
{

	memset(visitedFirst,0,sizeof(visitedFirst));
	memset(First,0,sizeof(First));
	for(int i=0;i<numNonTerminal;i++)
		helperFirst(i);	
	//helperFirst(24);
}


void printFirst()
{
	int i;
	int j;
	for(i=0;i<numNonTerminal;i++)
	{	
		printf("%s:",stNonTerm[i]);
		for(j=0;j<numTerminal;j++)
		{
			unsigned long long int print_temp = 1;
			print_temp = print_temp<<j;
			if(print_temp & First[i])
			{
				printf("%s ",stTerm[j]);
			}
		}	
		printf("\n");
	}
	printf("\n");	
}


void helperFollow(int next_nt)
{
	if(visitedFollow[next_nt] == 1)
		return;

	int i,j,k;
	unsigned long long int one =1;
	int follow_symbol;
	int follow_index;

	Nonterminal temp = mainFunction;
	if(temp == next_nt)
	{
		//printf("%d",next_nt);
		Follow[next_nt] |= one<<5;
		//printBinary(Follow[next_nt]);

	}
	for(i=1;i<numRules;i++)
	{
		for(j=1;j<=GrammarRules[i][0].numRHS;j++)
		{
			if(GrammarRules[i][j].type == NONTERMINAL && GrammarRules[i][j].value.nonTerm == next_nt)
			{
				for(k=j;k<=GrammarRules[i][0].numRHS;k++)
				{	
					follow_index = k+1;
					
					if(follow_index > GrammarRules[i][0].numRHS)
					{
						if(GrammarRules[i][0].value.nonTerm != next_nt)
						{
							follow_symbol =  GrammarRules[i][0].value.nonTerm;
							helperFollow(follow_symbol);
							Follow[next_nt]|= Follow[follow_symbol];
						}
						break;	
					}
					else
					{
						if(GrammarRules[i][follow_index].type == NONTERMINAL)
						{
							follow_symbol = GrammarRules[i][follow_index].value.nonTerm;
							
							if( !(First[follow_symbol] & one<<9))
							{	
								Follow[next_nt] |= First[follow_symbol];
								break;
							}
							else
							{
								Follow[next_nt] |= First[follow_symbol];
								Follow[next_nt] &= ~(one<<9);

							}	

						}
						else
						{
							follow_symbol = GrammarRules[i][follow_index].value.Term;
							if(follow_symbol!= 9)
							{		
								Follow[next_nt] |= one<<follow_symbol;
								break;
							}

						}	
					}
				}	
			}

		}
		
		
	
	}
	visitedFollow[next_nt] =1;	

}





void computeFollow()
{
	memset(visitedFollow,0,sizeof(visitedFollow));
	memset(Follow,0,sizeof(Follow));

	Nonterminal temp = start;
	int i;
	for(i=0;i<numNonTerminal;i++)
	{
		if(i== temp)
			continue;
		else
			helperFollow(i);
	}	

}

void printFollow()
{
	int i;
	int j;
	for(i=0;i<numNonTerminal;i++)
	{	
		printf("%s:",stNonTerm[i]);
		for(j=0;j<numTerminal;j++)
		{
			unsigned long long int print_temp = 1;
			print_temp = print_temp<<j;
			if(print_temp & Follow[i])
			{
				printf("%s ",stTerm[j]);
			}
		}	
		printf("\n");
	}
	printf("\n");	
}



void computeParseTable()
{
	memset(ParseTable,-1,sizeof(ParseTable));
	int i,j,k,l;
	for(i=0;i<numNonTerminal;i++)
	{
		for(j=0;j<numRules;j++)
		{
			if(GrammarRules[j][0].value.nonTerm == i)
			{
						
				if(GrammarRules[j][1].type== TERMINAL && GrammarRules[j][1].value.Term == EPSILON)
				{
					for(l=0;l<numTerminal;l++)
					{
						unsigned long long int follow_temp = 1;
						follow_temp = follow_temp<<l;
						if(follow_temp & Follow[i])
						{
							if(ParseTable[i][k]!= -1)
								printf("Errorfollow\n");
							
							ParseTable[i][l] = j;
						}	
					}
						
				}
				else if(GrammarRules[j][1].type == TERMINAL)
				{
					int value = GrammarRules[j][1].value.Term;
					if(ParseTable[i][value]!= -1)
						printf("Errorterminal\n");
					ParseTable[i][value] = j;
					
				}
				else
				{
					int value = GrammarRules[j][1].value.nonTerm;
					/*if(GrammarRules[j][0].value.nonTerm == start)
					{
						printf("%d\n",value);
					}*/
					unsigned long long int first_temp = 1;
						
					for(k=0;k<numTerminal;k++)
					{
						first_temp = first_temp<<k;
						
						if(first_temp & First[value] )
						{
							if(ParseTable[i][k]!= -1)
								printf("Errorfirst%d%d\n",i,k);
							ParseTable[i][k] = j;
						}
						first_temp =1;

					}
					/*
					

					if(first_temp<<9 &&First[value])
					{
						for(l=0;l<numTerminal;l++)
						{
							unsigned long long int follow_temp = 1;
							follow_temp = follow_temp<<l;
							if(follow_temp & Follow[value])
							{
								if(ParseTable[i][k]!= -1)
									printf("Errorfollow2\n");
								
								ParseTable[i][l] = j;
							}	
						}
					}
					*/
					

				}
	

			}	
		}	
	}	

}

void printParseTable()
{
	int i , j;
	printf("**************************************\n");
	for(i=0;i<numNonTerminal;i++)
	{
		printf("For %s :\n",stNonTerm[i]);
		for(j=0;j<numTerminal;j++)
		{
			if(ParseTable[i][j]!=-1)
				printf("%s-->%d\n",stTerm[j],ParseTable[i][j]);
		}
		
		printf("**************************************\n");
	}

}


void intiliseLastToken()
{
	lastToken = (tokenInfo *)malloc(sizeof(tokenInfo));
	strcpy(lastToken->value,"$");
	strcpy(lastToken->symbol,"DOLLAR");
	lastToken->lineNo = -1;
}

ParseTreeNode * createChildren(ParseTreeNode * root,int rule)
{
	//printf("%d\n",rule);
	
	root->childCount = GrammarRules[rule][0].numRHS;
	root->children = (ParseTreeNode **)malloc(root->childCount*sizeof(ParseTreeNode*));

	parsercount += root->childCount;


	for(int i=1;i<=GrammarRules[rule][0].numRHS;i++)
	{ 
		ParseTreeNode * temp =(ParseTreeNode *)malloc(sizeof(ParseTreeNode));
		temp->gram = GrammarRules[rule][i];
		temp->token = NULL;
		temp->children = NULL;
		temp->childCount = 0;
		temp->parent = root;
		root->children[i-1] = temp;
	}
	return root;
}


ParseTreeNode * constructParseTree(ParseTreeNode * root,tokenInfo ** token,FILE * fp,int * errorflag)
{
	
	if(*token == NULL)
	{
		*token = getNextToken(fp);
		if(*token == NULL)
		{
			*token = lastToken;
			if(*token==NULL)
				return root;

		}	

	}

	/*if(root->gram.type ==TERMINAL)
		printf("%s\n",stTerm[root->gram.value.Term]);
	else
		printf("%s\n",stNonTerm[root->gram.value.nonTerm]);
	*/
		
	if(root->gram.type == NONTERMINAL)
	{

		int j = convertTtoEnum((*token)->symbol);
		int i = root->gram.value.nonTerm;
		if(ParseTable[i][j]== -1)
		{

			*errorflag = 1;

			fprintf(stderr, "%d Syntactic error : %s This token cannot be derived with any rule\n",(*token)->lineNo,(*token)->symbol);
			
			*token = getNextToken(fp);
				
			while(*token !=NULL)
			{	

				int tempsym = convertTtoEnum((*token)->symbol);
				unsigned long long int temp =1;
				for(int k=0;k<numTerminal;k++)
				{
						if(Follow[i] && temp<<k)
							return root;

				}
				*token = getNextToken(fp);

			}	


			if(*token == NULL)
			{
				fprintf(stderr, "Syntactic error :Too many errors. Cannot be corrected \n");
			
				exit(0);

			}
			
	
		}
		else
		{	
			int rule = ParseTable[i][j];
			root = createChildren(root,rule);
			
		}
		
	}
	else
	{
		if(root->gram.value.Term == convertTtoEnum((*token)->symbol))
		{
			//printf("%s\n",(*token)->value);	
			root->token = *token;
			*token = NULL;
			
		}
		else if(root->gram.value.Term == EPSILON)
		{

		}
		else
		{
			//printf("%s %s",stTerm[root->gram.value.Term],(*token)->value);
			

			*errorflag = 1;

			while(root->gram.value.Term != convertTtoEnum((*token)->symbol))
			{


				fprintf(stderr, "%d Syntactic error : %s This token not matching derived terminal\n",(*token)->lineNo,(*token)->symbol);
				
				*token = getNextToken(fp);
				if(*token == NULL)
				{
					fprintf(stderr, "Syntactic error :Too many errors. Cannot be corrected \n");
				
					exit(0);

				}	

			}
			root->token = *token;
			*token = NULL;
			

		}

			
	}



	/*if(root->gram.type ==TERMINAL)
		printf("%s\n",stTerm[root->gram.value.Term]);
	else
		printf("%s\n",stNonTerm[root->gram.value.nonTerm]);
	*/

	for(int i=0;i<root->childCount;i++)
		root->children[i] = constructParseTree(root->children[i],token,fp,errorflag);
	
	return root;	
	
}


void recprintParseTree(ParseTreeNode * root,FILE * fp)
{

	
	if(root->children !=NULL)
		recprintParseTree(root->children[0],fp);

	if(root->gram.type ==TERMINAL)
	{
		if(root->token!=NULL)
		{
			
			fprintf(fp,"lexeme: %s lineno:%d token: %s parent: %s isLeafNode: yes Node: %s\n",root->token->value,root->token->lineNo,root->token->symbol,stNonTerm[root->parent->gram.value.nonTerm],stTerm[root->gram.value.Term]);

		}
		else
		{
			fprintf(fp,"lexeme: --- lineno: --- token: --- parent:%s isLeafNode: yes Node: %s\n",stNonTerm[root->parent->gram.value.nonTerm],stTerm[root->gram.value.Term]);


		}
	
	}	
		
	else
	{
		if(root->parent == NULL)
		{

			fprintf(fp,"lexeme: --- lineno: --- token: --- parent: null isLeafNode: no Node: %s\n",stNonTerm[root->gram.value.nonTerm]);

		}
		else
		{

			fprintf(fp,"lexeme: --- lineno: --- token: --- parent: %s isLeafNode: no Node: %s\n",stNonTerm[root->parent->gram.value.nonTerm],stNonTerm[root->gram.value.nonTerm]);

		}	

	}	
		//printf("%s\n",stNonTerm[root->gram.value.nonTerm]);

	
	for(int i=1;i<root->childCount;i++)
		recprintParseTree(root->children[i],fp);		


}

void printParseTree(ParseTreeNode * root,char *outputFile)
{
	FILE * fp = fopen(outputFile,"w");
	fp = stderr;
	recprintParseTree(root,fp);
	fclose(fp);
}


ParseTreeNode * parseInputSourceCode(char *testcaseFile,int * flag)
{
	ParseTreeNode * root = (ParseTreeNode *)malloc(sizeof(ParseTreeNode));
	root->gram.type = NONTERMINAL;
	root->gram.value.nonTerm = start;
	root->parent = NULL;
	root->childCount =0;
	root->children = NULL;
	root->token = NULL;
	parsercount++;

	//int errorflag =0;

	initialiseHash();
	insertEntries();
	intiliseLastToken();

	FILE * fp = fopen(testcaseFile,"r");
	getStreamInitial(fp);
	tokenInfo * token = getNextToken(fp);
	//printf("%s",token->value);
	root = constructParseTree(root,&token,fp,flag);

	//printf("%d",root->left->gram.type);
	//printParseTree(root);
	fclose(fp);
	
	return root;
	
}

void printParseCount()
{
	printf("Parse tree Number of nodes:%d\t",parsercount);
	printf("Allocated Memory:%lu Bytes\n\n",parsercount*sizeof(ParseTreeNode));
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
	printParseTable();

	ParseTreeNode * root = parseInputSourceCode("testcases/testcase4.txt");
	printParseTree(root);
	
	return 0;

}
*/


