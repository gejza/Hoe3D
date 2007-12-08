%pure-parser
%parse-param { Scaner * lex }
%lex-param   { Scaner * lex }
     //%parse-param {int *randomness}

%{
#include "StdAfx.h"
#include "scan.h"
static int yylex(int * l, Scaner * lex)
{
	return lex->Lex();
}

int yyerror(Scaner * lex, char* )
{
	return 0;
}

%}

%token AAA



%%
a: AAA
%%
