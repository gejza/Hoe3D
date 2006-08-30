
#include "StdAfx.h"
#include "../include/he/utils.h"
#include "../include/he/app.h"
#include "../include/he/editor.h"

wxString HoeEditor::FullPath(const wxString &dir, const wxString &relpath)
{
	wxString fp;
	fp = HoeUtils::fullpath(dir.c_str(), relpath.c_str()).c_str();
	return fp;
}

void HoeEditor::EditorTool::RightDown(const int x, const int y, const wxMouseEvent &e)
{
	App::Get()->GetEditor()->SetTool(NULL);
}

HoeEditor::EditorDragTool::EditorDragTool()
{
	m_leftdragging = false;
	m_rightdragging = false;
}

void HoeEditor::EditorDragTool::LeftDown(const int x, const int y, const wxMouseEvent & e)
{
	m_mouseleftx = e.GetX();
	m_mouselefty = e.GetY();
}

void HoeEditor::EditorDragTool::LeftUp(const int x, const int y, const wxMouseEvent & e)
{
	if (m_leftdragging)
	{
		m_leftdragging = false;
		OnLeftDrop(x,y);
	}
}

void HoeEditor::EditorDragTool::RightDown(const int x, const int y, const wxMouseEvent & e)
{
	m_mouserightx = e.GetX();
	m_mouserighty = e.GetY();
}

void HoeEditor::EditorDragTool::RightUp(const int x, const int y, const wxMouseEvent & e)
{
	if (m_rightdragging)
	{
		m_rightdragging = false;
		OnRightDrop(x,y);
	}
}

void HoeEditor::EditorDragTool::Move(int relX, int relY, int absX, int absY, const wxMouseEvent & ev)
{
	if (ev.LeftIsDown())
	{
		m_leftdragging = true;
		OnLeftDrag(m_mouseleftx, m_mouselefty);
	}
	if (ev.RightIsDown())
	{
		m_rightdragging = true;
		OnRightDrag(m_mouserightx, m_mouserighty);
	}
}

void HoeEditor::EditorDragTool::Leave()
{
	if (m_leftdragging)
	{
		m_leftdragging = false;
		OnLeftDrop(-1,-1);
	}
	if (m_rightdragging)
	{
		m_rightdragging = false;
		OnRightDrop(-1,-1);
	}
}


