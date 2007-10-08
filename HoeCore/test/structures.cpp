
#include "../src/StdAfx.h"
#include <stdlib.h>
#include "../include/hoe_core.h"
#include "../include/hoe_structures.h"

#include <sys/time.h>

////////// Timer /////////////////

float SysFloatTime()
{
   struct timeval ctime;

   gettimeofday(&ctime, NULL);

   return ((ctime.tv_sec % 100000) + ctime.tv_usec * 0.000001f);
}

struct TestItem
{
	int a,b,c,d;
    int aa[16];
};

void print(const char *str, HoeCore::Queue<TestItem> &q)
{
	puts(str);
	for (uint i=0;i < q.Count();i++)
	{
		printf("(%d %d) ", q[i].a,q[i].b);
	}
	printf("\n");
}

bool test_func()
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
	TestItem ti = {20,20};
	q.Push(ti);
	print("push 20,20..",q);
	q.Remove(1);
	print("remove 1 index..",q);
	q.Front(2);
	print("front2..", q);
	q.Remove(5);
	print("Remove 5 index..",q);
    
    return true;
}

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

int main(int argc, char * argv[])
{
    if (argc < 2)
        return 0;
    float start = SysFloatTime();
    int a=0, b=0, c=0;
    test_efectivity(atoi(argv[1]),a,b,c);
    float end = SysFloatTime();
    printf("End in %f second (push %d, front %d, rem %d)\n", end - start,a,b,c);
}






