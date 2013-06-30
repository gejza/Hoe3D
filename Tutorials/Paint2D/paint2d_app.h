
#ifndef _PAINT2D_APP_H_
#define _PAINT2D_APP_H_

#include "../Common/tutorial.h"

class Paint2DApp : public HoeTutorial , public XHoe2DCallback
{
	IHoeFont * f;
	IHoeFont * f2;
	//IHoePicture * pic[4];
public:
	Paint2DApp(HOE_INSTANCE instance, HoeGame::Console * con);
	virtual const char * GetAppName() { return "Paint2DApp"; }
	bool LoadScene();
	virtual void HOEAPI _Paint(IHoe2D *);
};

#endif // _PAINT2D_APP_H_

