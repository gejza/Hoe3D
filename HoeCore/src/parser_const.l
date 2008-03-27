%option yylineno
%{
#include "StdAfx.h"
#include "../include/HoeCore/const_file.h"
#define YY_CLASS_NAME HoeCore::ConstParser

enum Tokens {
	TName = 256,
	TReal,
};
%}

comment	\/\/.*
comment2 ^#.*
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
{comment}				/* skip */
{comment2}				/* skip */


{real}					return TReal;
{name}					return TName;
.						return yytext[0];
\n						return T('\n');

%%

// parse functions
bool  YY_CLASS_NAME::Parse(ConstParserSAX& parser)
{
	int t;
	while (t = Lex())
	{
		switch (t)
		{
		case '\n':
			printf("nl\n"); break;
		case TName:
			 printf("n: %s\n", yytext); break;
		case TReal:
			 printf("real: %s\n", yytext); break;
		default:
			printf("ch: %c\n", (char)t);
		};
	}
	return true;
}


