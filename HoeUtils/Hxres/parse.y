%pure-parser
%error-verbose
%parse-param { Scaner * lex }
%lex-param   { Scaner * lex }
     //%parse-param {int *randomness}

%{
#define YYDEBUG 1
#define YYERROR_VERBOSE 1

#include <stdio.h>
#include "scan.h"
static int yylex(int * l, Scaner * lex)
{ 
	return lex->Lex();
}

static int yyerror(Scaner * lex, char* err)
{
	fprintf(stderr, "%s(%d) : ", lex->GetIdentifier(), lex->GetLine());
	fprintf(stderr, err, lex->GetText());
	fprintf(stderr, "\n");
	return 0;
}

%}

%token TK_Stream StreamFile Texture TextureFile
%token Material MaterialFile Index IndexFile
%token TK_Picture TK_Model TK_Index
%token TK_name "%s" TK_num " %s" TK_string

%%
file: 
	res
;
res:
	resource
	| res resource
;
resource:
		'\n'
		| stream
		| picture
		| index
		| model
;
stream:	TK_Stream TK_name '[' { /* start fvf */ } fvf ']' '\n'
		{
			// parse stream
		}
			stream_data
		'~' TK_Stream '\n'
;
fvf:	TK_name
		| fvf '|' TK_name
;
stream_data:
		stream_data_row '\n'
		| stream_data stream_data_row '\n'
;
stream_data_row:
		TK_num
		| stream_data_row ',' TK_num
		| stream_data_row ';' TK_num
;
picture: 
		TK_Picture TK_name '\n'
		  picture_attribute
		'~' TK_Picture '\n'
;
picture_attribute:
		'\n'
		| TK_name '=' value '\n'
		| picture_attribute TK_name '=' value '\n'
;
index:	TK_Index TK_name '\n'
		index_data
		'~' TK_Index
;
index_data:
		| TK_num
		| index_data ',' TK_num
		| index_data '\n'
		| index_data '\n' TK_num
;
model:	TK_Model model_name '\n'
		'~' TK_Model
;
model_name: TK_name
		| TK_name '(' ')'
		| TK_name '(' model_param ')'
;
model_param:
		TK_name
		| model_param ',' TK_name
;
value:	TK_name
		| TK_num
		| TK_string
		| vector
;
vector: '(' vector_item ')'
;
vector_item: TK_num
			 | vector_item ',' TK_num
;
%%
