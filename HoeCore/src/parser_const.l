%option yylineno
%{
#include "StdAfx.h"
#include "../include/HoeCore/const_file.h"
#define YY_CLASS_NAME HoeCore::ConstParser
#define YY_CLASS_DECL Lex(ConstParserSAX& parser)

%}

string  \"[^\n"]+\"
ws      [ \t\r]+
alpha   [A-Za-z_]
dig     [-+]?[0-9]+
hex		"0x"[0-9A-Fa-f]+
name    {alpha}({alpha}|{dig}|[_\-$:])*
real1    [-+]?{dig}+\.({dig}+)?f?
real2    [-+]?{dig}*\.{dig}+f?
real	 {real1}|{real2}

%%
{ws}					/* skip */
\n						/* skip */


%%

