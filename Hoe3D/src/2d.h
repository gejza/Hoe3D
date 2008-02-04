
#ifndef _HOE_2D_
#define _HOE_2D_

#include "../include/hoeinterfaces.h"

class HoeLog;
class HoePicture;

class Hoe2D : public IHoe2D
{
	bool in_progress;

	vfloat m_maxX;
	vfloat m_maxY;

	//HoeFreeType freetype;
public:
	Hoe2D();
	~Hoe2D();
	void Begin();
	void End();

    // user func
	void SetRect(const vfloat w,const vfloat h);

	// paint func
	void PaintRect(const THoeRect* dest,unsigned long color,bool full);
	void PaintLine(vfloat x1,vfloat y1,vfloat x2,vfloat y2,unsigned long c);

	void Blt(IHoePicture *,const THoeRect * dest,const THoeRect * src);
	virtual void SetAlpha(bool set);
};

#endif // _HOE_2D_
