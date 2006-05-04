
#ifndef _HOE_CHAR_MAP_H_
#define _HOE_CHAR_MAP_H_

struct HKeyDesc
{
	char lower;
	char upper;
	const char * text;
};

HKeyDesc * GetKeyDesc(int hkey);


#endif //_HOE_CHAR_MAP_H_
