%{
#include "test.h"
#include <stdlib.h>
#include <hoe_flex.h>

class HoeFlexTest : public HoeCore::HoeFlex
{
public:
	DECLARE_FLEX_FUNCTIONS(void)
};
#define YY_CLASS_NAME HoeFlexTest

%}
 
%%

.			/* nothing */

%%

DEFINE_TEST(TestFlex, "Flex parser")
{
	FILE * f = fopen("test.txt","rt");
    HoeCore::HoeFlexFile buff(f);
	HoeFlexTest fl;
	fl.Switch(buff);
	fl.Lex();
	fclose(f);
	return HoeTest::TEST_OK;
} 

// dalsi definice
END_TEST(TestFlex)



