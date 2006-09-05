
#ifndef _HOE_GUI_UTILS_H_
#define _HOE_GUI_UTILS_H_

#include "hoe_game.h"

BEGIN_HOEGAME

class BaseGui
{
public:
	virtual void Set(const char * prop, const char *value) = 0;
};

class GuiItem : public BaseGui
{
protected:
	bool m_visible;
	THoeRect m_rect;
	const char * m_name;
public:
	GuiItem();
	virtual void Set(const char * prop, const char * value);
	virtual void Draw(IHoe2D * h2d) = 0;
	void SetRect(const THoeRect * rect);
	void SetRect(const char * value);
	const THoeRect & GetRect() const { return m_rect; }
	const char * GetName() const { return m_name; }
};

class TextDevice : public XHoeKeyboard
{
	int shift;
public:
	TextDevice();
	virtual void HOEAPI _KeyDown(int);
	virtual void HOEAPI _KeyUp(int);
	virtual void AddChar(char c) {};
	virtual void Back() {};
	virtual bool Key(int k) { return true; }
};

class StaticPicture : public GuiItem
{
protected:
	IHoePicture * m_pic;
public:
	StaticPicture();
	virtual void Set(const char * prop, const char *value);
	virtual void Draw(IHoe2D * h2d);
	void SetPicture(IHoePicture * pic) { m_pic = pic; }
};

class ColorRect : public GuiItem
{
public:
	virtual void Set(const char * prop, const char *value);
	virtual void Draw(IHoe2D * h2d);
}; 

#define MAX_INFOS 20

class InfoPanel : public GuiItem
{
	struct Info
	{
		bool visible;
		float y;
		float totime;
		char info[512];
		static int comp(const void *,const void *);
	};

	Info m_infos[MAX_INFOS];
	int act;
	float startheight;
	float stepsize;

	IHoeFont * m_font;

	float infoStartX,infoStartY;
	float infoX,infoY;

public:
	InfoPanel();
	~InfoPanel();

	bool Init(float min, float bottom, float left);
	virtual void Draw(IHoe2D * hoe2d);
	virtual void Set(const char * prop, const char *value);

	void Add(int id);
	void Add(const char *);
	void Addf(const char *, ...);
};

class Button : public StaticPicture
{
public:
	virtual void OnClick() {};
};

class DigiCounter : public StaticPicture
{
protected:
	int *m_value;
public:
	DigiCounter() { m_value = NULL; }
	virtual void Draw(IHoe2D * hoe2d);
	//virtual void Set(const char * prop, const char *value);
	void Set(int *value) { m_value = value; }
};

class Font : public GuiItem
{
protected:
	IHoeFont * m_font;
	const char * m_ptr;
public:
	Font() { m_font = NULL;m_ptr = NULL; }
	virtual void Draw(IHoe2D * hoe2d);
	virtual void Set(const char * prop, const char *value);
	void SetFont(IHoeFont * font) { m_font = font; }
	void SetPtr(const char * ptr) { m_ptr = ptr; }
};

END_HOEGAME

#endif // _HOE_GUI_UTILS_H_

