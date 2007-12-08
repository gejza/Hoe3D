%{
#include "test.h"
#include <stdlib.h>

class HoeFlexTest : public HoeCore::HoeFlex
{
public:
	DECLARE_FLEX_FUNCTIONS
};
#define YY_CLASS_NAME HoeFlexTest

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

DEFINE_TEST(TestFlex, "Flex parser")
{
    HoeCore::HoeFlexFile buff(stdin);
	HoeFlexTest f;
	f.Switch(buff);
	f.Lex();
	return HoeTest::TEST_OK;
} 

// dalsi definice
END_TEST(TestFlex)



