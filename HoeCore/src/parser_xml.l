%option yylineno
%{
#include "StdAfx.h"
#include "../include/HoeCore/struct_file.h"
#define YY_CLASS_NAME HoeCore::XMLParser
#define YY_CLASS_DECL Lex(StructParserSAX& parser)

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

%s		XMLTEXT

%%
{ws}					/* skip */
\n						/* skip */
"<?xml"[^>]*">"			/* skip */
"<!--"[^-]*"-->"		/* skip */
"<"[A-Za-z]+			parser.Begin(yytext_ptr+1); BEGIN(INITIAL);
[a-z]+[ \t]*=[ \t]*\"[^\"]*\" this->SetProp(parser, yytext_ptr);
">"						m_str = T(""); BEGIN(XMLTEXT);
"/>"					parser.End();
"</"[^>]+">"			if (m_str) parser.SetProp(m_textprop, m_str); parser.End(); BEGIN(INITIAL); // end

<XMLTEXT>[^<]*			m_str += yytext_ptr;

%%

//todo amprsand atd znaky
HoeCore::XMLParser::XMLParser()
{
	m_textprop = T("text");
}


void HoeCore::XMLParser::SetProp(StructParserSAX& parser, tchar* text)
{
	const tchar* name = text;
	while (*text != ' ' && *text != '\t' && *text != '=') text++;
	*text = 0; 
	while (*text != '\"') text++;
	text++;
	const tchar* value = text;
	while (*text != '\"') text++;
	*text = 0;
	parser.SetProp(name, value);
}