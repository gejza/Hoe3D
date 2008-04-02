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
	TString,
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
{string}				return TString;
.						return yytext[0];
\n						return T('\n');

%%

#define ERR(str) { parser.ParseError(T("Error: ") T(str)); return false; }

// parse functions
bool  YY_CLASS_NAME::Parse(ConstParserSAX& parser)
{
	m_token = Lex();
	while (m_token)
	{
		if (m_token == '\n') continue;
		if (m_token != TName) ERR("Name requied.");
		// jmeno
		m_name.Set(m_pool.Insert(yytext));
		while ((m_token = Lex()) == '.')
		{
			if (Lex() != TName) ERR("Property requied.");
			m_name.Add(m_pool.Insert(yytext));
		}
		if (m_token != '=') ERR("`=' requied.");
		// value
		switch (m_token = Lex())
		{
		case '\n': ERR("Value requied"); 
		case TName:
			// set as name -- muze byt vzorec nebo struktura
			{
				
			}			
			break;
		case TReal:
			parser.SetConst(m_name, HoeCore::string::GetReal(yytext));
			break;
		case TString:
			yytext[yyleng-1] = T('\0');
			parser.SetConst(m_name, yytext+1);
			break;
		default:
			printf("Syntax error: %c\n", (char)m_token); break;
		};
		
		if (m_token && m_token != T('\n')) ERR("End line requied.");
		if (m_token == T('\n')) m_token = Lex();
	}
	return true;
}

// sax

void HoeCore::ConstParserSAX::SetConst(const ValueName& name,
	const Universal& value) // run from sax parser
{
	hoe_assert(name.Count());
	HoeCore::String_s<2048> n;
	n.Join(name, T("."));
	this->SetConst(n, value);
}

void HoeCore::ConstParserSAX::SetConst(const tchar* name,
		const Universal& value)
{
}

void HoeCore::ConstParserSAX::SetConst(const ValueName& name, const tchar* type, const Values& params)
{
}

bool HoeCore::ConstParserSAX::GetConst(const ValueName& name, Universal& value) // run from sax parser
{
	hoe_assert(name.Count());
	HoeCore::String_s<2048> n;
	n.Join(name, T("."));
	return this->GetConst(n, value);
}

bool HoeCore::ConstParserSAX::GetConst(const tchar* name, Universal& value)
{
	return false;
}


