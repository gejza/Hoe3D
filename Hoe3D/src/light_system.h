
#ifndef _LIGHT_SYSTEM_H_
#define _LIGHT_SYSTEM_H_

#include "light.h"

/** @todo Jine ulozeni svetel */
class LightSystem
{
	HoeLight * m_lights;
	HoeLight * m_index[32];
	uint m_active;
public:
	LightSystem();
	~LightSystem();
	void AddLight(HoeLight *l);
	void Setup();
	uint GetNumActiveLights() const;
	const HoeLight * GetActiveLight(uint i) const;
};


#endif // _LIGHT_SYSTEM_H_

