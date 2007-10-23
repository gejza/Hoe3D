
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
	vfloat m_mapDist;
	vfloat m_target[3];
	vfloat angle;
	bool m_tracked;
	vfloat m_track_x;
	vfloat m_track_y;
	vfloat m_track_speed;
public:
	StrategyView();
	void SetTargetPosition(vfloat x, vfloat y);	
	void GetTargetPosition(vfloat *x, vfloat *y) { *x=m_target[0];*y=m_target[2];}
	void SetAngle(vfloat a);
	void SetDistance(vfloat dist);
	vfloat GetDistance() { return m_mapDist; }
	void Update();
	void Rotate(vfloat a);
	void Zoom(vfloat d);
	void Move(vfloat straight, vfloat side);
	bool GetPick(vfloat mx, vfloat my, vfloat *sx, vfloat *sy);
	float GetAngle() { return angle; }
	Strategy::StgObject * SelObject(vfloat mx, vfloat my);
	void Update(const vfloat dtime);
	void SetTrack(vfloat x, vfloat y, vfloat speed);
};

class ModelView : public View
{
	vfloat m_target[3];
	vfloat m_angle;
	vfloat m_arcangle;
	vfloat m_distance;
protected:
	void Update();
public:
	ModelView();
	void SetTargetPosition(vfloat x, vfloat y, vfloat z);
	void SetDistance(vfloat dist);
	vfloat GetDistance() { return m_distance; }
	void MoveDistance(vfloat dist);
	void SetAngle(vfloat a);
	void Rotate(vfloat a);
	void SetArcAngle(vfloat a);
	void RotateArc(vfloat a);

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
	vfloat m_pos[3];
	vfloat m_arcangle;
	vfloat m_angle;
	vfloat m_zoom;
protected:
	void Update();
public:
	FreeCameraView();
	void SetPosition(vfloat x, vfloat y, vfloat z);
	void SetAngle(vfloat a);
	void Rotate(vfloat a);
	void SetArcAngle(vfloat a);
	void RotateArc(vfloat a);
	void SetZoom(vfloat z);
	void Zoom(vfloat z);
	void Move(vfloat fw, vfloat si);
};

class FreeCameraViewCtrl : public FreeCameraView, public XHoeMouse, public XHoeKeyboard
{
public:
	virtual void HOEAPI _Wheel(long);
	virtual void HOEAPI _MouseMove(float X, float Y);
	virtual void HOEAPI _KeyDown(int);
	bool KeyDown(int);
};

class StrightCameraView : public View
{
	vfloat m_pos[3];
	vfloat m_zoom;
protected:
	void Update();
public:
	StrightCameraView();
	void SetPosition(vfloat x, vfloat y, vfloat z);
	void SetZoom(vfloat z);
	void Zoom(vfloat z);
	void Move(vfloat fw, vfloat si);
};

class StrightCameraViewCtrl : public StrightCameraView, public XHoeMouse, public XHoeKeyboard
{
public:
	virtual void HOEAPI _Wheel(long);
	virtual void HOEAPI _MouseMove(float X, float Y);
	virtual void HOEAPI _KeyDown(int);
	bool KeyDown(int);
};

END_HOEGAME

#endif // _HOEGAME_VIEW_H_

