
#include "StdAfx.h"
#include "tools.h"
#include "items.h"

ToolItem::ToolItem(BaseItem * item)
{
	m_item = item;
}

void ToolItem::Move(int relX, int relY, int absX, int absY, const wxMouseEvent & ev)
{
	HoeEditor::EditorDragTool::Move(relX,relY,absX,absY,ev);
	if (IsLeftDrag())
	{
		m_item->Resize(m_mouseleftx,m_mouselefty, absX,absY);
	}
}

void ToolItem::OnLeftDrag(const int x, const int y)
{
}

void ToolItem::OnLeftDrop(const int x, const int y)
{
	m_item->Resize(m_mouseleftx, m_mouselefty, x, y);
}



