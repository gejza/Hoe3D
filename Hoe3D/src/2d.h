
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
	vfloat m_multX;
	vfloat m_multY;

	Hoe2D();
	~Hoe2D();
	void Begin();
	void End();

    // user func
	void SetRect(const vfloat w,const vfloat h);

	// paint func
	void PaintRect(const THoeRect* dest,unsigned long color,bool full);
	void PaintLine(vfloat x1,vfloat y1,vfloat x2,vfloat y2,unsigned long c);

	virtual void Blt(IHoePicture *,const THoeRect * dest,const THoeRect * src);
	virtual void Blt(IHoePicture *, vfloat x, vfloat y, int rx);
	void MultRect(const THoeRect &src, THoeRect* dest)
	{
		dest->top = src.top * m_multY;
		dest->bottom = src.bottom * m_multY;
		dest->left = src.left * m_multX;
		dest->right = src.right * m_multX;
	}
	virtual void SetAlpha(bool set);
};

#endif // _HOE_2D_
