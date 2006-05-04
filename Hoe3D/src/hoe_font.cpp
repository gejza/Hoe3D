
#include "system.h"
#include "shared.h"
#include "utils.h"
#include "ref.h"
#include "config.h"
#include "states.h"
#include "hoe_format.h"
#include "hoe_texture.h"
#include "texture_system.h"
#include "hoe_font.h"
#include "freetype.h"
#include "hoe2d.h"

HoeFont::HoeFont(const char* strFontName, uint dwHeight, dword dwFlags)
{
    strcpy( m_strFontName, strFontName );
    m_dwFontHeight         = dwHeight;
    m_dwFontFlags          = dwFlags;
    m_dwSpacing            = 0;

    m_tex = NULL;
}

HoeFont::~HoeFont()
{

}

#define MIN_C (32)
#define MAX_C (127) 

static void PrintCharA8L8(uint texX, uint texY, HoeFreeTypeFont::FreeChar * data,HoeTexture::LOCKRECT *lr)
{
    uint x, y;

	// prepsat obraz znaku
	for( y=0; y < data->height; y++ )
	{
		word * pDst32 = (word*)(((byte*)lr->data + (lr->pitch * (texY + y)) + texX * sizeof(word)));
		for( x=0; x < data->width; x++ )
		{
			byte bAlpha = (byte)data->buffer[(y * data->pitch) + x];
			if (bAlpha > 0)
			{
				//*pDst32++ = (dword) (0xffffff00 | (bAlpha));
				*pDst32++ = (word) ((bAlpha << 8) | (bAlpha));
			}
			else
			{
				*pDst32++ = 0x00000000;
			}
		}
	}

}

static void PrintCharA8R8G8B8(uint texX, uint texY, HoeFreeTypeFont::FreeChar * data,HoeTexture::LOCKRECT *lr)
{
    uint x, y;

	// prepsat obraz znaku
	for( y=0; y < data->height; y++ )
	{
		dword * pDst32 = (dword*)(((byte*)lr->data + (lr->pitch * (texY + y)) + texX * sizeof(dword)));
		for( x=0; x < data->width; x++ )
		{
			byte bAlpha = (byte)data->buffer[(y * data->pitch) + x];
			if (bAlpha > 0)
			{
				//*pDst32++ = (dword) (0xffffff00 | (bAlpha));
				*pDst32++ = (dword) ((bAlpha << 24) | (bAlpha << 16) | (bAlpha << 8) | bAlpha);
			}
			else
			{
				*pDst32++ = 0x00000000;
			}
		}
	}

}

bool HoeFont::Init()
{
	HoeFreeTypeFont ffont(Get2D()->GetFreeType());
	if (!ffont.Load(this->m_strFontName))
		return false;
	ffont.SetHeight(this->m_dwFontHeight);

	bool lformat = true;

	this->m_tex = new HoeTexture;
	if (!this->m_tex->Create(256,256,HOE_A8L8))
	{
		lformat = false;
		Con_Print("A8L8 format failed, use A8R8G8B8");
		if (!this->m_tex->Create(256,256,HOE_A8R8G8B8))
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

	for( int c = MIN_C; c < MAX_C; c++ )
	{
		HoeFreeTypeFont::FreeChar data;
		if (!ffont.GetChar(c,&data))
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
			if (lformat)
				PrintCharA8L8(texX,texY,&data,&lr);
			else
				PrintCharA8R8G8B8(texX,texY,&data,&lr);

			// zapsat tex souradnice
			this->m_fTexCoords[c - MIN_C].x1 = texX / 256.f;
			this->m_fTexCoords[c - MIN_C].x2 = (texX + data.width) / 256.f;
			this->m_fTexCoords[c - MIN_C].y1 = texY / 256.f;
			this->m_fTexCoords[c - MIN_C].y2 = (texY + data.height) / 256.f;

			texX += data.width + 2;
			if (data.height > maxY)
				maxY = data.height;
		}

		this->m_fTexCoords[c - MIN_C].prex = (float)data.left;
		this->m_fTexCoords[c - MIN_C].postx = (float)data.movex;
		this->m_fTexCoords[c - MIN_C].top = (float)data.top;
	}

    m_tex->Unlock();

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
	float x, y, z, rhw; 
	dword color;
	float       tu, tv;   // The texture coordinates
};

#define FONT_VERTEX_FVF (D3DFVF_XYZRHW|D3DFVF_DIFFUSE|D3DFVF_TEX1)

struct FONTBACK_VERTEX
{
	float x, y, z, rhw; 
	dword color;
};

#define FONTBACK_VERTEX_FVF (D3DFVF_XYZRHW|D3DFVF_DIFFUSE)

#endif

bool HoeFont::DrawText( float sx, float sy, dword dwColor, 
                      const char* strText, dword dwFlags )
{
	// pain back
	/*if (dwFlags & HOEFONT_BACK)
	{
		GetHoeStates()->SetupFontBack();
		float tx,ty;
		this->GetTextSize(strText,tx,ty);
		tx += sx; ty += sy;
#ifdef _HOE_OPENGL_
		glColor4f(0,0,0,1);
		glBegin(GL_QUADS);// Zaèátek kreslení obdélníkù
			glVertex2f( sx-0.5f,sy-0.5f );// Levý horní bod
			glVertex2f( tx+0.5f,sy-0.5f);// Pravý horní bod
			glVertex2f( tx+0.5f,ty+0.5f );// Pravý dolní bod
			glVertex2f( sx-0.5f,ty+0.5f);// Levý dolní bod
			/*glVertex2f( sx,sy );// Levý horní bod
			glVertex2f( tx,sy);// Pravý horní bod
			glVertex2f( tx,ty );// Pravý dolní bod
			glVertex2f( sx,ty);// Levý dolní bod*/
		/*glEnd();// Konec kreslení obdélníkù
#endif // _HOE_OPENGL_
#ifdef _HOE_D3D_
		FONTBACK_VERTEX pvb[] = 
		{
			{ sx-0.5f,sy-0.5f , 0.5f, 1.0f, 0xff000000 }, 
			{ tx+0.5f,sy-0.5f , 0.5f, 1.0f, 0xff000000 }, 
			{ tx+0.5f,ty+0.5f , 0.5f, 1.0f, 0xff000000 },
			{ sx-0.5f,ty+0.5f , 0.5f, 1.0f, 0xff000000 }
		};
		D3DDevice()->SetFVF(FONTBACK_VERTEX_FVF);
		D3DDevice()->DrawPrimitiveUP(D3DPT_TRIANGLEFAN,2,pvb,sizeof(FONTBACK_VERTEX));
#endif
	}*/
	
	this->m_tex->Set();
	GetHoeStates()->SetupFont();

#ifdef _HOE_OPENGL_
	glColor4f(GET_RED_FLOAT(dwColor),GET_GREEN_FLOAT(dwColor),GET_BLUE_FLOAT(dwColor),GET_ALPHA_FLOAT(dwColor));
#endif // _HOE_OPENGL_

	while( *strText )
    {
        char c = *strText++;

        if( (c-32) < 0 || (c-32) >= 128-32 )
            continue;

		sx += m_fTexCoords[c-32].prex;

        float tx1 = m_fTexCoords[c-32].x1;
        float ty1 = m_fTexCoords[c-32].y1;
        float tx2 = m_fTexCoords[c-32].x2;
        float ty2 = m_fTexCoords[c-32].y2;

        float w = (tx2-tx1) *  256;
        float h = (ty2-ty1) *  256;

		float top = m_fTexCoords[c-32].top - 12;

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
			{ sx+0-0.5f,sy+0-0.5f-top , 0.5f, 1.0f, dwColor, tx1 , ty1}, 
			{ sx+w-0.5f,sy+0-0.5f-top , 0.5f, 1.0f, dwColor, tx2, ty1}, 
			{ sx+w-0.5f,sy+h-0.5f-top , 0.5f, 1.0f, dwColor, tx2 , ty2},
			{ sx+0-0.5f,sy+h-0.5f-top , 0.5f, 1.0f, dwColor, tx1, ty2}
		};
		D3DDevice()->SetFVF(FONT_VERTEX_FVF);
		D3DDevice()->DrawPrimitiveUP(D3DPT_TRIANGLEFAN,2,pv,sizeof(FONT_VERTEX));
#endif // _HOE_D3D9_
        }

        sx += w/*m_fTexCoords[c-32].postx*/ - (2 * m_dwSpacing);
    }

	return true;
}

void HoeFont::GetTextSize(const char *text,THoeFontSize * size)
{
	size->width = 0.f;
	//y = 0.f;

	if (*text == '\0')
		return;

	float ty1 = this->m_fTexCoords['X'-32].y1;
	float ty2 = this->m_fTexCoords['X'-32].y2;

	//y = (ty2-ty1) * 256;

	while (*text)
	{
		char c = *text++;

		size->width += m_fTexCoords[c-32].prex;

		float tx1 = this->m_fTexCoords[c-32].x1;
        float tx2 = this->m_fTexCoords[c-32].x2;

        float w = (tx2-tx1) *  256;
 
        size->width += w - (2 * m_dwSpacing);
	}
	size->width += 2 * m_dwSpacing;
}

float HoeFont::GetTextHeight()
{
	float ty1 = this->m_fTexCoords['X'-32].y1;
	float ty2 = this->m_fTexCoords['X'-32].y2;

	return (ty2-ty1) * 255;
}



















