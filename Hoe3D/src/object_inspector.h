
#ifndef _OBJECT_INSPECTOR_
#define _OBJECT_INSPECTOR_

#include "object_controller.h"

#define MAX_OBJ 1000

class ObjectInspector
{
	ObjectController m_objects[MAX_OBJ];
	uint m_numobject;
	uint m_find;
	dword m_findflag;
public:
	ObjectInspector();
	int RegisterObject(XHoeObject *, dword flags);
	void UnregisterObject(int);

	void SetStreaming(dword flag);
	ObjectController * Object();
};

#endif // _OBJECT_INSPECTOR_

