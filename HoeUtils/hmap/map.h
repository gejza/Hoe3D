

#ifndef _MAP_H_
#define _MAP_H_

#include "part.h"

class Map
{
	std::vector<Part*> parts;
	HoeUtils::XMLNode * cur;
	std::string mapname;
	std::string autor;
	std::string desc;
public:
	Map();
	bool Process(HoeUtils::XMLNode *);
	bool SaveToFile(HoeUtils::File *);
	virtual void ParseInfo(HoeUtils::XMLNode * node);
};


#endif // _MAP_H_

