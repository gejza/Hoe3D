
#ifndef _PATHFIND_APP_H_
#define _PATHFIND_APP_H_

#include "../Common/tutorial.h"
#include <hoe_core.h>
#include <hoe_input.h>

class PathFindApp : public HoeTutorial , public XHoe2DCallback, HoeGame::MouseStdInput,
	HoeGame::KeyboardStdInput
{
	HoeCore::ByteTileMap m_terrain;
	HoeMath::Polygon2 * m_poly;
	HoeCore::Set<HoeMath::VLine2Int> m_lines;
	void Preprocess();
public:
	PathFindApp(HOE_INSTANCE instance, HoeGame::Console * con);
	virtual const char * GetAppName() { return "PathFindApp"; }
	bool LoadScene();
	virtual void HOEAPI _Paint(IHoe2D *);
	void OnMouseMove(float X, float Y);
	virtual void OnRightButtonUp();
	virtual void OnKeyDown(int key);
};

#endif // _PATHFIND_APP_H_

