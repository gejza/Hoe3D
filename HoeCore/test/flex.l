%{
#include "test.h"
#include <stdlib.h>
#include <hoe_flex.h>
#include <hoe_file.h>

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
	HoeCore::File f;
	f.Open("test.txt");
    HoeCore::HoeFlexFile buff(f);
	HoeFlexTest fl;
	fl.Switch(buff);
	fl.Lex();
	return HoeTest::TEST_OK;
} 

// dalsi definice
END_TEST(TestFlex)



