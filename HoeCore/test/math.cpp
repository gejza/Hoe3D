
#include "../src/StdAfx.h"
#include "test.h"

#include "../include/hoe_fixed.h"



DEFINE_TEST(MathFixed, "Decription of test")
{
	HoeMath::fixed a,b,c;
	a = 0.346f;
	b = 6;
	c = a * b;

	return HoeTest::TEST_OK;
}

// dalsi definice
END_TEST(MathFixed)






