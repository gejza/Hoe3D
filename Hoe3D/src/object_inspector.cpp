
#include "StdAfx.h"
#include "../include/hoeinterfaces.h"
#include "object_inspector.h"

// textury, mazani

ObjectInspector::ObjectInspector()
{
	m_find = 0;
}

int ObjectInspector::RegisterObject(XHoeObject * obj, dword flags)
{
	ObjectController * c = new ObjectController;
	c->Create(obj);
	c->SetFlags(flags);
	m_obj.Add(c);
	return 0;
}

void ObjectInspector::UnregisterObject(XHoeObject * obj)
{
	ObjectController * c = m_obj.Remove(obj);
	if (c)
		c->Unregister();
}

void ObjectInspector::SetStreaming(dword flag)
{
	m_find = 0;
	m_findflag = flag;
}

ObjectController * ObjectInspector::Object()
{
	for(;m_find < m_obj.Count();m_find++)
	{
		if (m_obj[m_find]->GetFlags() & m_findflag)
			return m_obj[m_find++];
	}
	return NULL;
}
