
#include "StdAfx.h"
#include "shared.h"
#include "utils.h"
#include "ref.h"
#include "config.h"
#include "states.h"
#include "hoe_format.h"
#include "hoe_texture.h"
#include "texture_system.h"
#include "hoe_font.h"
#include "unicode.h"
#include "hoe2d.h"
#ifndef _WIN32_WCE
#include "freetype.h"
#endif

HoeFont::HoeFont(const tchar* strFontName, uint dwHeight, vfloat scalpha, dword dwFlags)
{
	HoeCore::string::copy( m_strFontName, strFontName, sizeof(m_strFontName)/sizeof(tchar) );
    m_dwFontHeight         = dwHeight;
    m_dwFontFlags          = dwFlags;
    m_dwSpacing            = 0;
	m_scalpha = scalpha;

    m_tex = NULL;
}

HoeFont::~HoeFont()
{

}

bool HoeFont::Init()
{
	HoeFontRenderer * fr = GetFontRenderer(this->m_strFontName, this->m_dwFontHeight);
	if (!fr)
		return false;

	bool lformat = true;

	if (!m_tex)
		m_tex = new HoeTexture;

	dword width=256,height=256;
	HOEFORMAT format = HOE_L8A8;
	/* poladit formaty */
	GetConfig()->CheckTexture(width, height, format);
	if (format == HOE_B8G8R8A8)
		lformat = false;

	if (!this->m_tex->Create(width,height,format))
	{
		fr->Release();
		return false;
	}
	
	HoeTexture::LOCKRECT lr;
	m_tex->Lock(&lr);

    //dword x, y;
	uint texX = 0;
	uint texY = 0;
	uint maxY = 0;
	// vymazat
	for (texY=0;texY<256;texY++)
	{
		memset(lr.data + (texY * lr.pitch),0,256*(lformat?2:4));
	}
	texY = 0;

	memset(this->m_fTexCoords,0,sizeof(m_fTexCoords));

	for( int c = 0; c < GetCodePage()->GetNumChars(); c++ )
	{
		// vygenerovat font
		THoeFontCharInfo data;
		wchar_t ch = GetCodePage()->GetChar(c);
		if (!fr->GetCharInfo(ch, &data))
			continue;
		
		// jestli se nevejde nacnout novou radku
		if ((texX + data.width) > 256)
		{
			texY += maxY + 2;
			texX = 0;
			if (texY > 256)
			{
				Con_Print("Warning: font '%s' - texture 256x256 size failed.(%c)",this->m_strFontName,(char)c);
				break;
			}
		}
		
		if ((texY + data.height) > 256)
		{
			Con_Print("Warning: font '%s' - texture 256x256 size failed.(%c)",this->m_strFontName,(char)c);
			break;
		}

		if (data.width && data.height)
		{
			// prepsat obraz znaku
			fr->Render(ch, texX, texY, lformat ? HOE_A8L8:HOE_A8R8G8B8, m_scalpha, &lr);

			// zapsat tex souradnice
			this->m_fTexCoords[c].x1 = texX / 256.f;
			this->m_fTexCoords[c].x2 = (texX + data.width) / 256.f;
			this->m_fTexCoords[c].y1 = texY / 256.f;
			this->m_fTexCoords[c].y2 = (texY + data.height) / 256.f;

			texX += data.width + 2;
			if (data.height > maxY)
				maxY = data.height;
		}

		this->m_fTexCoords[c].prex = (float)data.left;
		this->m_fTexCoords[c].postx = (float)data.movex;
		this->m_fTexCoords[c].top = (float)data.top;
	}

    m_tex->Unlock();
	fr->Release();

	Con_Print("Create font: %s %d",this->m_strFontName,this->m_dwFontHeight);
    return true;
}

bool HoeFont::Restore()
{
	return true;
}

void HoeFont::Invalidate()
{
}

void HoeFont::Delete()
{
}

#ifdef _HOE_D3D_

struct FONT_VERTEX
{
	float x, y, z; 
	dword color;
	float       tu, tv;   // The texture coordinates
};

#define FONT_VERTEX_FVF (D3DFVF_XYZ|D3DFVF_DIFFUSE|D3DFVF_TEX1)

struct FONTBACK_VERTEX
{
	float x, y, z; 
	dword color;
};

#define FONTBACK_VERTEX_FVF (D3DFVF_XYZ|D3DFVF_DIFFUSE)

#endif

bool HoeFont::DrawText( vfloat sx, vfloat sy, dword dwColor, 
                      const tchar* strText, dword dwFlags )
{
	this->m_tex->Set();
	GetStates()->SetupFont();

#ifdef _HOE_OPENGL_
	glColor4f(GET_RED_FLOAT(dwColor),GET_GREEN_FLOAT(dwColor),GET_BLUE_FLOAT(dwColor),GET_ALPHA_FLOAT(dwColor));
#endif // _HOE_OPENGL_

	while( *strText )
    {
        int c = GetCodePage()->StringToIndex(strText);

		// gen text

		sx += m_fTexCoords[c].prex;

        float tx1 = m_fTexCoords[c].x1;
        float ty1 = m_fTexCoords[c].y1;
        float tx2 = m_fTexCoords[c].x2;
        float ty2 = m_fTexCoords[c].y2;

        float w = (tx2-tx1) *  256;
        float h = (ty2-ty1) *  256;

		float top = m_fTexCoords[c].top - 12;

        if( w > 0 && h > 0 )
        {

#ifdef _HOE_OPENGL_
			glBegin(GL_QUADS);// Zaèátek kreslení obdélníkù
			glTexCoord2f( tx1 , ty1 );glVertex2f( sx+0,sy+0-top );// Levý horní bod
			glTexCoord2f( tx2, ty1);glVertex2f( sx+w,sy+0-top);// Pravý horní bod
			glTexCoord2f( tx2 , ty2);glVertex2f( sx+w,sy+h -top);// Pravý dolní bod
			glTexCoord2f( tx1, ty2);glVertex2f( sx+0,sy+h-top);// Levý dolní bod
			glEnd();// Konec kreslení obdélníkù
#endif // _HOE_OPENGL_
#ifdef _HOE_D3D_
		FONT_VERTEX pv[] = 
		{
			{ sx+0-0.5f,sy+0-0.5f-top , 0.f, dwColor, tx1 , ty1}, 
			{ sx+w-0.5f,sy+0-0.5f-top , 0.f, dwColor, tx2, ty1}, 
			{ sx+w-0.5f,sy+h-0.5f-top , 0.f, dwColor, tx2 , ty2},
			{ sx+0-0.5f,sy+h-0.5f-top , 0.f, dwColor, tx1, ty2}
		};
		D3DDevice()->SetFVF(FONT_VERTEX_FVF);
		D3DDevice()->DrawPrimitiveUP(D3DPT_TRIANGLEFAN,2,pv,sizeof(FONT_VERTEX));
#endif // _HOE_D3D9_
        }

        sx += w/*m_fTexCoords[c-32].postx*/ - (2 * m_dwSpacing);
    }

	return true;
}

void HoeFont::GetTextSize(const tchar *text,THoeFontSize * size)
{
	size->width = 0.f;
	//y = 0.f;

	if (*text == '\0')
		return;

	int index = GetCodePage()->GetIndex('X');
	float ty1 = this->m_fTexCoords[index].y1;
	float ty2 = this->m_fTexCoords[index].y2;

	//y = (ty2-ty1) * 256;

	while (*text)
	{
		index = GetCodePage()->StringToIndex(text);

		size->width += m_fTexCoords[index].prex;

		float tx1 = this->m_fTexCoords[index].x1;
        float tx2 = this->m_fTexCoords[index].x2;

        float w = (tx2-tx1) *  256;
 
        size->width += w - (2 * m_dwSpacing);
	}
	size->width += 2 * m_dwSpacing;
}

vfloat HoeFont::GetTextHeight()
{
	int index = GetCodePage()->GetIndex('X');
	vfloat ty1 = this->m_fTexCoords[index].y1;
	vfloat ty2 = this->m_fTexCoords[index].y2;

	return (ty2-ty1) * 255;
}



















