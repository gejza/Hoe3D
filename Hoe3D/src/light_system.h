
#ifndef _LIGHT_SYSTEM_H_
#define _LIGHT_SYSTEM_H_

#include "light.h"

/** @todo Jine ulozeni svetel */
class LightSystem
{
	HoeLight * m_lights;
public:
	LightSystem();
	~LightSystem();
	void AddLight(HoeLight *l);
	void Setup();
};


#endif // _LIGHT_SYSTEM_H_

