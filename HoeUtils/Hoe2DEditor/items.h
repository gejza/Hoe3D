
/**
   @file    items.h
   @date    Aug 2006
   @version 1.0
   @brief   Soubor s deklaraci trid prvku.
*/

#ifndef _HOE_2D_EDITOR_ITEMS_H_
#define _HOE_2D_EDITOR_ITEMS_H_


class BaseItem : public wxTreeItemData, public HoeEditor::PropObject, public HoeGame::Gui::Base
{
	friend class FigureEdit;
protected:
	FigureEdit * m_owner;
	wxTreeItemId m_id;
public:
	BaseItem();
	virtual void Draw(IHoe2D * ) = 0;
	virtual void Select(HoeEditor::PropertyGrid *prop) {}
	virtual void OnChangeProp(int id, const HoeEditor::PropItem & pi);
	virtual void Resize(const float left, const float top, const float right, const float bottom);
	virtual void Save(FILE * f) {}
	virtual void Set(const char * prop, const char *value);
	virtual HoeGame::Gui::Item * GetGui() = 0;
	virtual const wxRect GetwxRect();
};

#define BASE_CONNECT_DEFINE virtual HoeGame::Gui::Item * GetGui() { return &m_base; } \
	virtual void Draw(IHoe2D * h2d) { m_base.Draw(h2d); } \
	virtual HoeGame::Gui::EType GetType() { return m_base.GetType(); }

class StaticItem : public BaseItem
{
protected:
	wxString m_strpic;
	bool m_alpha;
	HoeGame::Gui::StaticPicture m_base;
public:
	StaticItem();
	virtual void Select(HoeEditor::PropertyGrid *prop);
	virtual void OnChangeProp(int id, const HoeEditor::PropItem & pi);
	virtual void Save(FILE * f);
	virtual void Set(const char * prop, const char *value);

	BASE_CONNECT_DEFINE
};

class ColorRectItem : public BaseItem
{
protected:
	dword m_color;
	float m_alpha;
	bool m_full;
	HoeGame::Gui::StaticPicture m_base;
public:
	ColorRectItem();
	virtual void Select(HoeEditor::PropertyGrid *prop);
	virtual void OnChangeProp(int id, const HoeEditor::PropItem & pi);
	virtual void Save(FILE * f);
	virtual void Set(const char * prop, const char *value);

	BASE_CONNECT_DEFINE
};

class FontItem : public BaseItem
{
protected:
	HoeGame::Gui::Font m_base;
	wxString m_font;
	wxString m_string;
public:
	FontItem();
	virtual void Select(HoeEditor::PropertyGrid *prop);
	virtual void OnChangeProp(int id, const HoeEditor::PropItem & pi);
	virtual void Save(FILE * f);
	virtual void Set(const char * prop, const char *value);

	BASE_CONNECT_DEFINE
};

class ButtonItem : public BaseItem
{
protected:
	wxString m_strpic;
	HoeGame::Gui::Button m_base;
public:
	ButtonItem();
	virtual void Select(HoeEditor::PropertyGrid *prop);
	virtual void OnChangeProp(int id, const HoeEditor::PropItem & pi);
	virtual void Save(FILE * f);
	virtual void Set(const char * prop, const char *value);

	BASE_CONNECT_DEFINE
};

class InfoItem : public BaseItem
{
protected:
	HoeGame::Gui::InfoPanel m_base;
public:
	InfoItem();
	virtual void Select(HoeEditor::PropertyGrid *prop);
	virtual void OnChangeProp(int id, const HoeEditor::PropItem & pi);
	virtual void Save(FILE * f);
	virtual void Set(const char * prop, const char *value);

	BASE_CONNECT_DEFINE
};

class DigiCounterItem : public BaseItem
{
protected:
	wxString m_strpic;
	HoeGame::Gui::DigiCounter m_base;
	int m_value;
public:
	DigiCounterItem();
	virtual void Select(HoeEditor::PropertyGrid *prop);
	virtual void OnChangeProp(int id, const HoeEditor::PropItem & pi);
	virtual void Save(FILE * f);
	virtual void Set(const char * prop, const char *value);

	BASE_CONNECT_DEFINE
};


#endif // _HOE_2D_EDITOR_ITEMS_H_

