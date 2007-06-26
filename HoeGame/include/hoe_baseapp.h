
#ifndef _HOE_BASE_APP_H_
#define _HOE_BASE_APP_H_

#include "hoe_game.h"

BEGIN_HOEGAME 

class HoeBaseApp
{
protected:
	static HoeBaseApp * this_;
public:
    HoeBaseApp();
    virtual ~HoeBaseApp() {}
	template<class C> static C * GetApp()
	{
		//assert(this_);
		return dynamic_cast<C*>(this_);
	}
};

//#define GET_THIS(app) app::GetApp<app>()
#define GET_THIS(app) ((app*)(app::this_))


END_HOEGAME

#endif // _HOE_BASE_APP_H_

