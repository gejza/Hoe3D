%option yylineno

%{
#include "StdAfx.h"
#include "scan.h"
#define YY_CLASS_NAME Scaner
#include "parse.tab.hpp"

%}

string  \"[^\n"]+\"
ws      [ \t]+
comment	\/\/.*
alpha   [A-Za-z_]
dig     [-+]?[0-9]+
hex		"0x"[0-9A-Fa-f]+
name    ({alpha}|\$)({alpha}|{dig}|[_\-/$:])*
num1    [-+]?{dig}+\.({dig}+)?f?
num2    [-+]?{dig}*\.{dig}+f?
num		{num1}|{num2}

%%
{comment}				/* skip */
{ws}					/* skip */

"Stream"				return TK_Stream;
"StreamFile"			return StreamFile;
"Picture"				return TK_Picture;
{name}					yytext_ptr[0]; return TK_name;
{num}					return TK_num;
{dig}					return TK_num;
{string}				return TK_string;

\n						return '\n';
.						return yytext_ptr[0];

%%


