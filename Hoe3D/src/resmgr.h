
#ifndef _HOE_RESOURCE_MANAGER_H_
#define _HOE_RESOURCE_MANAGER_H_

#include "filesystem.h"
#include "../include/hoeinterfaces.h"

class ResourceMgr : public IHoeResourceMgr
{
public:
	ResourceMgr();
	~ResourceMgr();

	static int c_list(int argc, const char * argv[], void * param);
	static int c_closeall(int argc, const char * argv[], void * param);

};



#endif // _HOE_RESOURCE_MANAGER_H_

