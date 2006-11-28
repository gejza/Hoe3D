
#include "../src/StdAfx.h"
#include <stdlib.h>
#include "../include/hoe_core.h"
#include "../include/hoe_structures.h"

int main()
{
    printf("Test Heap\n");
    HoeCore::Heap<int> heap;
    for (int i=0;i < 10000000;i++)
    {
        heap.Insert(rand()%100);
    }
    printf("V halde je nyni %d cisel.\n",heap.Count());
    while (heap.Count() > 0)
    {
        if (heap.Count()%1000000==0)
            printf("%d\n", heap.GetHeap());
        heap.RemoveHeap();
    }
    printf("OK\n");
    
    return 0;
}

