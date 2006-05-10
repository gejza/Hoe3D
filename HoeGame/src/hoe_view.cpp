
#include "StdAfx.h"
#include "../include/hoe_view.h"

BEGIN_HOEGAME

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


void StrategyView::SetTargetPosition(float x, float y)
{
	m_target[0] = x;
	m_target[1] = 0;
	m_target[2] = y;

	Update();
}

void StrategyView::SetAngle(float a)
{
	angle = a;

	Update();
}

void StrategyView::SetDistance(float dist)
{
	m_mapDist = dist;

	Update();
}

void StrategyView::Update()
{
	float l[3];
	l[0] = (float)sin(angle);
	l[1] = -1.f;
	l[2] = (float)cos(angle);

	float p[3] = { -l[0] * m_mapDist + m_target[0],-l[1] * m_mapDist + m_target[1],-l[2] * m_mapDist + m_target[2] };

	GetCamera()->Set(p,l);
}

void StrategyView::Rotate(float a)
{
	angle += a;
	Update();
}

void StrategyView::Zoom(float d)
{
	m_mapDist += d;
	Update();
}

void StrategyView::Move(float straight, float side)
{
	m_target[0] += side * cosf(-angle) - straight * sinf(-angle);
	m_target[2] += side * sinf(-angle) + straight * cosf(-angle);
	Update();
}

bool StrategyView::GetPick(float mx, float my, float *sx, float *sy)
{
	float dir[3];
	float orig[3];
	GetCamera()->Pick(mx,my,dir,orig);
	const float t = - orig[1] / dir[1];
	if (t > 0)
	{
		*sx = t * dir[0] + orig[0];
		*sy = t * dir[2] + orig[2];
		return true;
	}
	else
		return false;
}

Strategy::StgObject * StrategyView::SelObject(float mx, float my)
{
	float dir[3];
	float orig[3];
	GetCamera()->Pick(mx,my,dir,orig);
	assert(m_scene->GetScenePhysics());
	register XHoeObject * obj = m_scene->GetScenePhysics()->Ray(dir,orig);
	if (obj)
		return reinterpret_cast<Strategy::StgObject *>(obj);
	else
		return NULL;
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
	float l[3];
	l[0] = cosf(m_arcangle) * sinf(m_angle);
	l[1] = sinf(m_arcangle);
	l[2] = cosf(m_arcangle) * cosf(m_angle);

	float p[3] = { -l[0] * m_distance + m_target[0],-l[1] * m_distance + m_target[1],-l[2] * m_distance + m_target[2] };

	GetCamera()->Set(p,l);
}

void ModelView::SetTargetPosition(float x, float y, float z)
{
	m_target[0] = x;
	m_target[1] = y;
	m_target[2] = z;
	Update();
}

void ModelView::SetDistance(float dist)
{
	m_distance = dist;
	Update();
}

void ModelView::MoveDistance(float dist)
{
	m_distance += dist;
	if (m_distance < 0)
		m_distance = 0;
	Update();
}

void ModelView::SetAngle(float a)
{
	m_angle = a;
	Update();
}

void ModelView::Rotate(float a)
{
	m_angle += a;
	Update();
}

void ModelView::SetArcAngle(float a)
{
	m_arcangle = a;
	Update();
}

void ModelView::RotateArc(float a)
{
	m_arcangle += a;
	if (m_arcangle > 1.5)
		m_arcangle = 1.5;
	if (m_arcangle < -1.5)
		m_arcangle = -1.5;
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
	float l[3];
	l[0] = cosf(m_arcangle) * sinf(m_angle) * m_zoom;
	l[1] = sinf(m_arcangle) * m_zoom;
	l[2] = cosf(m_arcangle) * cosf(m_angle) * m_zoom;

	this->GetCamera()->Set(m_pos, l);
}

void FreeCameraView::SetPosition(float x, float y, float z)
{
	m_pos[0] = x; m_pos[1] = y; m_pos[2] = z;
	Update();
}

void FreeCameraView::SetAngle(float a)
{
	m_angle = a;
	Update();
}

void FreeCameraView::Rotate(float a)
{
	m_angle += a;
	Update();
}

void FreeCameraView::SetArcAngle(float a)
{
	m_arcangle = a;
	Update();
}

void FreeCameraView::RotateArc(float a)
{
	m_arcangle += a;
	if (m_arcangle > 1.5)
		m_arcangle = 1.5;
	if (m_arcangle < -1.5)
		m_arcangle = -1.5;
	Update();
}

/*void FreeCameraView::SetZoom(float z);
void FreeCameraView::Zoom(float z);*/
void FreeCameraView::Move(float fw, float si)
{
	float l[3];
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
	/*switch (key)
	{
	case HK_W:
		Move(5,0);
		break;
	case HK_S:
		Move(-5,0);
		break;
	case HK_A:
		Move(0,-5);
		break;
	case HK_D:
		Move(0,5);
		break;
	default:
		return false;
	};

	return true;*/
	return false;
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
	switch (key)
	{
	case HK_W:
		Move(5,0);
		break;
	case HK_S:
		Move(-5,0);
		break;
	case HK_A:
		Move(0,-5);
		break;
	case HK_D:
		Move(0,5);
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

END_HOEGAME
