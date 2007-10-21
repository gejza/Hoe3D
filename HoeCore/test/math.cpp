
#include "../src/StdAfx.h"
#include "test.h"

#include "../include/hoe_fixed.h"
#include "../include/hoe_math.h"

#include <time.h>


template<class TYPE> float compute(float c)
{
	TYPE a,b;
	a = c;
	b = tanf(a);
    return b;
}

DEFINE_TEST(MathFixed, "Decription of test")
{
	/*printf("float fix\n");
	float ab = 0.f;
	for (float f = 0.f;f < 1.61f;f+=0.01)
	{
		float a = compute<HoeMath::fixed>(f);
		float b = compute<float>(f);
		printf("%f -- %f %f - (%f)\n", f, b, a,a-b);
		ab += fabs(a-b);
	}
	printf("chyba: %f\n", ab);*/
	const int niter = 90000000;
	volatile float f1 = 0.5679f;
	HoeMath::fixed f2 = f1;
	int t1 = time(NULL);
	for (int i=0;i < niter;i++)
		f1 = sinf(f1);
	int t2 = time(NULL);
	for (int i=0;i < niter;i++)
		f2 = sinf(f2);
	int t3 = time(NULL);
	printf("%d: %f - %d: %f\n", t2-t1, f1, t3-t2, (float)f2);

	return HoeTest::TEST_OK;
}

// dalsi definice
END_TEST(MathFixed)






