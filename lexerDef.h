//2014B4A70867P Karan Makhija


#ifndef LEXERDEF_H
#define LEXERDEF_H
#include <stdio.h>

#define MAX_BUFFER 10
#define MAX_LEXEME 20
#define MAX_INTEGER 9


typedef struct _tokenInfo
{
	char value[MAX_LEXEME];
	char symbol[MAX_LEXEME];
	int lineNo;
}tokenInfo;


#define MAX_HASH 11


struct list
{
	tokenInfo *token;
	struct list *next;
};

typedef struct list tokenlist;


typedef struct 
{
	tokenlist * list[MAX_HASH];

}HashTable;
#endif