
/**
   @file    items.h
   @date    Aug 2006
   @version 1.0
   @brief   Soubor s deklaraci trid prvku.
*/

#ifndef _HOE_2D_EDITOR_ITEMS_H_
#define _HOE_2D_EDITOR_ITEMS_H_


class BaseItem : public wxTreeItemData, public HoeEditor::PropObject
{
	friend class FigureEdit;
protected:
	FigureEdit * m_owner;
	wxTreeItemId m_id;
	float m_top;
	float m_bottom;
	float m_left;
	float m_right;
public:
	BaseItem();
	virtual void HOEAPI _Paint(IHoe2D * ) = 0;
	virtual void Select(HoeEditor::PropertyGrid *prop) {}
	virtual void Resize(const float left, const float top, const float right, const float bottom);
};

class StaticItem : public BaseItem
{
protected:
	IHoePicture * m_pic;
	bool m_alpha;
public:
	StaticItem();
	virtual void HOEAPI _Paint(IHoe2D * );
	virtual void Select(HoeEditor::PropertyGrid *prop);
	virtual void OnChangeProp(int id, const HoeEditor::PropItem & pi);
};

class ColorRectItem : public BaseItem
{
protected:
	dword m_color;
	float m_alpha;
	bool m_full;
public:
	ColorRectItem();
	virtual void HOEAPI _Paint(IHoe2D * );
	virtual void Select(HoeEditor::PropertyGrid *prop);
	virtual void OnChangeProp(int id, const HoeEditor::PropItem & pi);
};

// dialog item
// muze mit dalsi poditemy
// pozadi

// picture item
// jen picture

// check item

// button

// rect

#endif // _HOE_2D_EDITOR_ITEMS_H_

