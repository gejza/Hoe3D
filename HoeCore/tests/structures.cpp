
#include "../src/StdAfx.h"
#include <stdlib.h>
#include "../include/hoe_core.h"
#include "../include/hoe_structures.h"

//#include <sys/time.h>

////////// Timer /////////////////

float SysFloatTime()
{
   /*struct timeval ctime;

   gettimeofday(&ctime, NULL);

   return ((ctime.tv_sec % 100000) + ctime.tv_usec * 0.000001f);*/
	return 0;
}

struct TestItem
{
	int a,b;
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

int main()
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
    return 0;
}

