
#ifndef _HOE_GUI_UTILS_H_
#define _HOE_GUI_UTILS_H_

#include "hoe_game.h"

BEGIN_HOEGAME
namespace Gui {

enum EType
{
	ENone,
	EStatic,
	EColorRect,
	EButton,
	EInfoPanel,
	EDigiCounter,
	EText,
};

class Base
{
public:
	virtual EType GetType() = 0; 
	virtual void Set(const char * prop, const char *value) = 0;
};

class Item : public Base
{
protected:
	bool m_show;
	THoeRect m_rect;
	const char * m_name;
public:
	Item();
	virtual void Set(const char * prop, const char * value);
	virtual void Draw(IHoe2D * h2d) = 0;
	void SetRect(const THoeRect * rect);
	void SetRect(const char * value);
	const THoeRect & GetRect() const { return m_rect; }
	const char * GetName() const { return m_name; }
	void Show() { m_show = true; }
	void Hide() { m_show = false; }
	bool GetShow() { return m_show; }
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

class StaticPicture : public Item
{
protected:
	IHoePicture * m_pic;
public:
	StaticPicture();
	virtual EType GetType() { return EStatic; }
	virtual void Set(const char * prop, const char *value);
	virtual void Draw(IHoe2D * h2d);
	void SetPicture(IHoePicture * pic) { m_pic = pic; }
	IHoePicture * GetPicture() { return m_pic; }
};

class ColorRect : public Item
{
public:
	virtual EType GetType() { return EColorRect; }
	virtual void Set(const char * prop, const char *value);
	virtual void Draw(IHoe2D * h2d);
}; 

#define MAX_INFOS 20

class InfoPanel : public Item
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
	virtual EType GetType() { return EInfoPanel; }

	bool Init(float min, float bottom, float left);
	virtual void Draw(IHoe2D * hoe2d);
	virtual void Set(const char * prop, const char *value);

	void Add(int id);
	void Add(const char *);
	void Addf(const char *, ...);
};

class Button : public StaticPicture
{
protected:
	bool m_active;
public:
	Button();
	virtual EType GetType() { return EButton; }
	virtual void Draw(IHoe2D * h2d);
	virtual void OnClick() {};
	bool Move(const float x, const float y);
	bool Click(const float x, const float y);
};

class DigiCounter : public StaticPicture
{
protected:
	int *m_value;
public:
	DigiCounter() { m_value = NULL; }
	virtual EType GetType() { return EDigiCounter; }
	virtual void Draw(IHoe2D * hoe2d);
	//virtual void Set(const char * prop, const char *value);
	void Set(int *value) { m_value = value; }
};

class Font : public Item
{
protected:
	IHoeFont * m_font;
	const char * m_text;
public:
	Font() { m_font = NULL;m_text = NULL; }
	~Font();
	virtual EType GetType() { return EText; }
	virtual void Draw(IHoe2D * hoe2d);
	virtual void Set(const char * prop, const char *value);
	void SetFont(IHoeFont * font) { m_font = font; }
	void SetText(const char * ptr);
};

} // namespace Gui
END_HOEGAME

#endif // _HOE_GUI_UTILS_H_

