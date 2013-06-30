
#ifndef _HOE_STRATEGY_H_
#define _HOE_STRATEGY_H_

#include "hoe_object.h"

namespace HoeGame {
namespace Strategy {

class StgObject : public BaseObject
{
public:
	StgObject(IHoeScene * scn);
	virtual ~StgObject();
};

class Level;

class SelectObjContainer
{
	StgObject * m_object;
public:
	float x,y;
	SelectObjContainer();
	bool IsSelectMode();
	void SetPosition(float _x, float _y);
	void Show();
	void Hide();
	StgObject * Build();
	void Cancel();
	void Set(StgObject * obj);
};

};
} // namespace HoeGame

#endif // _HOE_STRATEGY_H_

