
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
}

void BaseItem::Resize(const float left, const float top, const float right, const float bottom)
{
	THoeRect rect;
	wxSize size = HoeEditor::EngineView::Get()->GetClientSize();
	rect.left = left * 800.f / size.GetWidth();
	rect.right = right * 800.f / size.GetWidth();
	rect.top = top * 600.f / size.GetHeight();
	rect.bottom = bottom * 600.f / size.GetHeight();
	GetGui()->SetRect(&rect);
}

const wxRect BaseItem::GetwxRect()
{
	THoeRect rect = GetGui()->GetRect();
	return wxRect(rect.left, rect.top, rect.right-rect.left+1,rect.bottom-rect.top+1);
}

void BaseItem::Set(const char * prop, const char *value)
{
	wxString p = prop;
	if (p == "name")
		m_owner->GetTreeCtrl()->SetItemText(m_id, value);
	else if (p == "rect")
		GetGui()->SetRect(value);
}

void BaseItem::OnChangeProp(int id, const HoeEditor::PropItem &pi)
{
	switch (id)
	{
	case 0:
		m_owner->GetTreeCtrl()->SetItemText(m_id, pi.GetString());
		break;
	case 4:
		{
			const wxRect r = pi.GetRect(); 
			Resize(r.GetLeft(),r.GetTop(),r.GetRight(),r.GetBottom());
			break;
		}
		break;
	}
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
	prop->AppendRect(4,_("Rect"), GetwxRect());
	prop->AppendBool(1, _("Fill"), m_full);
	prop->AppendCategory(_("Color Settings"));
	prop->AppendColor(2,_("Color"), m_color);
	prop->AppendFloat(3,_("Alpha"), m_alpha);
	prop->End();
}

void ColorRectItem::OnChangeProp(int id, const HoeEditor::PropItem &pi)
{
	switch (id)
	{
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
	default:
		BaseItem::OnChangeProp(id,pi);
	}
}

void ColorRectItem::Save(FILE * f)
{
	fprintf(f, "colorrect\n{\n");
	fprintf(f, "\tname = \"%s\"\n", m_owner->GetTreeCtrl()->GetItemText(m_id).c_str());
	fprintf(f,"\trect = %f %f %f %f\n", m_base.GetRect().left, m_base.GetRect().top, m_base.GetRect().right, m_base.GetRect().bottom);
	fprintf(f,"\tfull = %s\n", m_full ? "true":"false");
	fprintf(f,"\talpha = %f\n", m_alpha);
	fprintf(f,"\tcolor = %x\n", m_color & 0xffffff);
	fprintf(f,"}\n");
}

void ColorRectItem::Set(const char * prop, const char *value)
{
	wxString p = prop;
	if (p == "full")
		m_full = value[0] == 't';
	else if (p == "alpha")
		sscanf(value,"%f",&m_alpha);
	else if (p == "color")
		sscanf(value,"%x",&m_color);
	else
		BaseItem::Set(prop, value);
}

////////////////////////////////////////////////

StaticItem::StaticItem()
{
	m_alpha = false;
}

void StaticItem::Select(HoeEditor::PropertyGrid *prop)
{
	prop->Begin(this);
	prop->AppendCategory(_("Globals Settings"));
	prop->AppendString(0, _("Name"), m_owner->GetTreeCtrl()->GetItemText(m_id));
	prop->AppendRect(4,_("Rect"), GetwxRect());
	prop->AppendCategory(_("Picture Settings"));
	prop->AppendString(2,_("Picture"), m_strpic);
	//prop->AppendCategory(_("Color Settings"));
	//prop->AppendColor(2,_("Color"), m_color);
	prop->AppendBool(3,_("Alpha"), m_alpha);
	prop->End();
}

void StaticItem::OnChangeProp(int id, const HoeEditor::PropItem &pi)
{
	switch (id)
	{
	case 2:
		{
		wxString str;
		m_strpic = pi.GetString();
		str = wxString::Format("picture %s", (const char *)m_strpic.c_str());
		m_base.SetPicture((IHoePicture*)HoeEditor::EngineView::Get()->GetEngine()->Create(str.c_str()));
		}
		break;
	case 3:
		m_alpha = pi.GetBool();
		break;
	default:
		BaseItem::OnChangeProp(id,pi);
	}
}

void StaticItem::Save(FILE * f)
{
	fprintf(f, "static\n{\n");
	fprintf(f, "\tname = \"%s\"\n", m_owner->GetTreeCtrl()->GetItemText(m_id).c_str());
	fprintf(f,"\trect = %f %f %f %f\n", m_base.GetRect().left, m_base.GetRect().top, m_base.GetRect().right, m_base.GetRect().bottom);
	fprintf(f,"\talpha = %s\n", m_alpha ? "true":"false");
	fprintf(f,"\tpicture = \"%s\"\n", (const char*)m_strpic.c_str());
	fprintf(f,"}\n");
}

// 	hud = (IHoePicture * )eng->Create("picture hud");
/*void StaticItem::Draw(IHoe2D * h2d)
{
	if (m_pic)
	{
		h2d->SetAlpha(m_alpha);
		h2d->BltFast(m_left,m_right,m_top,m_bottom,m_pic);
	}
	else
		h2d->PaintRect(m_left,m_right,m_top,m_bottom,0xffffffff,false);
	//
}*/

void StaticItem::Set(const char * prop, const char *value)
{
	wxString p = prop;
	if (p == "picture")
	{
		wxString str;
		m_strpic = value;
		if (m_strpic == "")
			return;
		str = wxString::Format("picture %s", value);
		m_base.SetPicture((IHoePicture*)HoeEditor::EngineView::Get()->GetEngine()->Create(str.c_str()));
	}
	else
		BaseItem::Set(prop, value);
}

////////////////////////////////////////////////

InfoItem::InfoItem()
{
}

void InfoItem::Select(HoeEditor::PropertyGrid *prop)
{
	prop->Begin(this);
	/*prop->AppendCategory(_("Globals Settings"));
	prop->AppendString(0, _("Name"), m_owner->GetTreeCtrl()->GetItemText(m_id));
	prop->AppendRect(4,_("Rect"), wxRect(m_left, m_top, m_right-m_left+1,m_bottom-m_top+1));
	prop->AppendCategory(_("Picture Settings"));
	prop->AppendString(2,_("Picture"), m_strpic);
	//prop->AppendCategory(_("Color Settings"));
	//prop->AppendColor(2,_("Color"), m_color);
	prop->AppendBool(3,_("Alpha"), m_alpha);*/
	prop->End();
}

void InfoItem::OnChangeProp(int id, const HoeEditor::PropItem &pi)
{
	/*switch (id)
	{
	case 0:
		m_owner->GetTreeCtrl()->SetItemText(m_id, pi.GetString());
		break;
	case 2:
		{
		wxString str;
		m_strpic = pi.GetString();
		str = wxString::Format("picture %s", (const char *)m_strpic.c_str());
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
	}*/
}

void InfoItem::Save(FILE * f)
{
	/*fprintf(f, "static\n{\n");
	fprintf(f, "\tname = \"%s\"\n", m_owner->GetTreeCtrl()->GetItemText(m_id).c_str());
	fprintf(f,"\trect = %f %f %f %f\n", m_left, m_top, m_right, m_bottom);
	fprintf(f,"\talpha = %s\n", m_alpha ? "true":"false");
	fprintf(f,"\tpicture = \"%s\"\n", (const char*)m_strpic.c_str());
	fprintf(f,"}\n");*/
}

void InfoItem::Set(const char * prop, const char *value)
{
	/*wxString p = prop;
	if (p == "picture")
	{
		wxString str;
		m_strpic = value;
		if (m_strpic == "")
			return;
		str = wxString::Format("picture %s", value);
		m_pic = (IHoePicture*)HoeEditor::EngineView::Get()->GetEngine()->Create(str.c_str());
	}
	else
		BaseItem::Set(prop, value);*/
}

