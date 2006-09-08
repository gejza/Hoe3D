
#ifndef _PATHFIND_APP_H_
#define _PATHFIND_APP_H_

#include "../Common/tutorial.h"

class PathFindApp : public HoeTutorial , public XHoe2DCallback
{
	IHoeFont * f;
	IHoeFont * f2;
	IHoePicture * pic[4];
public:
	PathFindApp(HOE_INSTANCE instance, HoeGame::Console * con);
	virtual const char * GetAppName() { return "PathFindApp"; }
	bool LoadScene();
	virtual void HOEAPI _Paint(IHoe2D *);
};

#endif // _PATHFIND_APP_H_

