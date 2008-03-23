%option yylineno
%{
#include "StdAfx.h"
#include "../include/HoeCore/struct_file.h"
#define YY_CLASS_NAME HoeCore::StructParser
#define YY_CLASS_DECL Lex(StructParserSAX& parser)

%}

string  \"[^\n"]+\"
ws      [ \t\r]+
comment	\/\/.*
alpha   [A-Za-z_]
dig     [-+]?[0-9]+
hex		"0x"[0-9A-Fa-f]+
name    {alpha}({alpha}|{dig}|[_\-$:])*
real1    [-+]?{dig}+\.({dig}+)?f?
real2    [-+]?{dig}*\.{dig}+f?
real	 {real1}|{real2}

%%
{comment}				/* skip */
{ws}					/* skip */

"Stream"				return 1;
.						return yytext_ptr[0];

%%

