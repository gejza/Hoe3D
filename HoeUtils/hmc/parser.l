 
%option stack
 
%{
#include <stdlib.h>
#include "parser.h"
#include "../lib/hoeUtils.h"

struct p_value *lval = NULL;

%}

string  \"[^\n"]+\"

ws      [ \t]+
comment	\/\/.*\n



alpha   [A-Za-z_]
dig     [-+]?[0-9]+
hex		"0x"[0-9A-Fa-f]+
name    ({alpha}|{dig}|\$)({alpha}|{dig}|[_.\-/$])*
num1    [-+]?{dig}+\.({dig}+)?f?
num2    [-+]?{dig}*\.{dig}+f?

num		{num1}|{num2}

%%
{ws}		/* skip blanks and tabs */
\/\/.*  	/* commentary */ return P_NL;  
<<EOF>>		return P_NL;

{string}	if (lval) lval->str = hu_GetString(yytext); return V_STRING;

[;,]		if (lval) lval->c = yytext[0]; return P_SEP;
[{\(\[]		if (lval) lval->c = yytext[0]; return P_STT;
[}\]\)]		if (lval) lval->c = yytext[0]; return P_END;
[|]			return P_PIPE;

[=]			return P_EQ;

{hex}		if (lval) lval->hex = hu_GetHex(yytext); return V_HEX;
{num}		if (lval) lval->f = hu_GetFloat(yytext); return V_FLOAT;
{dig}		if (lval) lval->i = hu_GetNum(yytext); return V_INT;
true		if (lval) lval->i = 1; return V_BOOL;
false		if (lval) lval->i = 0; return V_BOOL;

Stream		if (lval) lval->str = yytext; return C_STREAM;
Index		if (lval) lval->str = yytext; return C_INDEX;
Model		if (lval) lval->str = yytext; return C_MODEL;
Material	if (lval) lval->str = yytext; return C_MATERIAL;
Point		if (lval) lval->str = yytext; return C_POINT;

~Stream		if (lval) lval->str = yytext; return E_STREAM;
~Index		if (lval) lval->str = yytext; return E_INDEX;
~Model		if (lval) lval->str = yytext; return E_MODEL;
~Material	if (lval) lval->str = yytext; return E_MATERIAL;
~Point		if (lval) lval->str = yytext; return E_POINT;

Autotracking\({name}\) if (lval) lval->str = yytext; return FNC_AUTOTRACKING;

Ambient		return M_AMBIENT;
Diffuse		return M_DIFFUSE;
Specular	return M_SPECULAR;
Texture		return M_TEXTURE;
AlphaTest	return M_ALPHATEST;

xyzrhw		return F_XYZRHW;
xyz			return F_XYZ;
normal		return F_NORMAL;
tex2		return F_TEX2;

streams		if (lval) lval->str = yytext; return MS_STREAMS;
indices		if (lval) lval->str = yytext; return MS_INDICES;
materials	if (lval) lval->str = yytext; return MS_MATERIALS;
helpers		if (lval) lval->str = yytext; return MS_HELPERS;
input		if (lval) lval->str = yytext; return MS_INPUT;
fp[0-9]		if (lval) lval->i = yytext[2] - '0'; return MS_FPARAM;
ip[0-9]		if (lval) lval->i = yytext[2] - '0'; return MS_IPARAM;
stdrender	if (lval) lval->str = yytext; return MS_STDRENDER;
allrender	if (lval) lval->str = yytext; return MS_ALLRENDER;
point		if (lval) lval->str = yytext; return MS_POINT;


{name}\({dig}\)	if (lval) lval->str = yytext; return V_NAME_INDEX;
{name}		if (lval) lval->str = yytext; return V_NAME;
.			if (lval) lval->str = yytext; return P_ERROR;

%%

void parser_scan(FILE * f)
{
	yyin = f;
}

void parse_load(const char *str)
{
	yy_scan_string (str);
} 

int lexln(struct p_value * val)
{
	int ret;
	lval = val;
	ret = yylex();
	lval = NULL;
	return ret;
}

int yywrap()
{
	return 1;
}

/*


int parse()
{
	return yylex();
}

const char *parser_getstring()
{
	return yytext;
}
*/






