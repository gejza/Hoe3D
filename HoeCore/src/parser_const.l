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
	TNumber,
	TPixel,
	TString,
}; // stejne jako const_file.cpp

%}

comment	\/\/.*
comment2 ^#.*
string  \"[^\n"]+\"
ws      [ \t\r]+
alpha   [A-Za-z_]
dig     [0-9]+
hex		"0x"[0-9A-Fa-f]+
name    {alpha}({alpha}|[0-9]|[_\$:])*
real1    [-+]?{dig}+\.({dig}+)?f?
real2    [-+]?{dig}*\.{dig}+f?
real	 {real1}|{real2}

%%
{ws}					/* skip */
{comment}				/* skip */
{comment2}				/* skip */


{real}					return TReal;
{name}					return TName;
{string}				return TString;
{dig}|{hex}				return TNumber;
{dig}px					return TPixel;
.						return yytext[0];
\n						return T('\n');

%%

#define ERR(str) { parser.ParseError(T("Error: ") T(str)); return false; }

// parse functions
bool  YY_CLASS_NAME::Parse(ConstParserI& parser)
{
	int token = Lex();
	while (token)
	{
		if (token == T('\n')) 
		{
			token = Lex();	
			continue;
		}
		if (token != TName) ERR("Name requied.");
		// jmeno
		m_name.Set(m_pool.Insert(yytext));
		while ((token = Lex()) == '.')
		{
			if (Lex() != TName) ERR("Property requied.");
			m_name.Add(m_pool.Insert(yytext));
		}
		if (token != '=') ERR("`=' requied.");
		// value
		m_value.Delete();
		while (token = Lex(), token && token != T('\n'))
		{
			// add property
			Token& t = m_value.Add();
			memset(&t, 0, sizeof(Token));
			t.token = token;
			switch (token)
			{
			case TName:
				t.str = m_pool.Insert(yytext);
				break;
			case TString:
			{
				tchar * s = (tchar*)m_pool.Strdup(yytext+1);
				s[yyleng-2] = T('\0');
				t.str = s;
			}	break;
			case TReal:
				t.real = string::GetReal(yytext);
				break;
			case TPixel:
			case TNumber:
				t.number = string::GetNumber(yytext);
				break;
			};
		}
		// process value
		if (!m_value.Count())
			ERR("Value requied.");
		
		// parse value
		if (!ParseValue(parser))
			return false;
		
		if (token == T('\n')) token = Lex();
	}
	return true;
}

