
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

int main()
{
    HoeCore::Universal u;
    u = "aaa";
    
    printf("%s : %s\n", u.GetTypeName(), (const char*)u);
    return 0;
}


