
#ifndef _HOE_SCENE_H_
#define _HOE_SCENE_H_

#include "hoe_figure2d.h"

namespace HoeGame {

class GuiConsole;

class Scene
{
	GuiConsole * console;
public:
	virtual void Update(float dtime);
	virtual void OnSet();
	virtual void OnUnset();
};

} // namespace HoeGame

#endif // _HOE_SCENE_H_

