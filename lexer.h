//2014B4A70867P Karan Makhija


#ifndef LEXER_H
#define LEXER_H

#include <stdio.h>
#include "lexerDef.h"

// Returns hash value for keyword
int hashFunction(char *key);

// Initialises the global hashTable
void initialiseHash();

//Inserts keyword in the hashTable
void insertitem(char * keyword);


//Inserts all the keywords
void insertEntries();

//Searches for the keyword. If found returns the token
tokenInfo * searchKey(char * keyword);

//Intialises two buffer
void getStreamInitial(FILE *fp);

//gets fixed size source code into the memory
// input is source code file returns updated file pointer
// flag 1 for buffer1 else for buffer2
void getStream(FILE *fp,int flag);


//Gets the next character from buffer
char getNextCharacter(FILE *fp);


//retracts the global pointer one step
void retract();

//recognizes lexemes, tokenizes lexemes, 
//returns relevant info encapsulated as tokenInfo
tokenInfo *getNextToken();

//Prints list of tokens
void printListOfTokens(char * filename);

//Test function
void test1(char *filename);


//removes Comments from testcaseFile and writes to cleanFile
void removeComments(char *testcaseFile);

#endif