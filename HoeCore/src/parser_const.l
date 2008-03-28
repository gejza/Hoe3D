%option yylineno
%{
#include "StdAfx.h"
#include "../include/HoeCore/const_file.h"
#include "../include/HoeCore/hoe_structures.h"
#include "../include/HoeCore/hoe_mem.h"
#include "../include/HoeCore/hoe_core.h"
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

#define ERR(str) { parser.ParseError(T("Error: ") T(str)); return false; }

// parse functions
bool  YY_CLASS_NAME::Parse(ConstParserSAX& parser)
{
	HoeCore::List<const tchar*> name;
	HoeCore::StringPool pool;
	while (int t = Lex())
	{
		if (t == '\n') continue;
		if (t != TName) ERR("Name requied.");
		// jmeno
		name.Set(pool.Insert(yytext));
		while ((t = Lex()) == '.')
		{
			if (Lex() != TName) ERR("Property requied.");
			name.Add(pool.Insert(yytext));
		}
		if (t != '=') ERR("`=' requied.");
		// value
		switch (t = Lex())
		{
		case '\n': ERR("Value requied"); 
		case TName:
		case TReal:
			parser.SetConst(name, HoeCore::string::GetReal(yytext));
			break;
		default:
			printf("Syntax error: %c\n", (char)t); break;
		};
		
		t = Lex(); // end
		if (t && t != '\n') ERR("End line requied.");
		if (!t) return true;
	}
	return true;
}


