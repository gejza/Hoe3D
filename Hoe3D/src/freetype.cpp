
#include "system.h"
#include "shared.h"
#include "utils.h"
#include "config.h"
#include "texture_system.h"

#include "freetype.h"

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

bool HoeFreeTypeFont::Load(const char * filename)
{
	FT_Error e;
	if (e = FT_New_Face( freetype->GetLibrary(), filename, 0, &face )) 
	{
		Con_Print("FT_New_Face failed (a problem with font file) %s",freetype->GetError(e));
		return false;
	}

	return true;
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
