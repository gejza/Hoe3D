%{
#include "StdAfx.h"
#include "scan.h"
#define YY_CLASS_NAME Scaner
#include "parse.tab.hpp"

%}


%%
"//".*$					/* skip */

"Stream"				return Stream;
"StreamFile"			return StreamFile;

.						return yytext_ptr[0];

%%


