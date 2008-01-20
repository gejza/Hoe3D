
#include "../src/StdAfx.h"
#include <stdlib.h>
#include "../include/hoe_core.h"
#include "../include/hoe_structures.h"
#include "../include/hoe_platform.h"
#include "test.h"

struct TestItem
{
	int a,b,c,d;
    int aa[16];
	HoeCore::String str;
	TestItem()
	{
		printf("Constructor TestItem\n");
	}
	~TestItem()
	{
		printf("Destructor TestItem %d\n", a);
	}
	bool operator < (const TestItem& o) const
	{
		return a < o.a;
	}
	bool operator > (const TestItem& o) const
	{
		return a > o.a;
	}

};

DEFINE_TEST(Queue, "Test queue structure")
{
    // test queue
    HoeCore::Queue<TestItem> q;
    for (int i=0;i < 10;i++)
    {
		TestItem &ti = q.Push();
		ti.a = i;
		ti.b = i*2;
	}
	print("init..",q);
	q.Front();
	print("front..",q);
	TestItem ti;
	ti.a = 20;
	ti.b = 20;
	q.Push(ti);
	print("push 20,20..",q);
	q.Remove(1);
	print("remove 1 index..",q);
	q.Front(2);
	print("front2..", q);
	q.Remove(5);
	print("Remove 5 index..",q);
    
	return HoeTest::TEST_OK;
}

void print(const char *str, HoeCore::Queue<TestItem> &q)
{
	puts(str);
	for (uint i=0;i < q.Count();i++)
	{
		printf("(%d %d) ", q[i].a,q[i].b);
	}
	printf("\n");
}


END_TEST(Queue)

DEFINE_TEST(List, "Test list structure")
{
	HoeCore::MemoryPool pool;
	TestItem* ti = new (pool) TestItem;
	ti->~TestItem();
	HoeCore::List<TestItem> l;
	l.Add().a = 3;
	l.Add().a = 2;
	l.Add().a = 5;
	l.Add().a = 9;
	l.Add().a = 5;
	l.Add().a = 1;
	l.Add().a = 5;
	l.Add().a = 10;
	l.Add().a = 5;
	l.QSort();
	return HoeTest::TEST_OK;
}

END_TEST(List)

DEFINE_TEST(Endian, "Test big endian and low endian")
{
	void * src = (void*)"12345678";
	float f1 = 1.245f;
	float f2 = be_num<float>(f1);
	float f3 = be_num<float>(f2);
	printf("Float puv: %f - %f - %f\n",f1,f2,f3);
	Test<short>(src);
	Test<long>(src);
	Test<unsigned long>(src);
	Test<int>(src);
	Test<float>(src);
	Test<double>(src);
	Test<size_t>(src);

	return HoeTest::TEST_OK;
}

template <typename TYPE> void Test(void*src)
{
	char buff[256];
	TYPE n = *reinterpret_cast<TYPE*>(src);
	TYPE bn = be_num<TYPE>(n);
	TYPE ln = le_num<TYPE>(n);
	memset(buff, 0, 200);
	memcpy(buff, &bn, sizeof(TYPE));
	printf("%s: %s\n", "Big", buff);
	memcpy(buff, &ln, sizeof(TYPE));
	printf("%s: %s\n", "Low", buff);

}


END_TEST(Endian)


/*


bool test_efectivity(int cyc, int &npush, int &nfront, int &nrem)
{
    HoeCore::Queue<TestItem> q;
    // podle pravdepodobnosti vybrat akci
    // vlozit prvek
    // vybrat prvek
    // odstranit nektery z prostrednich prvku
    // drzet kolem 20 itemu
    for (int i=0;i < cyc;i++)
    {
        int r = HoeCore::RandInt(950,1050);
        int n = q.Count();
        if (r > n)
        {
            TestItem &ti = q.Push();
            ti.a = i;
            ti.b = n+r;
            npush++;
        }
        else
        {
            if (r % 2) {
                q.Front();
                nfront++;
            }
            else {
                q.Remove(HoeCore::RandInt(n));
                nrem++;
            }
        }
        //printf("%d ",n);
    }
    print("Ende ",q);
    return true;
}

int stmain(int argc, char * argv[])
{
    if (argc < 2)
        return 0;
    float start = SysFloatTime();
    int a=0, b=0, c=0;
    test_efectivity(atoi(argv[1]),a,b,c);
    float end = SysFloatTime();
    printf("End in %f second (push %d, front %d, rem %d)\n", end - start,a,b,c);
}

*/




