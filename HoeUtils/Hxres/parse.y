%pure-parser
%error-verbose
%parse-param { Linker& linker, HoeCore::StringPool& pool, Scaner& lex }
%lex-param   { HoeCore::StringPool& pool }
%lex-param   { Scaner& lex }
	//%parse-param {int *randomness}

%{
#define YYDEBUG 1
#define YYERROR_VERBOSE 1

#include "StdAfx.h"
#include "scan.h"
#include "linker.h"

static int yylex(union YYSTYPE * l, HoeCore::StringPool& pool, Scaner& lex)
{ 
	return lex.Lex(pool,l);
}

static int yyerror(Scaner& lex, char* err)
{
	fprintf(stderr, "%s(%d) : ", lex.GetIdentifier(), lex.GetLine());
	fprintf(stderr, err, lex.GetText());
	fprintf(stderr, "\n");
	return 0;
}

%}

%union {
  int            ivalue;
  const char * string;
}

%token TK_Stream TK_StreamFile TK_Texture TK_TextureFile
%token TK_Material TK_MaterialFile TK_IndexFile
%token TK_Picture TK_Model TK_Index TK_Namespace TK_Texture
%token <string>TK_name "%s" TK_num " %s" TK_string

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
		| namespace
;
namespace:
		TK_Namespace TK_name '\n'
		 { linker.PushNamespace($2); } 
		res
		'~' TK_Namespace
		 { linker.PopNamespace(); }
;
stream:	TK_Stream TK_name { linker.AddObject($2, 1); } 
			'[' { /* start fvf */ } fvf ']' '\n'
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
