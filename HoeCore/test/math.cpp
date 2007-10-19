
#include "../src/StdAfx.h"
#include "test.h"

#include "../include/hoe_fixed.h"
#include "../include/hoe_math.h"


template<class TYPE> float compute()
{
	TYPE a,b,c;
	a = 1.346f;
	b = 6;

    HoeMath::Vector2<TYPE> vec1(a,b);
    HoeMath::Vector2<TYPE> vec2(a-2,b-3);

    HoeMath::Vector2<TYPE> vec3 = vec1 + vec2;

    return vec3.MagnitudeF();
}

DEFINE_TEST(MathFixed, "Decription of test")
{
    printf("fixed: %f\n", compute<HoeMath::fixed>());
    printf("float: %f\n", compute<float>());
    printf("double: %f\n", compute<double>());
    printf("int: %f\n", compute<int>());

	return HoeTest::TEST_OK;
}

// dalsi definice
END_TEST(MathFixed)






