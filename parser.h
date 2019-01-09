//2014B4A70867P Karan Makhija

#ifndef PARSER_H
#define PARSER_H

#include "lexer.h"
#include "parserDef.h"
#include <stdio.h>


//Converts Terminal string to enum
int convertTtoEnum(char * str);

//Converts Nonterminal string to enum
int convertNTtoEnum(char * str);


//Loads the grammar from filepath
void LoadGrammar(char * filepath);


//Print the number in binary
void printBinary(unsigned long long int n);

//helper function to calculate first
void helperFirst(int next_nt);

//Computes First set
void computeFirst();

//Prints first
void printFirst();


//helper function to calculate follow
void helperFollow(int next_nt);


//Computes Follow set
void computeFollow();


//Prints follow
void printFollow();

//Computes Parse Table
void computeParseTable();

//Prints Parse Table
void printParseTable();


//Initiliase the last token with Dollar
void intiliseLastToken();

//Creates children of root for the given rule
ParseTreeNode * createChildren(ParseTreeNode * root,int rule);

//Constructs parse tree 
ParseTreeNode * constructParseTree(ParseTreeNode * root,tokenInfo ** token,FILE *fp,int * errorflag);

//Opens the testcaseFile and initialise root with start
ParseTreeNode * parseInputSourceCode(char *testcaseFile,int * flag);

//Print the parse tree in the outputFile
void recprintParseTree(ParseTreeNode * root,FILE * fp);

//Opens file and wrapper for the main recursive function recprintParseTree 
void printParseTree(ParseTreeNode * root,char * outputFile);

void printParseCount();

#endif

