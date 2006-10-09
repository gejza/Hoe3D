
#include "StdAfx.h"
#include "utils.h"
#include "resmgr.h"
#include "shared.h"


ResourceMgr::ResourceMgr()
{
	SET_SHARED_PTR(resmgr);

	GetExec()->Register("files",ResourceMgr::c_list,NULL);
	GetExec()->Register("closefiles",ResourceMgr::c_closeall,NULL);

}

ResourceMgr::~ResourceMgr()
{
	UNSET_SHARED_PTR(resmgr);
}

/*IHoeTextureFile * ResourceMgr::GetTextureFile(HOE_HANDLE handle)
{
	return new TextureFile((uint)handle);
}

IHoeModelFile * ResourceMgr::GetModelFile(HOE_HANDLE handle)
{
	return new ModelFile((uint)handle);
}*/

int ResourceMgr::c_list(int argc, const char * argv[], void * param)
{
	GetFileSystem()->PrintList();
	return 0;
}

int ResourceMgr::c_closeall(int argc, const char * argv[], void * param)
{
	GetFileSystem()->CloseAll();
	return 0;
}


