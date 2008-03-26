%option yylineno
%{
#include "StdAfx.h"
#include "../include/HoeCore/const_file.h"
#define YY_CLASS_NAME HoeCore::ConstParser

enum Tokens {
	TName = 1,
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
{alpha}[{alpha}\.]*{alpha}		return TName;

%%

// parse functions
bool  YY_CLASS_NAME::Parse(ConstParserSAX& parser)
{
	int t;
	while (t = Lex(parser))
	{
		switch (t)
		{
		case TName:
			 printf("n: %s\n", yytext); break;
		case TReal:
			 printf("real: %s\n", yytext); break;
		};
	}
	return true;
}


