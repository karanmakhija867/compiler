//2014B4A70867P

#ifndef SYMBOLDEF_H
#define SYMBOLDEF_H

#include "astDef.h"
#include <stdio.h>

#define BUCK_SIZE 11

struct HashBucket
{
	char lexeme[20];
	int offset;
	int size;
	int dtype;
	int lineno;
	struct HashBucket * next;

};

typedef struct HashBucket HashBucket;


struct HashTree
{
	HashBucket ** ptr;
	char func[20];
	int sscope;
	int	escope; 
	AstNode * inode;
	AstNode * onode;
	struct HashTree * child;
	struct HashTree * link;
	struct HashTree * parent;
};

typedef struct HashTree HashTree;

#endif