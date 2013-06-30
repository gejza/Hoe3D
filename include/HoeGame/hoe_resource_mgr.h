
#ifndef _RESOURCE_MGR_H_
#define _RESOURCE_MGR_H_

#include "hoe_game.h"

namespace HoeGame {

class ResourceMgr
{
	struct Res
	{
		int id;
		IHoeResource * res;
	};
	Res m_resources[1000];
	int numres;
	bool sorted;
public:
	IHoeResource * ReqResource(int id);
	ResourceMgr();
	bool AddResource(int id,const tchar * cmd);
	void Optimize();
	IHoeResource * GetResource(int id);
	template<class I> inline I * Get(int id)
	{
		return (I*)GetResource(id);
	}

protected:
	int GetPosByID(int id);
	virtual void AddResource(int id, IHoeResource * res);
	virtual IHoeResource * MissingResource(int id);
};

} // namespace HoeGame

#endif // _RESOURCE_MGR_H_
