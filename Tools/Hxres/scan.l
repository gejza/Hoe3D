%option yylineno

%{
#include "StdAfx.h"
#include "scan.h"
#define YY_CLASS_NAME Scaner
#define YY_CLASS_DECL Lex(HoeCore::StringPool& pool, union YYSTYPE * l)
#include "parse.tab.hpp"

const char * GetString(const char* text, size_t l, HoeCore::StringPool& pool);

%}

string  \"[^\n"]+\"
ws      [ \t\r]+
comment	\/\/.*
alpha   [A-Za-z_]
dig     [-+]?[0-9]+
hex		"0x"[0-9A-Fa-f]+
name    ({alpha}|\$)({alpha}|{dig}|[_\-/$:])*
real1    [-+]?{dig}+\.({dig}+)?f?
real2    [-+]?{dig}*\.{dig}+f?
real	 {real1}|{real2}

%%
{comment}				/* skip */
{ws}					/* skip */

"Stream"				return TK_Stream;
"StreamFile"			return TK_StreamFile;
"Picture"				return TK_Picture;
"Index"					return TK_Index;
"Model"					return TK_Model;
"Namespace"             return TK_Namespace;
"Texture"               return TK_Texture;
{name}					l->string = pool.Insert(yytext_ptr); return TK_name;
{real}					l->real = HoeCore::string::GetReal(yytext_ptr); return TK_real;
({real}|{dig})"%"		l->real = HoeCore::string::GetReal(yytext_ptr) * 0.01; return TK_perc;
{dig}|{hex}				l->num = HoeCore::string::GetNumber(yytext_ptr); return TK_num;
{string}				l->string = GetString(yytext_ptr, yyleng, pool); return TK_string;
"%"{string}				l->string = GetString(yytext_ptr+1, yyleng-1, pool); return TK_re;
"!"{string}				l->string = GetString(yytext_ptr+1, yyleng-1, pool); return TK_wild;
"@"{string}				l->string = GetString(yytext_ptr+1, yyleng-1, pool); return TK_expand;

\n						return '\n';
.						return yytext_ptr[0];

%%

const char * GetString(const char* text, size_t l, HoeCore::StringPool& pool)
{
	char * str = (char*)pool.Clone(text+1, (l-1) * sizeof(YY_CHAR));
	str[l-2] = '\0';
	return str;
}
