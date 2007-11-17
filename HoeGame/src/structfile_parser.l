
%option prefix="yyobject"
%option yylineno

%{
#pragma warning( disable : 4102 4273 )

#ifdef yywrap
#undef yywrap
#else
#define YY_SKIP_YYWRAP 1
static int yyobjectwrap() { return 1; } 
#endif
#include "StdAfx.h"
%}

ws			[ \t\r]+
comment1	#.*
comment2	\/\/.*
comment3	\-\-.*

dig     [-+]?[0-9]+
digf	{dig}\.[0-9]+
alpha	[A-Za-z]+
name    ({alpha}|\$)({alpha}|{dig}|[_\-/$])* 
string  \"[^\n"]+\" 
string2  '[^\n']+' 
  
%%

{ws}								/* skip */
{comment1}|{comment2}				/* comment */
{name}								return 257;
{string}|{string2}					{ /* odzavorkovat */
									yytext[yyobjectleng-1] = 0;
									yytext++;
									return 258; }
{dig}								return 259;
{digf}								return 260;
.									return yytext[0];
\n									return '\n';

%%





