
/**
   @file    figure.h
   @date    Aug 2006
   @version 1.0
   @brief   Soubor s definici tridy pro zobrazovani 2d.
*/

#include "StdAfx.h"
#include "figure.h"
#include "items.h"
#include <locale.h>

FigureEdit::FigureEdit(wxTreeCtrl * tc)
{
	m_tc = tc;
	m_scene = NULL;
	m_eng = NULL;
}

bool FigureEdit::Create(IHoe3DEngine * eng)
{
	m_scene = eng->CreateScene(HOETS_2D);
	eng->SetActiveScene(m_scene);
	m_scene->Set2DCallback(this);
	m_eng = eng;

	m_root = m_tc->AddRoot("root",-1,-1, NULL);

	return true;
}

void FigureEdit::Delete()
{
	m_tc->DeleteAllItems();
	if (m_eng->GetActiveScene()==m_scene)
		m_eng->SetActiveScene(NULL);
	m_scene->Delete();
	delete this;
}

void HOEAPI FigureEdit::_Paint(IHoe2D * h2d)
{
	h2d->SetRect(800,600);

	for (std::vector<BaseItem*>::iterator i=m_list.begin();i != m_list.end();i++)
		(*i)->Draw(h2d);

}

void FigureEdit::AddItem(BaseItem * item, wxString name)
{
	item->m_owner = this;
	item->m_id = m_tc->AppendItem(m_root,name,-1,-1, item);
	//m_tc->Set
	m_list.push_back(item);
	m_tc->SelectItem(item->m_id, true);
}

bool FigureEdit::OnSave(const wxString fname)
{
	setlocale(LC_NUMERIC, "C");

	FILE * f = fopen(fname.c_str(), "wt");
	if (!f)
		return false;

	wxTreeItemIdValue cook;
	wxTreeItemId item = m_tc->GetFirstChild(m_root, cook);
	while (item.IsOk())
	{
		wxTreeItemData * d = m_tc->GetItemData(item);
		if (d) dynamic_cast<BaseItem*>(d)->Save(f);
		item = m_tc->GetNextChild(m_root, cook);
	}

	fclose(f);
}

HoeGame::BaseGui * FigureEdit::CreateGUI(const char * type)
{
	wxString t = type;
	BaseItem * bi = NULL;
	if (t == "static")
		bi = new StaticItem;
	else if (t == "colorrect")
		bi = new ColorRectItem;
	else if (t == "button")
		bi = new ButtonItem;
	else if (t == "infopanel")
		bi = new InfoItem;
	else if (t == "digicounter")
		bi = new DigiCounterItem;

	if (bi == NULL)
		return NULL;
	AddItem(bi, "");
	return bi;
}