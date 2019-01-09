//2014B4A70867P Karan Makhija
#ifndef SYMBOL_H
#define SYMBOL_H

#include "ast.h"
#include "symboltableDef.h"
#include <stdio.h>

HashBucket** createHashTable(void);

HashTree * createHashTreeNode(AstNode * astRoot,HashTree * parent);

HashBucket * createHashBucket(AstNode * astRoot);

HashTree * insertEntry(AstNode * astRoot,HashTree * parent);

int searchHKey(HashTree * root,AstNode * node,int recursion);

HashBucket * findbuck(HashTree * root, AstNode * node,int recursion);

//Only add entries to symbol table does not do typecheching
HashTree * addEntriesAndTypeCheck(AstNode * astRoot, HashTree * parent);

void printSymboltable(HashTree * mainroot,int level);

void printscope(HashTree * root,int level);

#endif