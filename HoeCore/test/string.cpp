
#include "../src/StdAfx.h"
#include <stdlib.h>
#include "../include/hoe_core.h"
#include "../include/hoe_structures.h"
#include "test.h"

DEFINE_TEST(String, "Test static string")
{
    HoeCore::String_s<100> str;
    str = "aaa";
    
    if (strlen(str) != 3)
		return HoeTest::TEST_FAILED;

	return HoeTest::TEST_OK;
}

// dalsi definice
END_TEST(String)




