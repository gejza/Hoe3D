
/**
   @file    tools.h
   @date    Aug 2006
   @version 1.0
   @brief   Soubor s deklaraci tridy pro nastroje.
*/

#ifndef _HOE_2D_EDITOR_TOOLS_H_
#define _HOE_2D_EDITOR_TOOLS_H_

class BaseItem;

class ToolItem : public HoeEditor::EditorDragTool
{
protected:
	BaseItem * m_item;
public:
	ToolItem(BaseItem * item);
	virtual void Move(int relX, int relY, int absX, int absY, const wxMouseEvent & ev);
	virtual void OnLeftDrag(const int x, const int y);
	virtual void OnLeftDrop(const int x, const int y);
};

class ToolResize : public HoeEditor::EditorTool
{
};

#endif // _HOE_2D_EDITOR_TOOLS_H_

