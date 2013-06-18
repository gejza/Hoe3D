%{
#include "StdAfx.h"
#include "icreate.h"
#define YY_CLASS_NAME ICParser
//#define YY_CLASS_DECL Lex(HoeCore::StringPool& pool, union YYSTYPE * l)
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
"sound3d"			return HOE_T_SOUND3D;
"soundplay"			return HOE_T_SOUNDPLAY;
"particle"			return HOE_T_PARTICLE;
"generate"			return HOE_T_GENERATE;
"dummy"				return HOE_T_DUMMY;

"box"				return HOE_T_BOX;
"sphere"			return HOE_T_SPHERE;
"plane"				return HOE_T_PLANE;

"-dump"				return HOE_T_DUMP; /* do logu se vypisou vsechna data */
"con:"				return HOE_T_CON;
"file:"				return HOE_T_FILE;
{name}				return HOE_T_NAME;
{dig}				return HOE_T_INT;
{}	


%%

 void parse_load(const tchar *str)
 {
	yy_scan_string (str);
 } 
 
 int parse()
 {
 	return yylex();
 }
 
const tchar *parser_getstring()
 {

	return (const tchar*)yytext;
 }





