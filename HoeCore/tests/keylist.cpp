
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

int main()
{
    HoeCore::KeyList<Key, KeyIndex> kl;
    Key k1("a","acko");
    Key k2("b","becko");
    kl.Add(k1);
    kl.Add(k2);
    kl.Add(KeyIndex("c")).value = "cecko";
    
    printf("value=%s\n", kl.Find(KeyIndex("a"))->value);
    printf("value=%s\n", kl.Find(KeyIndex("b"))->value);
    printf("value=%s\n", kl.Find(KeyIndex("c"))->value);
    
    return 0;
}


