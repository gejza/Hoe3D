
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

// umet u policek oznacovat open a closed
// pak uz nebrani nic v ceste

int main()
{
    float start = SysFloatTime();
    printf("Test Heap\n");
    HoeCore::Heap<int> heap;
    for (int i=0;i < 30000000;i++)
    {
        heap.Insert(rand());
    }
    printf("V halde je nyni %d cisel.\n",heap.Count());
    while (heap.Count() > 0)
    {
        if (heap.Count()%1000000==0)
            printf("%d\n", heap.GetHeap());
        heap.RemoveHeap();
    }
    printf("OK %f\n", (SysFloatTime()-start));
    
    return 0;
}

