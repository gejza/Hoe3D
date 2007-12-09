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

%token Stream StreamFile Texture TextureFile
%token Material MaterialFile Index IndexFile



%%
file: 
	res
;
res:
	resource
	| res resource
;
resource:
	Stream
	{
		// parse stream
	}
	'~' Stream
%%
