//2014B4A70867P Karan Makhija

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <math.h>

#include "symboltable.h"

char typeinttostr[4][40]={"integer","real","string","matrix"};


int globaloffset = 0;

HashBucket** createHashTable(void)
{
	HashBucket ** hashTable = (HashBucket **)malloc(sizeof(HashBucket *)*BUCK_SIZE);
	for (int i = 0; i < BUCK_SIZE; ++i)
	{
		hashTable[i] = NULL;
	}
	return hashTable;

}

HashTree * createHashTreeNode(AstNode * astRoot,HashTree * parent)
{

	HashTree * root = (HashTree *)malloc(sizeof(HashTree));
	strcpy(root->func,astRoot->child->link->link->token->value);
	root->sscope = astRoot->startscope;
	root->escope = astRoot->endscope;
	root->onode = astRoot->child;
	root->inode = astRoot->child->link->link->link;
	root->child = NULL;
	root->parent = parent;
	root->link = NULL;
	root->ptr = createHashTable();


	AstNode * temp = root->inode->child;
	if(temp!=NULL)
	{
		root = insertEntry(temp->link,root);
		temp= temp->link->link;
		while(temp!=NULL)
		{
			root = insertEntry(temp->link,root);
			temp = temp->link->link;
		
		}

	}

	temp = root->onode->child;
	if(temp!=NULL)
	{
		root = insertEntry(temp->link,root);
		temp= temp->link->link;
		while(temp!=NULL)
		{
			root = insertEntry(temp->link,root);
			temp = temp->link->link;
		
		}

	}

	return root;

}


HashBucket * createHashBucket(AstNode * astRoot)
{

	HashBucket * root = (HashBucket *)malloc(sizeof(HashBucket));
	root->next = NULL;
	strcpy(root->lexeme,astRoot->token->value);
	root->dtype = astRoot->datatype;
	root->lineno = astRoot->token->lineNo;

	if(root->dtype == 0)
	{
		root->size = 2;
		root->offset = globaloffset;
		globaloffset +=2;
	}	
	else if(root->dtype ==1)
	{
		root->size = 4;
		root->offset = globaloffset;
		globaloffset +=4;

	}	
	else if(root->dtype ==2)
	{
		root->size = 20;
		root->offset = globaloffset;
		globaloffset +=20;

	}	
	else
	{
		root->size = -1;
		root->offset = -1;

	}	
	return root;

}


HashTree * insertEntry(AstNode * astRoot,HashTree * parent)
{
	HashBucket * temp;
	HashBucket * addr = createHashBucket(astRoot); 
	int index = hashFunction(astRoot->token->value);
	
	
	if((parent->ptr)[index] == NULL)
		(parent->ptr)[index] = addr;
	else
	{
		temp = (parent->ptr)[index];
		while(temp->next!=NULL)
			temp = temp->next;
		temp->next = addr;
	}
	return parent;	
 
}


//Check Hash Table along with all parents, as well as input and output parameter lists of functions 
int searchHKey(HashTree * root, AstNode * node,int recursion)
{
	if(root==NULL)
		return 0;

	//printf("%s\n",root->func);

	int index = hashFunction(node->token->value);
	HashBucket * hash = (root->ptr)[index];

	while(hash!=NULL)
	{	
		//printf("%s\n",node->token->value);
		//printf("s:%d\n",strcmp(hash->lexeme,node->token->value)==0);
		//printf("l:%d\n",(node->token->lineNo >= hash->lineno));
			

		if(strcmp(hash->lexeme,node->token->value)==0 && (node->token->lineNo >= hash->lineno))
			return 1;

		hash = hash->next;

	}

	if(recursion==1)
	{
		return searchHKey(root->parent,node,1);
	}	

	return 0;
}


HashBucket * findbuck(HashTree * root, AstNode * node,int recursion)
{
	if(root==NULL)
		return NULL;

	int index = hashFunction(node->token->value);
	HashBucket * hash = (root->ptr)[index];

	while(hash!=NULL)
	{	
		if(strcmp(hash->lexeme,node->token->value)==0)
			return hash;

		hash = hash->next;

	}

	if(recursion==1)
	{
		return findbuck(root->parent,node,1);
	}	

	return NULL;
}



HashTree * addEntriesAndTypeCheck(AstNode * astRoot, HashTree * parent)
{

	if(astRoot == NULL)
		return parent;

	AstNode * temp;
	HashTree * hashtemp;
	if(astRoot->gram.type == NONTERMINAL && astRoot->gram.value.nonTerm == start)
	{
		parent->sscope = astRoot->startscope;
		parent->escope = astRoot->endscope;
		
		temp = astRoot->child;

		while(temp!=NULL)
		{
			parent = addEntriesAndTypeCheck(temp,parent);
			temp = temp->link;
		}
		return parent;

	}
	else if(astRoot->gram.type == NONTERMINAL && astRoot->gram.value.nonTerm == functionDef)
	{
		if(parent->child == NULL)
		{
			parent->child = createHashTreeNode(astRoot,parent);
			parent->child = addEntriesAndTypeCheck(astRoot->child->link->link->link->link,parent->child);
			return parent;
		}
		hashtemp = parent->child;

		while(hashtemp->link!=NULL)
		{
			hashtemp = hashtemp->link;
		}
		temp = astRoot->child->link->link->link->link;
		hashtemp->link = createHashTreeNode(astRoot,parent);
		while(temp!=NULL)
		{
			hashtemp->link = addEntriesAndTypeCheck(temp,hashtemp->link);
			temp= temp->link;		
		}	
		return parent;
	}
	else if(astRoot->gram.type == NONTERMINAL && astRoot->gram.value.nonTerm == declarationStmt)
	{
		temp = astRoot->child->link->child;

		while(temp != NULL)
		{
			parent = insertEntry(temp,parent);
			temp = temp->link;
		}	
		return parent;
	}
	else
	{
		temp = astRoot->child;
		while(temp!=NULL)
		{
			parent = addEntriesAndTypeCheck(temp,parent);
			temp = temp->link;
		}
		return parent;

	}


}



void printscope(HashTree * root,int level)
{
	HashBucket * buck;
	for(int i=0;i<BUCK_SIZE;i++)
	{
		buck = (root->ptr)[i];
		while(buck!=NULL)
		{
			if(root->parent!=NULL)
				printf("id_name= %s scope: %s nesting_level: %d parentscope: %s type: %s width: %d offset: %d lineno: %d\n",
					buck->lexeme,root->func,level,root->parent->func,typeinttostr[buck->dtype],buck->size,buck->offset,buck->lineno);
			else
				printf("id_name= %s scope: %s nesting_level: %d parentscope:---- type: %s width: %d offset: %d lineno: %d\n",
					buck->lexeme,root->func,level,typeinttostr[buck->dtype],buck->size,buck->offset,buck->lineno);
			buck = buck->next;
		}	

	}	
}




void printSymboltable(HashTree * mainroot,int level)
{
	if(mainroot ==NULL)
		return;

	printscope(mainroot,level);

	printSymboltable(mainroot->child,level+1);

	HashTree * temp = mainroot->child;
	
	while(temp!=NULL)
	{
		printSymboltable(temp->link,level+1);
		temp = temp->link;
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
	int error = 0;
	parseRoot = parseInputSourceCode("testcase5.txt",&error);
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


	return 0;

}
*/
