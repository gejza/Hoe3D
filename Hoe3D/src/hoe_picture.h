
#ifndef _HOE_PICTURE_
#define _HOE_PICTURE_

#include "../include/hoeinterfaces.h"

/* flags
 */
#define HOE_PICTURE_ARR 0x01

class HoePicture : public IHoePicture
{
	HoeTexture * m_tex;
	dword m_flags;
	union {
		struct {
		uint m_rX;
		uint m_rY;
		uint m_active;
		};
	};
public:
	HoePicture();
	void CreateRegions(uint x,uint y);
	void SetActiveRegion(uint);
	uint GetNumRegions();

	void GetRect(THoeRect * rect);
	virtual unsigned int GetWidth();
	virtual unsigned int GetHeight();
	//virtual void Alpha(bool,DWORD);
	//virtual void Paint(float x1,float x2,float y1,float y2,int mer);
	//virtual void SetN(int nx,int ny);
	//virtual int GetNumRect();

	void SetSource(TextureSystem::HoeTexture * tex) { m_tex = tex; }
	TextureSystem::HoeTexture * GetSource() { return m_tex;}

};

#endif // _HOE_PICTURE_
