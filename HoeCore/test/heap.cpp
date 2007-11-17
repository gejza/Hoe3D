
#include "../src/StdAfx.h"
#include <stdlib.h>
#include "../include/hoe_core.h"
#include "../include/hoe_structures.h"


struct Tile
{
    bool in_open;
    bool in_closed;
    float g;
    float f;
};

struct PTile
{
    Tile * tile;
    inline bool operator < (const PTile & t)
    {
        return tile->f < t.tile->f;
    }
    inline bool operator > (const PTile & t)
    {
        return tile->f > t.tile->f;
    }

};

int hmain()
{
    HoeCore::Heap<PTile> open;
    PTile t;
    open.Insert(t);
    // pridat prvni
    while (open.Count()>0)
    {
        //
    }
    return 0;
}


