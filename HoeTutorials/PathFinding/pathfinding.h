
#ifndef _PATHFIND_APP_H_
#define _PATHFIND_APP_H_

#include "../Common/tutorial.h"
#include <hoe_core.h>
#include <hoe_input.h>
#include <hoe_ai.h>
#include <hoe_path.h>

using namespace HoeMath;
using namespace HoeCore;

class PathFindApp : public HoeTutorial , public XHoe2DCallback, HoeGame::MouseStdInput,
	HoeGame::KeyboardStdInput
{
	IHoeFont * font;
	HoeGame::AI::MapFindPath m_map;
	Vector2 m_from;
	Vector2 m_to;
	//HoeGame::AI::Path m_path;
	void Preprocess();
public:
	PathFindApp(HOE_INSTANCE instance, HoeGame::Console * con);
	virtual const char * GetAppName() { return "PathFindApp"; }
	bool LoadScene();
	virtual void HOEAPI _Paint(IHoe2D *);
	void OnMouseMove(float X, float Y);
	virtual void OnRightButtonUp();
	virtual void OnLeftButtonUp();
	virtual void OnKeyDown(int key);
};

#endif // _PATHFIND_APP_H_

