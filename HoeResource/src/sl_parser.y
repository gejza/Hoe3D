%{
#include <stdio.h>
#include <stdlib.h>
#include "../include/hoe_sl_parser.h"

#define YYDEBUG 1
#define YYERROR_VERBOSE 1

using namespace HoeSL::Parser;

static BaseParser * parser_main = NULL;
static IndexParser * parser_index = NULL;
static StreamParser * parser_stream = NULL;
static ModelParser * parser_model = NULL;
static MaterialParser * parser_material = NULL;
#define PO(pars) if (parser_##pars) parser_##pars

int yylex(void);
int yyerror(char* );

extern char *yytext;

/*
	{'f',4,1,'\0'},
	{'x',4,1,'\0'},
	{'i',4,1,'\0'},
	{'b',1,1,'\0'},
	{'w',2,1,'\0'},
	{'1',4,1,'f'},
	{'2',8,2,'f'},
	{'3',12,3,'f'},
	{'4',16,4,'f'},
	{'c',4,1,'x'},
	{'p',12,3,'3'},
	{'r',16,4,'4'},
	{'n',12,3,'3'},
	{'d',4,1,'c'},
	{'s',4,1,'c'},
	{'t',8,2,'2'} 
*/

char fvf_string[256];
char * p_fvf = fvf_string;

%}

%union {
  int            ivalue;
  float			 fvalue;
  char          svalue[128];
}

%token C_STREAM C_INDEX C_MODEL C_MATERIAL C_POINT 
%token E_STREAM E_INDEX E_MODEL E_MATERIAL E_POINT
%token M_AMBIENT M_DIFFUSE M_SPECULAR M_TEXTURE M_ALPHATEST
%token F_XYZRHW F_XYZ F_NORMAL F_TEX2
%token FNC_AUTOTRACKING 
%token MS_STREAMS MS_INDICES MS_MATERIALS MS_HELPERS
%token NAMESPACE E_NAMESPACE
%token <ivalue> V_BOOL 
%token <svalue> V_STRING 
%token <ivalue> V_NUMBER 
%token <fvalue> V_FLOAT 
%token <svalue> V_NAME

%%

classes:  class          
        | classes class
;

class:     index   
			| material
			| stream
			| point
			| model
			| function
			| namespace
           | '\n'
;
namespace: NAMESPACE V_NAME '\n'
			{ PO(main)->BeginNamespace($2); }
           classes
           E_NAMESPACE '\n'
            { PO(main)->EndNamespace(); }
;
index:    C_INDEX V_NAME '\n'
			{ if (parser_main) parser_index = parser_main->BeginIndex($2,true); }
           index_data
          E_INDEX '\n'
            { PO(index)->End(); PO(main)->EndIndex(); parser_index=NULL;}
          | C_INDEX V_NAME '~' '\n' { PO(main)->BeginIndex($2, false); }
;		
index_data: 
           |  V_NUMBER { PO(index)->ParseNumber($1); }
           | index_data ',' V_NUMBER { PO(index)->ParseNumber($3); }
           | index_data '\n' V_NUMBER { PO(index)->ParseNumber($3); }
           | index_data '\n'
;
material: C_MATERIAL V_NAME '\n'
           { if (parser_main) parser_material = parser_main->BeginMaterial($2, true); } 
           material_data
          E_MATERIAL '\n'
           { PO(material)->End(); PO(main)->EndMaterial(); parser_material=NULL; }
           | C_MATERIAL V_NAME '~' '\n'
           { PO(main)->BeginMaterial($2, false); } 
;
material_data:
           material_property
           | material_data '\n' material_data   
;
material_property:
           M_TEXTURE '=' V_STRING '\n'
           { PO(material)->SetTexture($3); }
;
stream:    C_STREAM V_NAME { p_fvf = fvf_string; }
           '[' fvf_data ']' '\n'
           { *p_fvf = 0; 
           if (parser_main) parser_stream = parser_main->BeginStream($2, fvf_string,true); } 
           stream_data
          E_STREAM '\n'
           { PO(stream)->End(); PO(main)->EndStream(); parser_stream = NULL;}
          | C_STREAM V_NAME '(' V_NUMBER ')' 
            { p_fvf = fvf_string; } '[' fvf_data ']' '\n'
           { *p_fvf = 0; if (parser_main) parser_stream = parser_main->BeginStream($2, fvf_string,true); } 
           stream_data
          E_STREAM '\n'
           { PO(stream)->End(); PO(main)->EndStream(); parser_stream = NULL; }
           | C_STREAM V_NAME { p_fvf = fvf_string; }
           '[' fvf_data ']' '~' '\n'
           { *p_fvf = 0; PO(main)->BeginStream($2, fvf_string,false); }
;		
fvf_data: F_XYZRHW { *p_fvf++ = 'r'; }
			| F_XYZ { *p_fvf++ = 'p'; }
			| F_NORMAL  { *p_fvf++ = 'n'; }
			| F_TEX2  { *p_fvf++ = 't'; }
			| fvf_data '|' fvf_data

stream_data: 
           |  V_FLOAT { PO(stream)->AddFloat($1); }
           |  stream_data ',' V_FLOAT  { PO(stream)->AddFloat($3); }
           |  stream_data ';' V_FLOAT  { PO(stream)->AddFloat($3); }
           | stream_data '\n' V_FLOAT  { PO(stream)->AddFloat($3); }
           |  V_NUMBER { PO(stream)->AddNumber($1); }
           | index_data ',' V_NUMBER { PO(stream)->AddNumber($3); }
           | index_data ';' V_NUMBER { PO(stream)->AddNumber($3); }
           | index_data '\n' V_NUMBER { PO(stream)->AddNumber($3); }
           | stream_data '\n'
;

point:     C_POINT V_NAME '\n'
            V_FLOAT ',' V_FLOAT ',' V_FLOAT '\n'
            E_POINT '\n'
            { PO(main)->Point($2,$4,$6,$8); }
;
model:    C_MODEL V_NAME '\n'
           { if (parser_main) parser_model = parser_main->BeginModel($2,true); } 
          model_param
          E_MODEL '\n'
           { PO(model)->End(); PO(main)->EndModel(); parser_model = NULL; }
           | C_MODEL V_NAME '~' '\n'
           { PO(main)->BeginModel($2,false); } 
;
model_param:
            MS_STREAMS name_list '\n'
          | MS_INDICES name_list '\n'
          | MS_MATERIALS name_list '\n'
          | MS_HELPERS name_list '\n'
          | model_param model_param
;
name_list:
          V_NAME 
          { printf("Link %s\n",$1); }
          | name_list ',' name_list
;
function:
         FNC_AUTOTRACKING '(' V_NAME ')' '\n'
         { PO(main)->FuncAutotracking($3); }
;

%%

void set_parser(BaseParser * parser)
{
	parser_main = parser;
}

int yyerror(char *chybka)
{
  char buff[1024];
  sprintf(buff,"%s(%s)", chybka, yytext);
  PO(main)->Error(buff);
 
  return 0;
}

