
#ifndef _HOE_ADVENTURE_H_
#define _HOE_ADVENTURE_H_

#include "hoe_object.h"
#include "hoe_view.h"

namespace HoeGame {
namespace Adventure {

class Kulission : public BaseObject
{
	IHoeModel * m_model;
public:
	Kulission(IHoeScene * scn);
	virtual ~Kulission();
	void SetSize(float width, float height);
	void SetTexture(const tchar * texname);
	void SetOverlap(int i);
};

class Theatre
{
	IHoeScene * m_scene;
	HoeGame::StrightCameraViewCtrl m_camera;
public:
	Theatre();
	virtual ~Theatre();
	bool Create();
	IHoeScene * GetScene() { return m_scene; }
	bool Load(const tchar * fname);
};

};
} // namespace HoeGame

#endif // _HOE_ADVENTURE_H_

