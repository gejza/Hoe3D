
#ifndef _HOE_STRATEGY_LEVEL_H_
#define _HOE_STRATEGY_LEVEL_H_

#include "hoe_game.h"
#include "hoe_view.h"
#include "hoe_strategy.h"

BEGIN_HOEGAME

namespace Strategy {

class Map
{
protected:
	enum State
	{
		LSSelect = 1,
	} m_state;

	IHoeScene * m_scene;
	HoeGame::StrategyView m_view;
public:
	Map();
	bool Create(IHoeScene * scn);
	HoeGame::StrategyView * GetView() { return &m_view; }
	void AddObject(float x, float y, StgObject * s);
	virtual void OnSelectObject(StgObject * obj);
	virtual void OnAddObject(StgObject * obj);
};

};

END_HOEGAME

#endif // _HOE_STRATEGY_LEVEL_H_

