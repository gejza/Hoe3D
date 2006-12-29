
%option prefix="yyfigure"
%option yylineno

%{
#ifdef yywrap
#undef yywrap
#else
#define yyconfigwrap yywrap
static int yywrap() { return 1; } 
#endif
#include "StdAfx.h"
#include "../include/hoe_gui.h"
%}

ws			[ \t]+
comment1	#.*
comment2	\/\/.*
comment3	\-\-.*

dig     [-+]?[0-9]+
alpha	[A-Za-z]+
name    ({alpha}|\$)({alpha}|{dig}|[_\-/$])* 
  
%%

{ws}								/* skip */
{comment1}|{comment2}				/* comment */
{name}								return 257;
\{									return 258;
=.*$								return 259;
\}									return 260;	
\n									/* skip */;  

.									return yytext[0];

%%





