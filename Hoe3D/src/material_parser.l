%option prefix="yymaterial"

%{
//#ifdef yywrap
//#undef yywrap
//#else
//#define yyconfigwrap yywrap
static int yywrap() { return 1; } 
//#endif

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "material_parser.h"

%}

ws      [ \t]+
string  \"[^\n"]+\" 
string2  '[^\n']+' 
alpha   [A-Za-z]
dig     [-+]?[0-9]+
name    ({alpha}|\$)({alpha}|{dig}|[_\-/$])*
  
%%


{ws}		/*skip*/
\/\/.*  	/* commentary */ 

=					return MTLP_EQ;
$texture			return MTLP_TEXTURE;
{string}|{string2}	return MTLP_TEXT;
.					return MTLP_ERROR;					

%%

static YY_BUFFER_STATE state;
static FILE * f;

int material_load(const char * file)
{
	f = fopen(file,"r");
	if (f == NULL)
		return 0;
	state = yy_create_buffer (f, 256);
	yy_switch_to_buffer( state);
	return 1;
} 

static int scan_text(mtl_value * val)
{
	if (yylex() != MTLP_TEXT)
		return MTLP_ERROR;

	strncpy(val->text,yytext+1,sizeof(val->text)-1);
	val->text[strlen(val->text)-1] = '\0';
	return val->type;
} 

int material_parse(mtl_value * val)
{
	// scan parametru
	val->type = yylex();
	if (val->type == 0)
		return 0;

	if (yylex() != MTLP_EQ)
		return MTLP_ERROR;

	return scan_text(val);
}

void material_free()
{
	yy_delete_buffer(state);
	if (f != NULL)
		fclose(f);
	f = NULL;
}






