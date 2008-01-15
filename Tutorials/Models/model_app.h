
#ifndef _FIRST_APP_H_
#define _FIRST_APP_H_

#include "../Common/tutorial.h"
#include "model.h"
#include <hoe_view.h>

struct DebugLight : public XHoeObject
{
	IHoeLight * light;
};

class ModelApp : public HoeTutorial
{
	HoeGame::ModelViewCtrl m_view;
	DebugLight m_l[17];
	Model * m_model;
public:
	Model * CreateObj(IHoeModel * m, float x, float y);
	bool LoadScene();
	ModelApp(HOE_INSTANCE instance, HoeGame::Console * con);
	virtual const tchar * GetAppName() { return T("ModelApp"); }
	virtual void OnUpdate(float time);

};

#endif // _FIRST_APP_H_

