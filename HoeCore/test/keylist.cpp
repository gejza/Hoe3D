
#include "../src/StdAfx.h"
#include <stdlib.h>
#include "../include/HoeCore/hoe_core.h"
#include "../include/HoeCore/hoe_structures.h"

#ifdef _LINUX
#include <sys/time.h>
#endif

////////// Timer /////////////////

float SysFloatTime()
{
#ifdef _LINUX
	struct timeval ctime;

	gettimeofday(&ctime, NULL);

	return ((ctime.tv_sec % 100000) + ctime.tv_usec * 0.000001f);
#endif
	return 0.f;
}

class KeyIndex
{
public:
    int hash;
    const char * str;
    KeyIndex(const char * s)
    {
        str = s;
        hash = HoeCore::HashString(s);
    }
};

struct Key
{
    int hash;
    const char * key;
    const char * value;
    bool operator == (const KeyIndex & k)
    {
        if (k.hash != hash)
            return false;
        return strcmp(key, k.str) == 0;
    }
    Key(const char * k, const char *v)
    {
        key = k;
        value = v;
        hash = HoeCore::HashString(key);
    }
    Key(const KeyIndex & k)
    {
        key = k.str;
        hash = k.hash;
    }
};

void FillSP(HoeCore::StringPool &sp)
{
	printf("smudla=%p\n", sp.Insert("smudla"));
	printf("rudla=%p\n", sp.Insert("rudla"));
	printf("jacob=%p\n", sp.Insert("jacob"));
}

int kmain()
{
    /*HoeCore::KeyList<Key, KeyIndex> kl;
    Key k1("a","acko");
    Key k2("b","becko");
    kl.Add(k1);
    kl.Add(k2);
    kl.Add(KeyIndex("c")).value = "cecko";
    
    printf("value=%s\n", kl.Find(KeyIndex("a"))->value);
    printf("value=%s\n", kl.Find(KeyIndex("b"))->value);
    printf("value=%s\n", kl.Find(KeyIndex("c"))->value);*/

	HoeCore::StringPool sp;
	FillSP(sp);
	printf("smudla=%p\n", sp.Insert("smudla"));
    
    return 0;
}


