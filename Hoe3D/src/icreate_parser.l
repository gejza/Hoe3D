%option prefix="yycreate"
%{
//#ifdef yywrap
//#undef yywrap
//#else
//#define yyconfigwrap yywrap
static int yywrap() { return 1; } 
//#endif
#include <stdlib.h>
#include "hoe_tokens.h"
//int mylineno = 0;
//using namespace std;

//extern Object * act;
%}

ws      [ \t]+
string  \"[^\n"]+\" 
string2  '[^\n']+' 
alpha   [A-Za-z]
dig     [-+]?[0-9]+
name    ({alpha}|\$)({alpha}|{dig}|[_\-/$:@#%^&*])*

  
%%

{ws}		/*skip*/

{string}|{string2}	return HOE_T_STRING;
"model"				return HOE_T_MODEL;
"picture"			return HOE_T_PICTURE;
"font"				return HOE_T_FONT;
"sound"				return HOE_T_SOUND;
"particle"			return HOE_T_PARTICLE;
"generate"			return HOE_T_GENERATE;

"box"				return HOE_T_BOX;
"sphere"			return HOE_T_SPHERE;

"-dump"				return HOE_T_DUMP; /* do logu se vypisou vsechna data */
"con:"				return HOE_T_CON;
"file:"				return HOE_T_FILE;
{name}				return HOE_T_NAME;
{dig}				return HOE_T_INT;
{}	


%%

void parse_load(const char *str)
{
	yy_scan_string (str);
} 

int parse()
{
	return yylex();
}

const char *parser_getstring()
{
	return yytext;
}



