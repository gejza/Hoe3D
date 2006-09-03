
#ifndef _HOE_SCENE_H_
#define _HOE_SCENE_H_

#include "hoe_figure2d.h"

BEGIN_HOEGAME

class GuiConsole;

class Scene
{
	GuiConsole * console;
public:
	virtual void Update(float dtime);
	virtual void OnSet();
	virtual void OnUnset();
};

END_HOEGAME

#endif // _HOE_SCENE_H_

