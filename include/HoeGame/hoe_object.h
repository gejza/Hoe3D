
#ifndef _HOE_BASE_OBJECT_H_
#define _HOE_BASE_OBJECT_H_

#include "hoe_game.h"

namespace HoeGame {

class BaseObject : public XHoeObject
{
protected:
	IHoeScene * m_scene;
public:
	BaseObject(IHoeScene * scn)
	{
		scn->RegisterObject(this);
		m_scene = scn;
	}
	virtual ~BaseObject()
	{
		m_scene->UnregisterObject(this);
	}
	inline IHoeScene * GetScene() { return m_scene; }
};

} // namespace HoeGame

#endif // _HOE_BASE_OBJECT_H_

