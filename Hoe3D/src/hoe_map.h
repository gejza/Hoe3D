
#ifndef _HOE_MAP_
#define _HOE_MAP_

#include "../include/hoeinterfaces.h"

//////////

class HoeMapPart;

class HoeMap
{
protected:
	char mapname[64];
	char autor[64];
	char desc[128];
	HoeMapPart * parts[128];
	int numParts;
public:
	HoeMap();
	void Clean();
	//bool Load(const char * name, XMapLoaderCallback *);
	HoeMapPart * GetPart(const char *id);

};

#endif // _HOE_MAP_
