
%option prefix="yylang"

%{
#undef yywrap
#include "StdAfx.h"
static int mylineno = 1;
%}

ws			[ \t]+
comment1	#.*\n
comment2	\/\/.*\n
comment3	\-\-.*\n

string  \"[^\n"]+\" 
string2  '[^\n']+' 
dig     [-+]?[0-9]+
  
%%

{ws}								/* skip */
{comment1}|{comment2}|{comment3}	/* comment */ unput('\n');
{string}|{string2}					return 3;
=									return 2;
{dig}								return 1;	
\n									mylineno++;  

.									return 5;

%%

int lang_load(FILE * f)
{
	yyrestart(f);
	return 1;
}

int lang_parse(char *buffer)
{
    int id;
	int l;
	int token;

	token = yylex();
	if (token == 0)
		return 0;
	if (token != 1)
		goto error;
		
	id = atoi(yytext);

	token = yylex();
	if (token != 2)
		goto error;

	token = yylex();
	if (token != 3)
		goto error;

	strcpy(buffer, yytext+1);
	l = strlen(buffer) - 1;
	if (buffer[l] != '\'' && buffer[l] != '\"')
		goto error;
	buffer[l] = '\0';

	return id;

error:
	sprintf(buffer,"Lang parse error on line %d",mylineno);
	return -1;
}


