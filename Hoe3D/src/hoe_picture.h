
#ifndef _HOE_PICTURE_
#define _HOE_PICTURE_

#include "../include/hoeinterfaces.h"

/* flags
 */
#define HOE_PICTURE_ARR 0x01

class HoePicture : public IHoePicture
{
	const HoeTexture * m_tex;
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
	uint GetNumRegions() const;

	void GetRect(THoeRect * rect) const;
	virtual unsigned int GetWidth() const;
	virtual unsigned int GetHeight() const;
	//virtual void Alpha(bool,DWORD);
	//virtual void Paint(float x1,float x2,float y1,float y2,int mer);
	//virtual void SetN(int nx,int ny);
	//virtual int GetNumRect();

	void SetSource(const TextureSystem::HoeTexture * tex) { m_tex = tex; }
	const TextureSystem::HoeTexture * GetSource() const { return m_tex;}

};

#endif // _HOE_PICTURE_
