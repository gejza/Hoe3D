
#ifndef _OBJECT_INSPECTOR_
#define _OBJECT_INSPECTOR_

#include "object_controller.h"

class ObjectInspector
{
	HoeCore::Set<ObjectController*> m_obj;
	uint m_find;
	dword m_findflag;
public:
	ObjectInspector();
	int RegisterObject(XHoeObject *, dword flags);
	void UnregisterObject(XHoeObject *);

	void SetStreaming(dword flag);
	ObjectController * Object();
};

#endif // _OBJECT_INSPECTOR_

