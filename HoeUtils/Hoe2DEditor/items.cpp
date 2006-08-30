
/**
   @file    items.h
   @date    Aug 2006
   @version 1.0
   @brief   Soubor s definici trid prvku.
*/

#include "StdAfx.h"
#include "items.h"
#include "figure.h"

BaseItem::BaseItem()
{
	m_left = 200;
	m_right = 600;
	m_top = 100;
	m_bottom = 500;
}

void BaseItem::Resize(const float left, const float top, const float right, const float bottom)
{
	wxSize size = HoeEditor::EngineView::Get()->GetClientSize();
	m_left = left * 800.f / size.GetWidth();
	m_right = right * 800.f / size.GetWidth();
	m_top = top * 600.f / size.GetHeight();
	m_bottom = bottom * 600.f / size.GetHeight();
}

ColorRectItem::ColorRectItem()
{
	m_color = 0x80ffff00;
	m_alpha = 1.f;
	m_full = true;
}

void ColorRectItem::Select(HoeEditor::PropertyGrid *prop)
{
	prop->Begin(this);
	prop->AppendCategory(_("Globals Settings"));
	prop->AppendString(0, _("Name"), m_owner->GetTreeCtrl()->GetItemText(m_id));
	prop->AppendBool(1, _("Fill"), m_full);
	prop->AppendRect(4,_("Rect"), wxRect(m_left, m_top, m_right-m_left+1,m_bottom-m_top+1));
	prop->AppendCategory(_("Color Settings"));
	prop->AppendColor(2,_("Color"), m_color);
	prop->AppendFloat(3,_("Alpha"), m_alpha);
	prop->End();
}

void ColorRectItem::OnChangeProp(int id, const HoeEditor::PropItem &pi)
{
	switch (id)
	{
	case 0:
		m_owner->GetTreeCtrl()->SetItemText(m_id, pi.GetString());
		break;
	case 1:
		m_full = pi.GetBool();
		break;
	case 2:
		m_color = pi.GetColor();
		break;
	case 3:
		m_alpha = pi.GetFloat();
		if (m_alpha > 1.f) m_alpha = 1.f;
		if (m_alpha < 0) m_alpha = 0;
		break;
	case 4:
		{
			const wxRect r = pi.GetRect(); 
			m_top = r.GetTop();
			m_left = r.GetLeft();
			m_right = r.GetRight();
			m_bottom = r.GetBottom();
			break;
		}
		break;
	}
}

// 	hud = (IHoePicture * )eng->Create("picture hud");
void ColorRectItem::_Paint(IHoe2D * h2d)
{
	h2d->PaintRect(m_left,m_right,m_top,m_bottom,(0xffffff&m_color) | ((byte)(255*m_alpha)) << 24,m_full);
	//h2d->BltFast(0,100,0,100,hud);
}

////////////////////////////////////////////////

StaticItem::StaticItem()
{
	m_pic = NULL;
	m_alpha = false;
}

void StaticItem::Select(HoeEditor::PropertyGrid *prop)
{
	prop->Begin(this);
	prop->AppendCategory(_("Globals Settings"));
	prop->AppendString(0, _("Name"), m_owner->GetTreeCtrl()->GetItemText(m_id));
	prop->AppendRect(4,_("Rect"), wxRect(m_left, m_top, m_right-m_left+1,m_bottom-m_top+1));
	prop->AppendCategory(_("Picture Settings"));
	prop->AppendString(2,_("Picture"), "");
	//prop->AppendCategory(_("Color Settings"));
	//prop->AppendColor(2,_("Color"), m_color);
	prop->AppendBool(3,_("Alpha"), m_alpha);
	prop->End();
}

void StaticItem::OnChangeProp(int id, const HoeEditor::PropItem &pi)
{
	switch (id)
	{
	case 0:
		m_owner->GetTreeCtrl()->SetItemText(m_id, pi.GetString());
		break;
	case 2:
		{
		wxString str;
		str = wxString::Format("picture %s", (const char *)pi.GetString().c_str());
		m_pic = (IHoePicture*)HoeEditor::EngineView::Get()->GetEngine()->Create(str.c_str());
		}
		break;
	case 3:
		m_alpha = pi.GetBool();
		break;
	case 4:
		{
			const wxRect r = pi.GetRect(); 
			m_top = r.GetTop();
			m_left = r.GetLeft();
			m_right = r.GetRight();
			m_bottom = r.GetBottom();
			break;
		}
		break;
	}
}

// 	hud = (IHoePicture * )eng->Create("picture hud");
void StaticItem::_Paint(IHoe2D * h2d)
{
	if (m_pic)
	{
		h2d->SetAlpha(m_alpha);
		h2d->BltFast(m_left,m_right,m_top,m_bottom,m_pic);
	}
	else
		h2d->PaintRect(m_left,m_right,m_top,m_bottom,0xffffffff,false);
	//
}
