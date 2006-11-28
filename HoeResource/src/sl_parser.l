%option stack

%{
#include <stdlib.h>
#include <string.h>
#include "../../HoeUtils/lib/hoeUtils.h"
#include "sl_parser.tab.hpp"
#include "../include/hoe_sl_parser.h"

static int yyline = 1;

%}

string  \"[^\n"]+\"
ws      [ \t]+
comment	\/\/.*
alpha   [A-Za-z_]
dig     [-+]?[0-9]+
hex		"0x"[0-9A-Fa-f]+
name    ({alpha}|\$)({alpha}|{dig}|[_\-/$])*
num1    [-+]?{dig}+\.({dig}+)?f?
num2    [-+]?{dig}*\.{dig}+f?
num		{num1}|{num2}

%%

Stream		return C_STREAM;
Index		return C_INDEX;
Model		return C_MODEL;
Material	return C_MATERIAL;
Point		return C_POINT;

~Stream		return E_STREAM;
~Index		return E_INDEX;
~Model		return E_MODEL;
~Material	return E_MATERIAL;
~Point		return E_POINT;

Autotracking return FNC_AUTOTRACKING;

Namespace   return NAMESPACE;
~Namespace  return E_NAMESPACE;

Ambient		return M_AMBIENT;
Diffuse		return M_DIFFUSE;
Specular	return M_SPECULAR;
Texture		return M_TEXTURE;
AlphaTest	return M_ALPHATEST;

xyzrhw		return F_XYZRHW;
xyz			return F_XYZ;
normal		return F_NORMAL;
tex2		return F_TEX2;

streams		return MS_STREAMS;
indices		return MS_INDICES;
materials	return MS_MATERIALS;
helpers		return MS_HELPERS;

{ws}		/* skip blanks and tabs */
{comment}  	/* commentary */  

true		yylval.ivalue = 1; return V_BOOL;
false		yylval.ivalue = 0; return V_BOOL;

{string}	strcpy(yylval.svalue,hu_GetString(yytext)); return V_STRING;
{hex}		yylval.ivalue = hu_GetHex(yytext); return V_NUMBER;
{num}		yylval.fvalue = hu_GetFloat(yytext); return V_FLOAT;
{dig}		yylval.ivalue = hu_GetNum(yytext); return V_NUMBER;
{name}		strcpy(yylval.svalue,yytext); return V_NAME;

\n			yyline++; return '\n';
.			return *yytext;

%%

void set_parser(HoeSL::Parser::BaseParser * parser);

void HoeSL::Parser::parser_scan(FILE * f, HoeSL::Parser::BaseParser * parser)
{
	yyline = 1;
	yyin = f;
	set_parser(parser);
}

/*void HoeSL::Parser::parse_load(const char *str)
{
	yy_scan_string (str);
} */

int HoeSL::Parser::get_line()
{
	return yyline;
}








