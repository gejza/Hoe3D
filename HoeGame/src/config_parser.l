
%option prefix="yyconfig"

%{
//#ifdef yywrap
//#undef yywrap
//#else
//#define yyconfigwrap yywrap
static int yywrap() { return 1; } 
//#endif
#include "StdAfx.h"
%}

ws			[ \t]+
comment1	#.*\n
comment2	\/\/.*\n
comment3	\-\-.*\n

string  \"[^\n"]+\" 
string2  '[^\n']+' 
alpha   [A-Za-z]
dig     [-+]?[0-9]+
digf	{dig}\.[0-9]+
name    ({alpha}|_)({alpha}|{dig}|[_\-/$:@#%^&*])*
  
%%

{ws}								/* skip */
{comment1}|{comment2}|{comment3}	/* comment */ unput('\n');
\n									return 1;  
=									return 3;
{string}|{string2}					return 4;
yes|y|true|t						return 7;
no|n|false|f						return 8;
{name}								return 2;
\${name}							return 5;
{dig}								return 6;
{digf}								return 9;
.									return -1;

%%

int config_load(FILE * f)
{
	yyrestart(f);
	return 1;
}

int config_parse(char ** text)
{
	int ret;
	ret = yylex();
	*text = yytext;
	while (**text == 10)
		(*text)++;
	return ret;
}


