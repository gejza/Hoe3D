
#ifndef _HOE_FONT_
#define _HOE_FONT_

#include "../include/hoeinterfaces.h"
#include "unicode.h"

//#include "hoe_texture.h"

// Font rendering flags
#define HOEFONT_CENTERED_X  0x0001
#define HOEFONT_CENTERED_Y  0x0002
#define HOEFONT_TWOSIDED    0x0004
#define HOEFONT_FILTERED    0x0008
#define HOEFONT_BACK		0x0010

class HoeTexture;
class HoePicture;

struct HoeFontCoord
{
	vfloat prex,postx;
	vfloat top;
	vfloat x1,y1;
	vfloat x2,y2;
};

struct THoeFontCharInfo
{
	uint width; // sirka znaku
	uint pitch; // posun
	uint height; // vyska znaku
	int left; // posun znaku
	int top; // posun znaku dolu
	int movex; // odsun znaku
};

/** abstract class for render fonts */
/*class HoeFontRenderer
{
public:
	virtual bool GetCharInfo(wchar_t c, THoeFontCharInfo* info) = 0;
	virtual void Render(wchar_t ch,uint texX, uint texY, HOEFORMAT format,
		vfloat sc_alpha, 
		HoeTexture::LOCKRECT *lr) = 0;
	virtual void Release() {}
};*/

class HoeFont : public IHoeFont
{
public:
    tchar   m_strFontName[80];            // Font properties
    dword   m_dwFontHeight;
    dword   m_dwFontFlags;
	dword	m_dwSpacing;
	vfloat m_scalpha;

    //dword   m_dwTexWidth;                 // Texture dimensions
    //dword   m_dwTexHeight;

	HoeFontCoord m_fTexCoords[256];
	HoeTexture * m_tex;
	HoePicture * m_pic;

	CodePage m_cp;

	void CreateTexture();

	//HoeFontRenderer * GetFontRenderer(const tchar* fname, vfloat height) { return 0; }
public:
	HoeFont(const tchar* strFontName, uint dwHeight, vfloat scaplha, dword dwFlags=0L);
	HoeFont(HoePicture* pic, HoeRes::Res::FontInfo::FD* fd, size_t num);
	~HoeFont();

	// Initializing and destroying device-dependent objects
    bool Init();
    bool Restore();
    void Invalidate();
    virtual void HOEAPI Delete();

	// public functions
    virtual bool DrawText( vfloat x, vfloat y, dword dwColor, 
                      const tchar* strText, dword dwFlags=0L );
	virtual void GetTextSize(const tchar *text,THoeFontSize * size);
	virtual vfloat GetTextHeight();
};

#endif // _HOE_FONT_

