%option prefix="yycreate"
%{
//#ifdef yywrap
//#undef yywrap
//#else
//#define yyconfigwrap yywrap
static int yywrap() { return 1; } 
//#endif
#include <stdlib.h>
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

{ws}				/* skip */

{string}|{string2}	return 4;
"model"				return 1;
{name}				return 2;
{dig}				return 3;
{}	


%%

void parse_load(const tchar *str)
{
	yy_scan_string ((const YY_CHAR *)str);
} 

int parse()
{
	return yylex();
}

const tchar *parser_getstring()
{
	return (const tchar*)yytext;
}



