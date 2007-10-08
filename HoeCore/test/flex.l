%option prefix="yycreate"
%{
#include <stdlib.h>
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

int test_flex()
{
    int a;
	yy_scan_string ("testovaci string");
    while (a = yylex())
    {
        printf("%d %s\n",a,yytext);
    }
    return 0;
} 



