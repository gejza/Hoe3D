
#include "../src/StdAfx.h"
#include <stdlib.h>
#include "../include/hoe_core.h"
#include "../include/hoe_structures.h"

int umain()
{
    HoeCore::Universal u;
    u = "aaa";
    
    printf("%s : %s\n", u.GetTypeName(), (const char*)u);
    return 0;
}


