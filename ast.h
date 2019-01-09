//2014B4A70867P Karan Makhija


#ifndef AST_H
#define AST_H

#include "parser.h"
#include "astDef.h"
#include <stdio.h>


AstNode * newNode(ParseTreeNode * root);

AstNode * constructAstTree(ParseTreeNode * parseRoot,AstNode * parent);

void printNode(AstNode * root);

void printAst(AstNode * root);


void printAstCount();


#endif