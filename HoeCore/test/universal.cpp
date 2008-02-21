
#include "../src/StdAfx.h"
#include <stdlib.h>
#include "../include/HoeCore/hoe_core.h"
#include "../include/HoeCore/hoe_structures.h"
#include "test.h"

DEFINE_TEST(Universal, "Test universal structure")
{
    HoeCore::Universal u;
    u = "aaa";
    
    printf("%s : %s\n", u.GetTypeName(), (const char*)u);

	return HoeTest::TEST_OK;
}

END_TEST(Universal)




