
#include "StdAfx.h"
#include "shared.h"
#include "utils.h"
#include "config.h"
#include "texture_system.h"

#include "freetype.h"

static void PrintCharA8L8(uint texX, uint texY, HoeFreeTypeFont::FreeChar * data,float sc_alpha, HoeTexture::LOCKRECT *lr)
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
				const float alfaf = ((float)bAlpha) * sc_alpha;
				byte a;
				//Con_Print("%d %f", alfaf, (int)bAlpha);
				//if (alfaf > 1.f) 
					a = 0xff;
				//else a = (byte)alfaf;
				//Con_Print("bt");
				*pDst32++ = (word) ((a << 8) | (bAlpha));
			}
			else
			{
				*pDst32++ = 0x00000000;
			}
		}
	}

}

static void PrintCharA8R8G8B8(uint texX, uint texY, HoeFreeTypeFont::FreeChar * data,float sc_alpha, HoeTexture::LOCKRECT *lr)
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
				float af = bAlpha * sc_alpha;
				byte a = af > 1.f ? 0xff:(byte)af;
				*pDst32++ = (dword) ((a << 24) | (bAlpha << 16) | (bAlpha << 8) | bAlpha);
			}
			else
			{
				*pDst32++ = 0x00000000;
			}
		}
	}

}

///////////////////////////////////////////

HoeFreeType::HoeFreeType()
{
	library = NULL;
}

bool HoeFreeType::Init(void)
{
	//Create and initilize a freetype font library.
	
	if (FT_Init_FreeType( &library ))
	{
		Con_Print("Failed initialize FreeType system.");
		return false;
	}

	FT_Int maj,min,patch;
	FT_Library_Version( library, &maj, &min, &patch);
	Con_Print("FreeType library v%d.%d.%d",maj,min,patch);

	return true;
}

#undef FT_ERRORDEF_                      
#define FT_ERRORDEF_( e, v, s )  case FT_Err_ ## e : return s; 
#define FT_NOERRORDEF_ FT_ERRORDEF_

const char * HoeFreeType::GetError(FT_Error e)
{
	switch (e)
	{
	#include "freetype/fterrdef.h"

	default:
		return "Unknown freetype error.";
	}
}

void HoeFreeType::Close()
{
	//Ditto for the library.
	FT_Done_FreeType(library);
}

HoeFreeTypeFont::HoeFreeTypeFont(HoeFreeType *ft)
{
	freetype = ft;
	face = NULL;
}

HoeFreeTypeFont::~HoeFreeTypeFont()
{
	if (face)
		FT_Done_Face(face);
}

bool HoeFreeTypeFont::Load(const tchar * filename)
{
#ifndef _UNICODE
	FT_Error e;
	if (e = FT_New_Face( freetype->GetLibrary(), filename, 0, &face )) 
	{
		Con_Print("FT_New_Face failed (a problem with font file) %s",freetype->GetError(e));
		return false;
	}

	return true;
#else
	assert(0);
	return false;
#endif
}

void HoeFreeTypeFont::SetHeight(long height)
{
	FT_Set_Char_Size(face, height << 6, height << 6, 96, 96);
}


bool HoeFreeTypeFont::GetChar(wchar_t c, FreeChar * out)
{
	FT_Error e;
	if(e = FT_Load_Glyph(face, FT_Get_Char_Index(face, c), FT_LOAD_DEFAULT))
	{
		Con_Print("FT_Load_Glyph failed (%s)",freetype->GetError(e));
		return false;
	}

	if(e = FT_Get_Glyph(face->glyph, &glyph))// Pøesun glyphu do glyph objektu
	{
		Con_Print("FT_Get_Glyph failed (%s)",freetype->GetError(e));
		return false;
	}

	FT_Glyph_To_Bitmap(&glyph, ft_render_mode_normal, 0, 1);// Konvertování glyphu na bitmapu
	FT_BitmapGlyph bitmap_glyph = (FT_BitmapGlyph)glyph;
	bitmap = &bitmap_glyph->bitmap;// Reference ulehèí pøístup k bitmapì

	out->width = bitmap->width;
	out->pitch = bitmap->pitch;
	out->height = bitmap->rows;
	if (c == ' ')
		out->left = face->glyph->advance.x >> 6;
	else
        out->left = bitmap_glyph->left;
    out->movex = face->glyph->advance.x >> 6;
	out->top = /*-(*/bitmap_glyph->top /*- bitmap->rows)*/;
	out->buffer = bitmap->buffer;

	return true;
}

bool HoeFont::Init()
{
	HoeFontRenderer * fr = GetFontRenderer(this->m_strFontName, this->m_dwFontHeight);
	HoeFreeTypeFont ffont(Get2D()->GetFreeType());
	if (!fr)
		return false;

	ffont.SetHeight();

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

		HoeFreeTypeFont::FreeChar data;
		if (!ffont.GetChar(GetCodePage()->GetChar(c),&data))
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
				PrintCharA8L8(texX,texY,&data,m_scalpha,&lr);
			else
				PrintCharA8R8G8B8(texX,texY,&data,m_scalpha,&lr);

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

	Con_Print("Create font: %s %d",this->m_strFontName,this->m_dwFontHeight);
    return true;
}

