
#include "StdAfx.h"
#include <hoe3d.h>
#include "../include/hoe_inputkeys.h"
#include "../include/hoe_gui.h"
#include "../include/hoe_engine.h"

BEGIN_HOEGAME

GuiItem::GuiItem()
{
	m_rect.left = 200.f;
	m_rect.top = 100.f;
	m_rect.right = 600.f;
	m_rect.bottom = 500.f;
}

void GuiItem::Set(const char * prop, const char * value)
{
	if (strcmp(prop,"rect")==0)
		SetRect(value);
}

void GuiItem::SetRect(const THoeRect * rect)
{
	m_rect = *rect;
}

void GuiItem::SetRect(const char * str)
{
	sscanf(str,"%f %f %f %f",&m_rect.left, &m_rect.top, &m_rect.right, &m_rect.bottom);
}

TextDevice::TextDevice()
{
	shift = 0;
}

void TextDevice::_KeyDown(int k)
{
	if (!Key(k))
		return;

	switch (k)
	{
	case HK_BACK:
        Back();
		break;
	case HK_LSHIFT:
		shift |= 2;
		break;
	case HK_RSHIFT:
		shift |= 1;
		break;
	default:
		if (shift && GetKeyDesc(k)->upper)
		{
			AddChar(GetKeyDesc(k)->upper);
			return;
		}
		else if (GetKeyDesc(k)->lower)
		{
			AddChar(GetKeyDesc(k)->lower);
			return;
		}
	}
}

void TextDevice::_KeyUp(int k)
{
	switch (k)
	{
	case HK_LSHIFT:
		shift &= ~2;
		break;
	case HK_RSHIFT:
		shift &= ~1;
		break;
	}
}

StaticPicture::StaticPicture()
{
  m_pic = NULL;
}

void StaticPicture::Set(const char * prop, const char *value)
{
	if (strcmp(prop,"picture") == 0)
	{
		// load
		char buff[256];
		sprintf(buff,"picture %s", value);
		m_pic = (IHoePicture*)HoeEngine::GetInstance()->Create(buff);
	}
	else 
		GuiItem::Set(prop, value);
}

void StaticPicture::Draw(IHoe2D * h2d)
{
	if (m_pic)
		h2d->Blt(&m_rect, m_pic);
}

void ColorRect::Set(const char * prop, const char *value)
{

}

void ColorRect::Draw(IHoe2D * h2d)
{
}

//////////////////////////////////////////////////////////////////

int InfoPanel::Info::comp(const void * v1,const void *v2)
{
	const Info * p1 = (Info*)v1;
	const Info * p2 = (Info*)v2;

	if (!p1->visible)
	{
		if (p2->visible)
			return 1;
		else
			return 0;
	}
	
	if (!p2->visible)
		return -1;

	if (p1->y < p2->y)
		return -1;
	else
		return 1;
}

InfoPanel::InfoPanel()
{
	font = NULL;
	memset(m_infos,0,sizeof(m_infos));
}

InfoPanel::~InfoPanel()
{
}

/*bool InfoPanel::Init(float min, float bottom, float left)
{
	font = (IHoeFont*)GetResMgr()->ReqResource(ID_INFO_FONT);

	stepsize = font->GetTextHeight();
	minheight = min;
	startheight = bottom - stepsize;

	return true;
}*/

void InfoPanel::Draw(IHoe2D * hoe2d)
{
	hoe2d->SetRect(800,600);
	float t = HoeEngine::GetInstance()->SysFloatTime();

	// vypsat info o objektu
	/*BecherObject * o = GetBecher()->GetLevel()->GetSelectedObject();
	if (o)
	{
		char buff[200];
		float top = 600.f-(o->GetNumInfos()+1)*20;
		for (int i=0; i < o->GetNumInfos();i++)
		{
			o->GetInfo(i, buff, sizeof(buff));
			font->DrawText(30, top, 0xff00cc00, buff);
			top += 20;
		}
	}*/

	// predelat cas by se mel odecitat 

	for (int i=0;i < MAX_INFOS;i++)
	{
		if (m_infos[i].visible)
		{
			const float tt = m_infos[i].totime - t;
			unsigned long alpha;
			if (tt < 0)
			{
				m_infos[i].visible = false;
				continue;
			}
			alpha = (unsigned long)(tt * float(0x80));
			if (alpha > 0xff) alpha = 0xff;

			font->DrawText(5,m_infos[i].y,((alpha & 0xff) << 24) | 0x00ffffff,m_infos[i].info);
		}
	}
	
	//return 0;
}

void InfoPanel::Add(const char * str)
{
	int a = -1;

	for (int i=0;i < MAX_INFOS;i++)
	{
		if (m_infos[i].visible)
		{
			// move up
			m_infos[i].y -= stepsize;
			if (m_infos[i].y < minheight)
			{
				m_infos[i].visible = false;
			}
			else
				continue;
		}
		
		if (a == -1)
		{
			a = i;
		}
	}

	if (a == -1) // get top list
	{
		a = 0;
		for (int i=0;i < MAX_INFOS;i++)
		{
			if (m_infos[i].visible && m_infos[a].y > m_infos[i].y)
			{
				a = i;
			}
		}
	}

	assert(a != -1);

	strcpy(m_infos[a].info,str);
	m_infos[a].y = startheight;
	m_infos[a].totime = HoeEngine::GetInstance()->SysFloatTime() + 5.f;
	m_infos[a].visible = true;

	//qsort(m_infos,MAX_INFOS,sizeof(Info),Info::comp);

	//GetConsole()->Con_Print(str);
}

void InfoPanel::Addf(const char * format, ...)
{
	char szBuff[512];

	va_list args;

	va_start(args, format);
#ifdef _WIN32
	_vsnprintf( szBuff, 512, format, args );
#else
	vsnprintf( szBuff, 512, format, args );
#endif
	va_end(args);

	Add(szBuff);
}

void InfoPanel::Set(const char *prop, const char *value)
{
}

/*void InfoPanel::Add(int id)
{
	Add(GetLang()->GetString(id));
}*/

////////////////////////////////////////////////
void DigiCounter::Draw(IHoe2D * d2)
{
		// draw digit
	IHoePicture * p = m_pic;//GetResMgr()->Get<IHoePicture>(ID_DIGITFONT);
	if (!p)
		return;
	const int np = 7;
	const int rl = m_rect.right;
	const int ll = m_rect.left;

	int i = m_value;
	bool sign = false;
	if (i < 0)
	{
		sign = true;
		i = -i;
	}

	const float pp = ((float)(rl-ll)) / np;
	float up = rl;
	if (i == 0)
	{
		THoeRect r = {up-pp, m_rect.top, up, m_rect.bottom};
		THoeRect r2={ 0, 0, 1*(60.0/64.0/4), 1*(60.0/64.0/3)};
		d2->Blt(&r, p, &r2);
		i = i / 10;
		up -= pp;
	}
	while (i != 0)
	{
		THoeRect r = {up-pp, m_rect.top, up, m_rect.bottom};
		int t=i%10;
		THoeRect r2={ (t%4)*(60.0f/64.0f/4), (t/4)*(60.0f/64.0f/3), (t%4+1)*(60.0f/64.0f/4), (t/4+1)*(60.0f/64.0f/3)};
		d2->Blt(&r, p, &r2);
		i = i / 10;
		up -= pp;
	}
	if (sign)
	{
		THoeRect r = {up-pp, m_rect.top, up, m_rect.bottom};
		THoeRect r2={ 2*(60.0/64.0/4), 2*(60.0/64.0/3), 3*(60.0/64.0/4), 3*(60.0/64.0/3)};
		d2->Blt(&r, p, &r2);
		i = i / 10;
		up -= pp;
	}
}

END_HOEGAME

