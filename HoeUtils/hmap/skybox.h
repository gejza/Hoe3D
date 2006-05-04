
#ifndef _MAP_SKYBOX_H_
#define _MAP_SKYBOX_H_

#include "part.h"

class SkyBox : public Unit
{
	std::string left;
	std::string right;
	std::string top;
	std::string bottom;
	std::string front;
	std::string back;
public:
    bool Process(HoeUtils::XMLNode *);
	void Destroy();
	virtual bool SaveToFile(HoeUtils::File *);
};

#endif // _MAP_SKYBOX_H_

