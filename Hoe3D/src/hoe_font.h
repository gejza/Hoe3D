
#ifndef _HOE_FONT_
#define _HOE_FONT_

#include "../include/hoeinterfaces.h"

// Font rendering flags
#define HOEFONT_CENTERED_X  0x0001
#define HOEFONT_CENTERED_Y  0x0002
#define HOEFONT_TWOSIDED    0x0004
#define HOEFONT_FILTERED    0x0008
#define HOEFONT_BACK		0x0010


struct HoeFontCoord
{
	float prex,postx;
	float top;
	float x1,y1;
	float x2,y2;
};

class HoeFont : public IHoeFont
{
public:
    tchar   m_strFontName[80];            // Font properties
    dword   m_dwFontHeight;
    dword   m_dwFontFlags;
	dword	m_dwSpacing;
	float m_scalpha;

    //dword   m_dwTexWidth;                 // Texture dimensions
    //dword   m_dwTexHeight;

	HoeFontCoord m_fTexCoords[256];
	class HoeTexture * m_tex;

	void CreateTexture();

public:
	HoeFont(const tchar* strFontName, uint dwHeight, float scaplha, dword dwFlags=0L);
	~HoeFont();

	// Initializing and destroying device-dependent objects
    bool Init();
    bool Restore();
    void Invalidate();
    virtual void HOEAPI Delete();

	// public functions
    virtual bool DrawText( float x, float y, dword dwColor, 
                      const tchar* strText, dword dwFlags=0L );
	virtual void GetTextSize(const tchar *text,THoeFontSize * size);
	virtual float GetTextHeight();
};

#endif // _HOE_FONT_

