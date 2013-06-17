
#include "StdAfx.h"
#include "../include/HoeGame/hoe_view.h"

namespace HoeGame {

View::View()
{
	m_scene = NULL;
	m_cam = NULL;
}

bool View::Init(IHoeScene * scene)
{
	m_scene = scene;
	m_cam = scene->GetCamera();
	if (m_cam == NULL)
		return false;
	return true;
}

//////////////////////////////////////////////////////
StrategyView::StrategyView()
{
	m_tracked = false;
}

void StrategyView::SetTargetPosition(vfloat x, vfloat y)
{
	m_target[0] = x;
	m_target[1] = 0;
	m_target[2] = y;
	m_tracked = false;
	Update();
}

void StrategyView::SetAngle(vfloat a)
{
	angle = a;
	m_tracked = false;
	Update();
}

void StrategyView::SetDistance(vfloat dist)
{
	m_mapDist = dist;
	m_tracked = false;
	Update();
}

void StrategyView::Update()
{
	vfloat l[3];
	l[0] = (vfloat)sin(angle);
	l[1] = -1.f;
	l[2] = (vfloat)cos(angle);

	vfloat p[3] = { -l[0] * m_mapDist + m_target[0],-l[1] * m_mapDist + m_target[1],-l[2] * m_mapDist + m_target[2] };

	GetCamera()->Set(p,l);
}

void StrategyView::Rotate(vfloat a)
{
	m_tracked = false;
	angle += a;
	Update();
}

void StrategyView::Zoom(vfloat d)
{
	m_tracked = false;
	m_mapDist += d;
	Update();
}

void StrategyView::Move(vfloat straight, vfloat side)
{
	m_tracked = false;
	m_target[0] += side * cosf(-angle) - straight * sinf(-angle);
	m_target[2] += side * sinf(-angle) + straight * cosf(-angle);
	Update();
}

bool StrategyView::GetPick(vfloat mx, vfloat my, vfloat *sx, vfloat *sy)
{
	HoeMath::Vector3v dir,orig;
	GetCamera()->Pick(mx,my,&dir,&orig);
	const vfloat t = - orig.y / dir.y;
	if (t > 0)
	{
		*sx = t * dir.x + orig.x;
		*sy = t * dir.z + orig.z;
		return true;
	}
	else
		return false;
}

Strategy::StgObject * StrategyView::SelObject(vfloat mx, vfloat my)
{
	HoeMath::Vector3v dir,orig;
	GetCamera()->Pick(mx,my,&dir,&orig);
	assert(m_scene->GetScenePhysics());
	register XHoeObject * obj = m_scene->GetScenePhysics()->Ray(dir,orig);
	if (obj)
		return dynamic_cast<Strategy::StgObject *>(obj);
	else
		return NULL;
}

void StrategyView::Update(const vfloat dtime)
{
	if (m_tracked)
	{
		// update
		vfloat mx = m_track_x - m_target[0];
		vfloat my = m_track_y - m_target[2];
		if (dtime * m_track_speed > 1.f)
		{
			m_target[0] = m_track_x;
			m_target[2] = m_track_y;
		}
		else
		{
			m_target[0] += mx * dtime * m_track_speed;
			m_target[2] += my * dtime * m_track_speed;
		}
		//if (abs(mx) < 0.1f && abs(my) < 0.1f)
		//	m_tracked = false;
		Update();
	}
}

void StrategyView::SetTrack(vfloat x, vfloat y, vfloat speed)
{
	m_tracked = true;
	m_track_x = x;
	m_track_y = y;
	m_track_speed = speed;
}

/////////////////////////////////////////////////////////////////////////////

ModelView::ModelView()
{
	memset(m_target,0,sizeof(float) * 3);
	m_angle = 0.f;
	m_arcangle = 0.f;
	m_distance = 1.f;
}

void ModelView::Update()
{
	vfloat l[3];
	l[0] = cosf(m_arcangle) * sinf(m_angle);
	l[1] = sinf(m_arcangle);
	l[2] = cosf(m_arcangle) * cosf(m_angle);

	vfloat p[3] = { -l[0] * m_distance + m_target[0],-l[1] * m_distance + m_target[1],-l[2] * m_distance + m_target[2] };

	GetCamera()->Set(p,l);
}

void ModelView::SetTargetPosition(vfloat x, vfloat y, vfloat z)
{
	m_target[0] = x;
	m_target[1] = y;
	m_target[2] = z;
	Update();
}

void ModelView::SetDistance(vfloat dist)
{
	m_distance = dist;
	Update();
}

void ModelView::MoveDistance(vfloat dist)
{
	m_distance += dist;
	if (m_distance < 0)
		m_distance = 0;
	Update();
}

void ModelView::SetAngle(vfloat a)
{
	m_angle = a;
	Update();
}

void ModelView::Rotate(vfloat a)
{
	m_angle += a;
	Update();
}

void ModelView::SetArcAngle(vfloat a)
{
	m_arcangle = a;
	Update();
}

void ModelView::RotateArc(vfloat a)
{
	m_arcangle += a;
	if (m_arcangle > 1.5f)
		m_arcangle = 1.5f;
	if (m_arcangle < -1.5f)
		m_arcangle = -1.5f;
	Update();
}

FreeCameraView::FreeCameraView()
{
	m_pos[0] = 0.f;m_pos[1] = 0.f;m_pos[2] = 0.f;
	m_arcangle = 0.f;
	m_angle = 0.f;
	m_zoom = 1.f;
}

void FreeCameraView::Update()
{
	vfloat l[3],s,c,sa,ca;
	sincosf(m_arcangle, &s, &c);
	sincosf(m_angle, &sa, &ca);
	l[0] = c * sa * m_zoom;
	l[1] = s * m_zoom;
	l[2] = c * ca * m_zoom;

	this->GetCamera()->Set(m_pos, l);
}

void FreeCameraView::SetPosition(vfloat x, vfloat y, vfloat z)
{
	m_pos[0] = x; m_pos[1] = y; m_pos[2] = z;
	Update();
}

void FreeCameraView::SetAngle(vfloat a)
{
	m_angle = a;
	Update();
}

void FreeCameraView::Rotate(vfloat a)
{
	m_angle += a;
	Update();
}

void FreeCameraView::SetArcAngle(vfloat a)
{
	m_arcangle = a;
	Update();
}

void FreeCameraView::RotateArc(vfloat a)
{
	m_arcangle += a;
	if (m_arcangle > 1.5f)
		m_arcangle = 1.5f;
	if (m_arcangle < -1.5f)
		m_arcangle = -1.5f;
	Update();
}

/*void FreeCameraView::SetZoom(float z);
void FreeCameraView::Zoom(float z);*/
void FreeCameraView::Move(vfloat fw, vfloat si)
{
	vfloat l[3];
	l[0] = cosf(m_arcangle) * sinf(m_angle) * m_zoom;
	l[1] = sinf(m_arcangle) * m_zoom;
	l[2] = cosf(m_arcangle) * cosf(m_angle) * m_zoom;


	m_pos[0] += l[0] * fw + sinf(m_angle+3.141592654f/2) * si;
	m_pos[1] += l[1] * fw;
	m_pos[2] += l[2] * fw + cosf(m_angle+3.141592654f/2) * si;

	this->GetCamera()->Set(m_pos, l);
}
////////////////////////////////////////////////////////////////////
bool ModelViewCtrl::KeyDown(int key)
{
	switch (key)
	{
	case HK_W:
		RotateArc(0.5f);
		break;
	case HK_S:
		RotateArc(-0.5f);
		break;
	case HK_A:
		Rotate(-0.5f);
		break;
	case HK_D:
		Rotate(0.5f);
		break;
	default:
		return false;
	};

	return true;
}

void ModelViewCtrl::_KeyDown(int key)
{
	KeyDown(key);
}

void ModelViewCtrl::_Wheel(long l)
{
}

void ModelViewCtrl::_MouseMove(float X, float Y)
{
	Rotate(0.01f * X);
	RotateArc(-0.01f * Y);
}
////////////////////////////////////////////////////////////////////
bool FreeCameraViewCtrl::KeyDown(int key)
{
	const float moveDistance = 5;
	const float rotateDistance = 0.1f;
	switch (key)
	{
	case HK_W:
		Move(moveDistance,0);
		break;
	case HK_S:
		Move(-moveDistance,0);
		break;
	case HK_A:
		Move(0,-moveDistance);
		break;
	case HK_D:
		Move(0,moveDistance);
		break;
	case HK_UP:
		RotateArc(rotateDistance);
		break;
	case HK_DOWN:
		RotateArc(-rotateDistance);
		break;
	case HK_LEFT:
		Rotate(-rotateDistance);
		break;
	case HK_RIGHT:
		Rotate(rotateDistance);
		break;

	default:
		return false;
	};

	return true;
}

void FreeCameraViewCtrl::_Wheel(long l)
{
}

void FreeCameraViewCtrl::_MouseMove(float X, float Y)
{
	Rotate(0.01f * X);
	RotateArc(-0.01f * Y);
}

void FreeCameraViewCtrl::_KeyDown(int key)
{
	KeyDown(key);
}

/*bool View::Init()
{
	m_cam = NULL;
//	m_cur = NULL;
	m_pos[0] = m_pos[1] = m_pos[2] = 0.f;
	angle = 0.f;
	m_look[0] = (float)sin(angle);
	m_look[1] = 1.f;
	m_look[2] = (float)cos(angle);
	curx = 200;
	cury = 200;
	map_dist = (float)Becher::GetSettings()->GetInt("game:mapdist",10);
	cam_dist = (float)Becher::GetSettings()->GetInt("game:camera_height",20);

	m_cam = (iHoeCamera *)Becher::GetEngine()->Create("camera");
	//m_cur = (iHoeCursor*)Becher::GetEngine()->Create("cursor");
	m_pos[0] = m_pos[1] = m_pos[2] = 0.f;
	angle = 0.f;
	m_look[0] = (float)sin(angle);
	m_look[1] = 1.f;
	m_look[2] = (float)cos(angle);
	m_cam->GetSize(&width,&height);
	curx = width / 2;
	cury = height / 2;

	Set();

	//m_cursor = m_cur->LoadCursor(0,0,"cursor");
	//m_cur->ShowCursor(m_cursor);
	//m_cur->SetPos(curx,cury);

	return true;
}

void View::Set()
{
	float pos[3] = { m_look[0] * cam_dist + m_pos[0],m_look[1] * cam_dist + m_pos[1],m_look[2] * cam_dist + m_pos[2] };
	float look[3] = { -m_look[0],-m_look[1],-m_look[2]};

	//m_cam->Set(pos,look);
}

void View::Rotate(float f)
{
	angle += f;
	m_look[0] = (float)sin(angle);
	m_look[2] = (float)cos(angle);
	Set();
}

void View::Move(float x,float y)
{
	m_pos[0] -= m_look[0] * y;
	m_pos[2] -= m_look[2] * y;
	m_pos[0] -= m_look[2] * x;
	m_pos[2] += m_look[0] * x;
	Set();
}

void View::MoveCursor(float x,float y)
{
	m_cam->GetSize(&width,&height);
	
	curx += (int)x;
	cury += (int)y;
	if (curx < 0)
		curx = 0;
	else if (curx >= width)
		curx = width-1;

	if (cury < 0)
		cury = 0;
	else if (cury >= height)
		cury = height - 1;

	m_cur->SetPos(curx,cury);
}

void View::Update(const float time)
{
	if (curx <= 0)
		Move(-map_dist * time,0);
	else if (curx >= (width-1))
		Move(map_dist * time,0);

	if (cury <= 0)
		Move(0,map_dist * time);
	else if (cury >= (height-1))
		Move(0,-map_dist * time);

}

float View::GetPick(const float z,float &x, float &y)
{
	static float orig[3];
	static float dir[3];

	m_cam->Pick((float)curx,(float)cury,dir,orig);

	const float t = (z-orig[1]) / dir[1];
	x = orig[0] + t * dir[0];
	y = orig[2] + t * dir[2];
	return t;
}

tObject * View::GetObject(const float x, const float y)
{
	static float orig[3];
	static float dir[3];

	m_cam->Pick((float)curx,(float)cury,dir,orig);

	return reinterpret_cast<tObject *>(Becher::GetPhys()->Coll(orig,dir,HOEPHYS_ONLYVISIBLE));
}

void View::Cursor(bool show)
{
	//assert(m_cur);
	//if (show)
//		m_cur->ShowCursor(m_cursor);
//	else
//		m_cur->ShowCursor(0);
}*/

////////////////////////////////////////////////////////////////////////////

StrightCameraView::StrightCameraView()
{
	m_pos[0] = 0.f;m_pos[1] = 0.f;m_pos[2] = 0.f;
	m_zoom = 1.f;
}

void StrightCameraView::Update()
{
	vfloat l[3];
	l[0] = 0;
	l[1] = 0;
	l[2] = m_zoom;

	this->GetCamera()->Set(m_pos, l);
}

void StrightCameraView::SetPosition(vfloat x, vfloat y, vfloat z)
{
	m_pos[0] = x; m_pos[1] = y; m_pos[2] = z;
	Update();
}

/*void FreeCameraView::SetZoom(float z);
void FreeCameraView::Zoom(float z);*/
void StrightCameraView::Move(vfloat fw, vfloat si)
{
	m_pos[0] += si;
	m_pos[1] += fw;
	m_pos[2] += 0;

	Update();
}

////////////////////////////////////////////////////////////////////
bool StrightCameraViewCtrl::KeyDown(int key)
{
	const float moveDistance = 5;
	switch (key)
	{
	case HK_UP:
	case HK_W:
		Move(moveDistance,0);
		break;
	case HK_DOWN:
	case HK_S:
		Move(-moveDistance,0);
		break;
	case HK_LEFT:
	case HK_A:
		Move(0,-moveDistance);
		break;
	case HK_RIGHT:
	case HK_D:
		Move(0,moveDistance);
		break;
	default:
		return false;
	};

	return true;
}

void StrightCameraViewCtrl::_Wheel(long l)
{
}

void StrightCameraViewCtrl::_MouseMove(float X, float Y)
{
	//Rotate(0.01f * X);
	//RotateArc(-0.01f * Y);
}

void StrightCameraViewCtrl::_KeyDown(int key)
{
	KeyDown(key);
}

} // namespace HoeGame
