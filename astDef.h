//2014B4A70867P Karan Makhija

#ifndef ASTDEF_H
#define ASTDEF_H

#include "parserDef.h"
#include <stdio.h>


#define TYPE_I 0
#define TYPE_R 1
#define TYPE_S 2
#define TYPE_M 3


struct AstNode{

	Node gram;
	tokenInfo *token;
	int startscope;
	int endscope;
	int datatype;
	struct AstNode * parent;
	struct AstNode * child;
	struct AstNode * link;
	int assigned;

};

typedef struct AstNode AstNode;

#endif