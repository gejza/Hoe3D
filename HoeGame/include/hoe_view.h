
#ifndef _HOEGAME_VIEW_H_
#define _HOEGAME_VIEW_H_

#include "hoe_game.h"
#include "hoe_strategy.h"

BEGIN_HOEGAME

class View
{
protected:
	IHoeCamera * m_cam;
	IHoeScene * m_scene;
public:
	View();

	IHoeCamera * GetCamera() { assert(m_cam); return m_cam; }
	bool Init(IHoeScene * scene);
	
	void Update(float * pos, float * look);
};

class StrategyView : public View
{
	float m_mapDist;
	float m_target[3];
	float angle;
	bool m_tracked;
	float m_track_x;
	float m_track_y;
	float m_track_speed;
public:
	StrategyView();
	void SetTargetPosition(float x, float y);	
	void GetTargetPosition(float *x, float *y) { *x=m_target[0];*y=m_target[2];}
	void SetAngle(float a);
	void SetDistance(float dist);
	float GetDistance() { return m_mapDist; }
	void Update();
	void Rotate(float a);
	void Zoom(float d);
	void Move(float straight, float side);
	bool GetPick(float mx, float my, float *sx, float *sy);
	float GetAngle() { return angle; }
	Strategy::StgObject * SelObject(float mx, float my);
	void Update(const float dtime);
	void SetTrack(float x, float y, float speed);
};

class ModelView : public View
{
	float m_target[3];
	float m_angle;
	float m_arcangle;
	float m_distance;
protected:
	void Update();
public:
	ModelView();
	void SetTargetPosition(float x, float y, float z);
	void SetDistance(float dist);
	float GetDistance() { return m_distance; }
	void MoveDistance(float dist);
	void SetAngle(float a);
	void Rotate(float a);
	void SetArcAngle(float a);
	void RotateArc(float a);

};

class ModelViewCtrl : public ModelView, public XHoeMouse, public XHoeKeyboard
{
public:
	virtual void HOEAPI _Wheel(long);
	virtual void HOEAPI _MouseMove(float X, float Y);
	virtual void HOEAPI _KeyDown(int);
	bool KeyDown(int);
};

class FreeCameraView : public View
{
	float m_pos[3];
	float m_arcangle;
	float m_angle;
	float m_zoom;
protected:
	void Update();
public:
	FreeCameraView();
	void SetPosition(float x, float y, float z);
	void SetAngle(float a);
	void Rotate(float a);
	void SetArcAngle(float a);
	void RotateArc(float a);
	void SetZoom(float z);
	void Zoom(float z);
	void Move(float fw, float si);
};

class FreeCameraViewCtrl : public FreeCameraView, public XHoeMouse, public XHoeKeyboard
{
public:
	virtual void HOEAPI _Wheel(long);
	virtual void HOEAPI _MouseMove(float X, float Y);
	bool KeyDown(int);
};

END_HOEGAME

#endif // _HOEGAME_VIEW_H_

