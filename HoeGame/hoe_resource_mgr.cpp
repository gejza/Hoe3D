
#include "StdAfx.h"
#include "hoe_resource_mgr.h"
#include "hoe_app.h"

BEGIN_HOEGAME

ResourceMgr::ResourceMgr()
{
	sorted = true;
	numres = 0;
}

bool ResourceMgr::AddResource(int id,const char * cmd)
{
	IHoeResource* resource = (IHoeResource*)HoeApp::GetEngine()->Create(cmd);
	if (!resource)
		return false;

	AddResource(id,resource);

	return true;
}

void ResourceMgr::AddResource(int id, IHoeResource * res)
{
	int pos;
	// get id
	if ((pos = GetPosByID(id)) == -1)
		pos = numres++;

	m_resources[pos].id = id;
	m_resources[pos].res = res;

	sorted = false;
}

int ResourceMgr::GetPosByID(int id)
{
	for (int i=0;i < numres;i++)
		if (m_resources[i].id == id)
			return i;
	return -1;
}

IHoeResource * ResourceMgr::GetResource(int id)
{
	int pos = GetPosByID(id);
	if (pos == -1)
	{
		IHoeResource * res = MissingResource(id);
		if (res)
		{
			AddResource(id,res);
			return res;
		}
		else
		{
			return NULL;
		}
	}
	else
		return m_resources[pos].res;
}

IHoeResource * ResourceMgr::ReqResource(int id)
{
	IHoeResource * res = GetResource(id);
	if (res)
		return res;

	throw "Missing resource";
	return NULL;
}

IHoeResource * ResourceMgr::MissingResource(int id)
{
	HoeApp::GetConsole()->Printf("Resource id:%d requied..",id);
	return NULL;
}

END_HOEGAME
