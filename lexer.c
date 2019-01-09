//2014B4A70867P Karan Makhija
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <math.h>

#include "lexer.h"



char buffer1[MAX_BUFFER];
char buffer2[MAX_BUFFER];
int bufcount1 = 0;
int bufcount2 = 0;

int line = 1;


HashTable ht;

int hashFunction(char *key)
{
    unsigned int hash; 
    int i;
    for(hash = i = 0; i < strlen(key); ++i)
    {
        hash += key[i];
        hash += (hash << 10);
        hash ^= (hash >> 6);
    }
    hash += (hash << 3);
    hash ^= (hash >> 11);
    hash += (hash << 15);

    return hash%MAX_HASH ;
}

void initialiseHash()
{
	for(int i=0;i<MAX_HASH;i++)
		ht.list[i] = NULL;	
}


void insertitem(char * keyword)
{
	tokenInfo *token = (tokenInfo *)malloc(sizeof(tokenInfo));
	strcpy(token->value,keyword);

	char temp2[20];
	for(int i=0;i<MAX_LEXEME;i++)
		temp2[i] =toupper(keyword[i]);

	strcpy(token->symbol,temp2);

	tokenlist *item =(tokenlist *)malloc(sizeof(tokenlist));
	item->next = NULL;
	item->token = token;

	int index = hashFunction(keyword);

	if(ht.list[index] == NULL)
		ht.list[index] = item;
	else
	{
		tokenlist * temp = ht.list[index];
		while(temp->next!=NULL)
			temp = temp->next;
		temp->next = item;

	}
}

void insertEntries()
{
	insertitem("end");
	insertitem("int");
	insertitem("real");
	insertitem("string");
	insertitem("matrix");
	insertitem("if");
	insertitem("else");
	insertitem("endif");
	insertitem("read");
	insertitem("print");
	insertitem("function");

}

tokenInfo * searchKey(char * keyword)
{
	//fflush(stdout);
	int index = hashFunction(keyword);
	//printf("%d\n",index);
		
	if(ht.list[index]==NULL)
		return NULL;
	else
	{
		tokenlist * temp = ht.list[index];
		while(temp!=NULL)
		{
			if(strcmp(temp->token->value,keyword)==0)
				return temp->token;
			temp = temp->next;
		}	

	}
	return NULL;
}

	
void getStreamInitial(FILE *fp)
{
	memset(buffer1,'0',sizeof(buffer1));
	memset(buffer2,'0',sizeof(buffer2));
	bufcount1 = 0;
	bufcount2 = 0;
	int count;
	count = fread(buffer1, 1, MAX_BUFFER, fp);
	//buffer1[MAX_BUFFER] ='\0';
	if(count < MAX_BUFFER) 
		buffer1[count] = EOF;
	else
	{

		count = fread(buffer2, 1, MAX_BUFFER, fp);
		//buffer2[MAX_BUFFER] ='\0';
		if(count < MAX_BUFFER) 
			buffer2[count] = EOF;
	}
	
}

void getStream(FILE *fp,int flag)
{
	int count;
	if(flag == 1)
	{
		memset(buffer1,0,sizeof(buffer1));
		count = fread(buffer1, 1, MAX_BUFFER, fp);
		
		//buffer1[MAX_BUFFER] ='\0';
		if(count < MAX_BUFFER) 
			buffer1[count] = EOF;
	}
	else
	{
		memset(buffer2,0,sizeof(buffer2));
		count = fread(buffer2, 1, MAX_BUFFER, fp);
		//buffer2[MAX_BUFFER] ='\0';
		if(count < MAX_BUFFER) 
			buffer2[count] = EOF;

	}

}
int retract_flag = 0;


char getNextCharacter(FILE *fp)
{
	
	char c;
	if(bufcount1 >= MAX_BUFFER)
	{
		c = buffer2[bufcount2];
		if(bufcount2 > MAX_BUFFER-2 && retract_flag == 0)
			getStream(fp,1);
		else
			retract_flag = 0;

		bufcount2++;
		if(bufcount2 == MAX_BUFFER)
			bufcount1 = 0;

	}
	
	else if(bufcount2 >= MAX_BUFFER)
	{
		c = buffer1[bufcount1];
		if(bufcount1 > MAX_BUFFER-2 && retract_flag == 0)
			getStream(fp,0);
		else
			retract_flag =0;

		bufcount1++;
		if(bufcount1 == MAX_BUFFER)
			bufcount2 = 0;

	}
	else
	{
		c = buffer1[bufcount1];
		bufcount1++;  
	}
	return c;

}


void retract()
{
	if(bufcount1 == MAX_BUFFER)
	{
		if(bufcount2 == 0)
		{
			bufcount1 = MAX_BUFFER-1;
			bufcount2 = MAX_BUFFER;
			retract_flag = 1;

		}
		else
		{
			bufcount2--;
		}

	}
	else if(bufcount2 == MAX_BUFFER)
	{
		if(bufcount1 == 0)
		{
			bufcount2 = MAX_BUFFER-1;
			bufcount1 = MAX_BUFFER;
			retract_flag =1;

		}
		else
		{
			bufcount1--;
		}

	}
	else
	{
		bufcount1--;
	}


}


//no need of forward pointer because in the worst case we are backtracking one character  
//This function will run till it finds a valid token
tokenInfo * getNextToken(FILE * fp)
{

	
	char lexeme[MAX_LEXEME];
	char token[MAX_LEXEME];
	
	tokenInfo* singleToken;
	tokenInfo * temp;
	singleToken = (tokenInfo*)malloc(sizeof(tokenInfo));
	
	int state =0;
	int lex_index;
	int int_index;
	
	char c;
	c = getNextCharacter(fp);

	while(1)
	{
 

		switch(state)
		{
			case 0:

				if(c== EOF)
					return NULL;
				memset(lexeme,'\0',MAX_LEXEME);
				memset(token,'\0',MAX_LEXEME);
				lex_index = 0;  
				int_index = 0;
				if(c==' ' || c=='\t' || c=='\r'|| c=='\0')
				{
					state =0;
					c = getNextCharacter(fp);
				}
				else if(c=='\n')
				{
					state = 0;
					line++;
					c = getNextCharacter(fp);
				}
				else if(c=='[')
				{
					state = 1;
					lexeme[lex_index] = c;
					lex_index++;
					
				}
				else if(c==']')
				{
					state = 2;
					lexeme[lex_index] = c;
					lex_index++;
					
				}
				else if(c=='(')
				{
					state = 3;
					lexeme[lex_index] = c;
					lex_index++;
					
				}
				else if(c==')')
				{
					state = 4;
					lexeme[lex_index] = c;
					lex_index++;
					
				}
				else if(c==';')
				{
					state = 5;
					lexeme[lex_index] = c;
					lex_index++;
					
				}
				else if(c==',')
				{
					state = 6;
					lexeme[lex_index] = c;
					lex_index++;
					
				}
				else if(c=='+')
				{
					state = 7;
					lexeme[lex_index] = c;
					lex_index++;
					
				}
				else if(c=='-')
				{
					state = 8;
					lexeme[lex_index] = c;
					lex_index++;
					
				}
				else if(c=='*')
				{
					state = 9;
					lexeme[lex_index] = c;
					lex_index++;
					
				}
				else if(c=='/')
				{
					state = 10;
					lexeme[lex_index] = c;
					lex_index++;
					
				}
				else if(c=='@')
				{
					state = 11;
					lexeme[lex_index] = c;
					lex_index++;
					
				}
				else if(c=='=')
				{
					state = 12;
					lexeme[lex_index] = c;
					lex_index++;
				}
				else if(c=='<')
				{
					state = 13;
					lexeme[lex_index] = c;
					lex_index++;
				}
				else if(c=='>')
				{
					state = 14;
					lexeme[lex_index] = c;
					lex_index++;
				}
				else if(c=='_')
				{
					state = 15;
					lexeme[lex_index] = c;
					lex_index++;
				}
				else if( (c>='a' && c<='z') || (c>='A' && c<='Z') )
				{
					state = 16;
					lexeme[lex_index] = c;
					lex_index++;
				}
				else if(c>='0' && c<='9')
				{
					state = 17;
					lexeme[lex_index] = c;
					lex_index++;
					int_index++;
				}
				else if(c=='\"')
				{
					state = 18;
					lexeme[lex_index] = c;
					lex_index++;
				}
				else if(c=='.')
				{
					state = 19;
					lexeme[lex_index] = c;
					lex_index++;
				}
				else if(c=='#')
				{
					state = 47;
					lexeme[lex_index] = c;
					lex_index++;

				}
				
				else
				{
					lexeme[lex_index] = c;
					state =0;
					c = getNextCharacter(fp);

					printf("%d: Lexical error: Unknown Symbol %s\n",line,lexeme);

				}
				break;

			case 1:
				strcpy(singleToken->value,lexeme);
				strcpy(singleToken->symbol,"SQO");
				singleToken->lineNo = line;
				return singleToken;

			case 2:
				strcpy(singleToken->value,lexeme);
				strcpy(singleToken->symbol,"SQC");
				singleToken->lineNo = line;
				return singleToken;

			case 3:
				strcpy(singleToken->value,lexeme);
				strcpy(singleToken->symbol,"OP");
				singleToken->lineNo = line;
				return singleToken; 

			case 4:
				strcpy(singleToken->value,lexeme);
				strcpy(singleToken->symbol,"CL");
				singleToken->lineNo = line;
				return singleToken; 

			case 5:
				strcpy(singleToken->value,lexeme);
				strcpy(singleToken->symbol,"SEMICOLON");
				singleToken->lineNo = line;
				return singleToken;

			case 6:
				strcpy(singleToken->value,lexeme);
				strcpy(singleToken->symbol,"COMMA");
				singleToken->lineNo = line;
				return singleToken;

			case 7:
				strcpy(singleToken->value,lexeme);
				strcpy(singleToken->symbol,"PLUS");
				singleToken->lineNo = line;
				return singleToken;

			case 8:
				strcpy(singleToken->value,lexeme);
				strcpy(singleToken->symbol,"MINUS");
				singleToken->lineNo = line;
				return singleToken;

			case 9:
				strcpy(singleToken->value,lexeme);
				strcpy(singleToken->symbol,"MUL");
				singleToken->lineNo = line;
				return singleToken;

			case 10:
				strcpy(singleToken->value,lexeme);
				strcpy(singleToken->symbol,"DIV");
				singleToken->lineNo = line;
				return singleToken;

			case 11:
				strcpy(singleToken->value,lexeme);
				strcpy(singleToken->symbol,"SIZE");
				singleToken->lineNo = line;
				return singleToken;

			case 13:
				c = getNextCharacter(fp);
				if(c=='=')
				{	
					lexeme[lex_index] = c;
					lex_index++;
					state = 20;
				}
				else
				{
					state = 21;
					

				}
				break;

			case 20:
				strcpy(singleToken->value,lexeme);
				strcpy(singleToken->symbol,"LE");
				singleToken->lineNo = line;
				return singleToken;
					 

			case 21:
				strcpy(singleToken->value,lexeme);
				strcpy(singleToken->symbol,"LT");
				singleToken->lineNo = line;
				//need to retract
				retract();
				return singleToken;



			case 14:
				c = getNextCharacter(fp);
				if(c=='=')
				{	
					lexeme[lex_index] = c;
					lex_index++;
					state = 22;
				}
				else
				{
					state = 23;
					

				}
				break;

			case 22:
				strcpy(singleToken->value,lexeme);
				strcpy(singleToken->symbol,"GE");
				singleToken->lineNo = line;
				return singleToken;
					 

			case 23:
				strcpy(singleToken->value,lexeme);
				strcpy(singleToken->symbol,"GT");
				singleToken->lineNo = line;
				//need to retract
				retract();
				return singleToken;


			case 12:
				c = getNextCharacter(fp);
				if (c=='=')
				{
					lexeme[lex_index] = c;
					lex_index++;
					state = 24;

				}
				else if (c=='/')
				{
					lexeme[lex_index] = c;
					lex_index++;
					state = 25;

				}
				else
				{
					state = 26;
				}
				break;

			case 24:
				strcpy(singleToken->value,lexeme);
				strcpy(singleToken->symbol,"EQ");
				singleToken->lineNo = line;
				return singleToken;

			case 25:
				c = getNextCharacter(fp);
				if(c=='=')
				{
					lexeme[lex_index] = c;
					lex_index++;
					state = 27;		
				}
				else
				{
					lexeme[lex_index] = c;
					state = 0;
					//c = getNextCharacter(fp);
					printf("%d: Lexical error: Unknown pattern %s\n",line,lexeme);
				}
				break;
			case 27:
				strcpy(singleToken->value,lexeme);
				strcpy(singleToken->symbol,"NE");
				singleToken->lineNo = line;
				return singleToken;

			case 26:
				strcpy(singleToken->value,lexeme);
				strcpy(singleToken->symbol,"ASSIGNOP");
				singleToken->lineNo = line;
				//need to retract
				retract();
				return singleToken;

			case 15:
				c = getNextCharacter(fp);
				if( (c>='a' && c<='z') || (c>='A' && c<='Z'))
				{
					lexeme[lex_index] = c;
					lex_index++;
					state = 28;
				}
				else
				{
					lexeme[lex_index] = c;
					state =0;
					//c = getNextCharacter(fp);
					printf("%d: Lexical error: Unknown pattern %s\n",line,lexeme);
				}
				break;

			case 28:
				c = getNextCharacter(fp);
				while((c>='a' && c<='z') || (c>='A' && c<='Z') || (c>='0' && c<='9') )
				{
					if(lex_index == MAX_LEXEME)
					{
						printf("%d: Lexical error: Function identifier length exceeded\n",line);
					}
					else
					{
						lexeme[lex_index] = c;
						lex_index++;
					}
					c = getNextCharacter(fp);

				}
				//check in Hash Table;
				//If not found
				if(strcmp(lexeme,"_main")==0)
				{
					strcpy(singleToken->symbol,"MAIN");
				}
				else
				{
					strcpy(singleToken->symbol,"FUNID");
				
				}	
				strcpy(singleToken->value,lexeme);
				singleToken->lineNo = line;
				//need to retract
				retract();
				return singleToken;

			case 16:
				c = getNextCharacter(fp);
				while((c>='a' && c<='z') || (c>='A' && c<='Z') )
				{
					if(lex_index == MAX_LEXEME)
					{
						printf("%d: Lexical error: Variable identifier length exceeded\n",line);

					}
					else
					{
						lexeme[lex_index] = c;
						lex_index++;
					}
					c = getNextCharacter(fp);

				}
				
				if(c>= '0' && c<='9')
				{
					if(lex_index == MAX_LEXEME)
					{
						
						c = getNextCharacter(fp);
						state = 30;
						printf("%d: Lexical error: Variable identifier length exceeded\n",line);

						
					}
					else
					{
						lexeme[lex_index] = c;
						lex_index++;
						state = 29;

					}

				}
				else
				{
					state = 30;
				}
				break;

			case 29: 
				//printf("%s",lexeme);
				strcpy(singleToken->value,lexeme);
				strcpy(singleToken->symbol,"ID");
				singleToken->lineNo = line;
				return singleToken;

			case 30:
				//check in Hash Table;
				//If not found
				//printf("%s",lexeme);

				temp = searchKey(lexeme);
				
				
				//if(singleToken == NULL && strcmp(lexeme,"string\0")==0)
				//{
				//	printf("error\n\n\n");
				//}

				if(temp != NULL)
				{
					temp->lineNo = line;
					retract();
					return temp;
				}	
				else
				{

					strcpy(singleToken->value,lexeme);
					strcpy(singleToken->symbol,"ID");
					singleToken->lineNo = line;

				}	
				//need to retract
				retract();
				return singleToken;

			case 17:
				c = getNextCharacter(fp);
				while(c>='0' && c<='9')
				{
					if(int_index == MAX_INTEGER)
					{
						printf("%d: Lexical error: Number length exceeded\n",line);

					}
					else
					{
						lexeme[lex_index] = c;
						lex_index++;
						int_index++;
					}
					c = getNextCharacter(fp);

				}
				if(c=='.')
				{
					lexeme[lex_index] = c;
					lex_index++;
					state = 31;
				}
				else
				{
					state = 32;
				}
				break;

			case 31:
				c = getNextCharacter(fp);
				if(c>='0' && c<='9')
				{
					lexeme[lex_index] = c;
					lex_index++;
					state = 33;
				}
				else
				{
					//lexeme[lex_index] = c;
					state =0;
					//c = getNextCharacter(c);
					printf("%d: Lexical error: Invalid real\n",line);
					
				}
				break;
			case 33:
				c = getNextCharacter(fp);
				if(c>='0' && c<='9')
				{
					lexeme[lex_index] = c;
					lex_index++;
					state = 34;
				}
				else
				{
					//lexeme[lex_index] = c;
					state =0;
					//c = getNextCharacter(c);
					printf("%d: Lexical error: Invalid real\n",line);
				}
				break;
			case 34:
				strcpy(singleToken->value,lexeme);
				strcpy(singleToken->symbol,"RNUM");
				singleToken->lineNo = line;
				return singleToken;
			case 32:
				strcpy(singleToken->value,lexeme);
				strcpy(singleToken->symbol,"NUM");
				singleToken->lineNo = line;
				//need to retract
				retract();
				return singleToken;
			case 18:
				c = getNextCharacter(fp);
				while((c>='a' && c<='z') || c==' ')
				{
					if(lex_index == MAX_LEXEME-1)
					{
						printf("%d: Lexical error: String length exceeded\n",line);


					}
					else
					{
						lexeme[lex_index] = c;
						lex_index++;
					}
					c = getNextCharacter(fp);
				}

				if(c=='\"')
				{
					lexeme[lex_index] = c;
					lex_index++;
					state = 35;
				}
				else
				{
					lexeme[lex_index] = c;
					state = 0;
					printf("%d: Lexical error: String should end with \" \n",line);

				
				}
				break;
			case 35:
				strcpy(singleToken->value,lexeme);
				strcpy(singleToken->symbol,"STR");
				singleToken->lineNo = line;
				return singleToken;
			case 19:
				c = getNextCharacter(fp);
				if( c =='a')
				{
					lexeme[lex_index] = c;
					lex_index++;
					state = 36;

				}
				else if(c=='o')
				{
					lexeme[lex_index] = c;
					lex_index++;
					state = 37;
				}
				else if(c=='n')
				{
					lexeme[lex_index] = c;
					lex_index++;
					state = 38;

				}
				else
				{
					lexeme[lex_index] = c;
					state = 0;
					//c = getNextCharacter(fp);
					printf("%d: Lexical error: Unknown pattern %s\n",line,lexeme);
				}
				break;
			case 36:
				c = getNextCharacter(fp);
				if(c=='n')
				{
					lexeme[lex_index] = c;
					lex_index++;
					state = 39;

				}
				else
				{
					lexeme[lex_index] = c;
					state = 0;
					//c = getNextCharacter(fp);
					printf("%d: Lexical error: Unknown pattern %s\n",line,lexeme);
				}
				break;
				
			case 39:
				c = getNextCharacter(fp);
				if(c=='d')
				{
					lexeme[lex_index] = c;
					lex_index++;
					state = 40;
				}
				else
				{
					lexeme[lex_index] = c;
					state = 0;
					//c = getNextCharacter(fp);
					printf("%d: Lexical error: Unknown pattern %s\n",line,lexeme);
				}	
				break;
			case 40:
				c = getNextCharacter(fp);
				if(c=='.')
				{
					lexeme[lex_index] = c;
					lex_index++;
					state = 41;

				}
				else
				{
					lexeme[lex_index] = c;
					state = 0;
					printf("%d: Lexical error: Logical operator should end with . \n",line);

				}
				break;
			case 41:
				strcpy(singleToken->value,lexeme);
				strcpy(singleToken->symbol,"AND");
				singleToken->lineNo = line;
				return singleToken;


			case 37:
				c = getNextCharacter(fp);
				if(c == 'r')
				{
					lexeme[lex_index] = c;
					lex_index++;
					state = 42;
				}
				else
				{
					lexeme[lex_index] = c;
					state = 0;
					//c = getNextCharacter(fp);
					printf("%d: Lexical error: Unknown pattern %s\n",line,lexeme);
				}	
				
				break;

			case 42:
				c = getNextCharacter(fp);
				if(c=='.')
				{
					lexeme[lex_index] = c;
					lex_index++;
					state = 43;

				}
				else
				{
					lexeme[lex_index] = c;
					state = 0;
					printf("%d: Lexical error: Logical operator should end with . \n",line);

				}
				break;

			case 43:
				strcpy(singleToken->value,lexeme);
				strcpy(singleToken->symbol,"AND");
				singleToken->lineNo = line;
				return singleToken;

			case 38:
				c = getNextCharacter(fp);
				if(c=='o')
				{
					lexeme[lex_index] = c;
					lex_index++;
					state = 44;

				}
				else
				{
					lexeme[lex_index] = c;
					state = 0;
					//c = getNextCharacter(fp);
					printf("%d: Lexical error: Unknown pattern %s\n",line,lexeme);
				}	
				//does not skip this character
				break;

			case 44:
				c = getNextCharacter(fp);
				if(c=='t')
				{
					lexeme[lex_index] = c;
					lex_index++;
					state = 45;

				}
				else
				{
					lexeme[lex_index] = c;
					state = 0;
					//c = getNextCharacter(fp);
					printf("%d: Lexical error: Unknown pattern %s\n",line,lexeme);
				}	
				//does not skip this character
				break;

			case 45:
				c = getNextCharacter(fp);
				if(c=='.')
				{
					lexeme[lex_index] = c;
					lex_index++;
					state = 46;

				}
				else
				{
					lexeme[lex_index] = c;
					state = 0;
					
					printf("%d: Lexical error: Logical operator should end with . \n",line);

				}
				break;

			case 46:
				strcpy(singleToken->value,lexeme);
				strcpy(singleToken->symbol,"AND");
				singleToken->lineNo = line;
				return singleToken;

			case 47:

				c = getNextCharacter(fp);
				while(c!='\n')
				{
					c = getNextCharacter(fp);
				}
				state =0;
				/*strcpy(singleToken->value,lexeme);
				strcpy(singleToken->symbol,"COMMENT");
				singleToken->lineNo = line;
				return singleToken;
				*/

		}
	}

}



void printListOfTokens(char * filename)
{
	FILE * fp = fopen(filename,"r");
	tokenInfo * token;
	initialiseHash();
	insertEntries();


	getStreamInitial(fp);

	token = getNextToken(fp);
	while(token!=NULL)
	{
		printf("Token value = %s \tToken name = %s\tline no = %d\n",token->value,token->symbol,token->lineNo);
		token = getNextToken(fp);

	}
	fclose(fp);	

}

void test1(char *filename)
{
	FILE * fp = fopen(filename,"r");
	

	getStreamInitial(fp);
	char c =getNextCharacter(fp);
	while(c!=EOF)
	{
		putchar(c);
		c =getNextCharacter(fp);

	} 
	fclose(fp);

}

void testretract(char *filename)
{
	FILE * fp = fopen(filename,"r");
	getStreamInitial(fp);
	char c;
	for(int i=1;i<=10;i++)
	{

		c =getNextCharacter(fp);
		
	} 
	printf("%c",c);
	//retract();
	c =getNextCharacter(fp);
	//c =getNextCharacter(fp);
		
	printf("%c",c);
	fclose(fp);
}


void removeComments(char *testcaseFile)
{
	FILE * fp = fopen(testcaseFile,"r");
	getStreamInitial(fp);
	char c =getNextCharacter(fp);
	int flag =1;
	while(c!=EOF)
	{
		if(c=='#')
			flag =0;

		if(c=='\n' && flag==0)
		{
			flag =1;

		}
		if(flag)
			putchar(c);
		c =getNextCharacter(fp);

	} 
	fclose(fp);

}

/*int main()
{	
	//test1("testcases/testcase1.txt");
	removeComments("testcases/testcase1.txt");
	//testretract("testcases/testcase1.txt");
	
	printListOfTokens("testcases/testcase1.txt");
	return 0;

}
*/




