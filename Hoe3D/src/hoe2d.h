
#ifndef _HOE_2D_
#define _HOE_2D_

#include "../include/hoeinterfaces.h"
#include "freetype.h"

class HoeLog;
class HoePicture;

class Hoe2D : public IHoe2D
{
	bool in_progress;

	float m_maxX;
	float m_maxY;

	//HoeFreeType freetype;
public:
	Hoe2D();
	~Hoe2D();
	void Begin();
	void End();

	HoePicture * CreatePicture(const tchar *, HoeLog * log);

	//HoeFreeType * GetFreeType() { return &freetype; }
	
    // user func
	void SetRect(const float w,const float h);

	// paint func
	void PaintRect(const float l,const float r,const float t,const float b,unsigned long color,bool full);
	void PaintLine(float x1,float y1,float x2,float y2,unsigned long c);

	void BltFast(const float l,const float r,const float t,const float b,IHoePicture *);
	void Blt(const THoeRect * dest,IHoePicture *);
	void Blt(const THoeRect * dest,IHoePicture *,const THoeRect * src);
	virtual void SetAlpha(bool set);
};

#endif // _HOE_2D_
