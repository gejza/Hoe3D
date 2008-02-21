
#include "../src/StdAfx.h"
#include <stdlib.h>
#include "../include/HoeCore/hoe_core.h"
#include "../include/HoeCore/hoe_structures.h"
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

DEFINE_TEST(DynString, "Test synamic string")
{
    HoeCore::String str1;
    HoeCore::String str2;
    int r = 1;
    str1.printf("%d",r++);
    str2.printf("(%d)%s",r++, str1.GetPtr());
    str1.printf("(%d)%s",r++, str2.GetPtr());
    str2.printf("(%d)%s",r++, str1.GetPtr());
    str1.printf("(%d)%s",r++, str2.GetPtr());
    str2.printf("(%d)%s",r++, str1.GetPtr());
    str1.printf("(%d)%s",r++, str2.GetPtr());
    str2.printf("(%d)%s",r++, str1.GetPtr());

    if (str2 != "(8)(7)(6)(5)(4)(3)(2)1")
		return HoeTest::TEST_FAILED;

	return HoeTest::TEST_OK;
}

// dalsi definice
END_TEST(DynString)




