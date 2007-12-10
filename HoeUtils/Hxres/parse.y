%pure-parser
%parse-param { Scaner * lex }
%lex-param   { Scaner * lex }
     //%parse-param {int *randomness}

%{
#define YYDEBUG 1
#define YYERROR_VERBOSE 1

#include "StdAfx.h"
#include "scan.h"
static int yylex(int * l, Scaner * lex)
{
	return lex->Lex();
}

int yyerror(Scaner * lex, char* err)
{
	fputs(err,stderr);
	return 0;
}

%}

%token TK_Stream StreamFile Texture TextureFile
%token Material MaterialFile Index IndexFile
%token TK_name TK_num



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
		| TK_Stream TK_name '[' { /* start fvf */ } fvf ']' '\n'
		{
			// parse stream
		}
			stream_data
		'~' TK_Stream '\n'
;
fvf:	TK_name ':' TK_name
		| fvf '|' TK_name ':' TK_name
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
%%
