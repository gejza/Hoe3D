
#include "system.h"
#include "../include/hoeinterfaces.h"
#include "object_inspector.h"

ObjectInspector::ObjectInspector()
{
	m_numobject = 0;
	m_find = 0;
}

int ObjectInspector::RegisterObject(XHoeObject * obj, dword flags)
{
	assert(m_numobject < MAX_OBJ);
	for (uint i = 0;i < m_numobject;i++)
		if (!m_objects[i].IsValid())
		{
			m_objects[i].Create(obj);
			return i;
		}

	m_objects[m_numobject++].Create(obj);
	obj->GetCtrl()->SetFlags(flags);
	return (m_numobject - 1);
}

void ObjectInspector::UnregisterObject(int id)
{
	m_objects[id].Unregister();
}

void ObjectInspector::SetStreaming(dword flag)
{
	m_find = 0;
	m_findflag = flag;
}

ObjectController * ObjectInspector::Object()
{
	for(;m_find < m_numobject;m_find++)
	{
		if (m_objects[m_find].IsValid() && (m_objects[m_find].GetFlags() & m_findflag))
			return &m_objects[m_find++];
	}
	return NULL;
}
