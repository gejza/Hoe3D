
/**
   @file    freetype.h
   @date    Sep 2004
   @version $Revision: 1.5 $
   @brief   Vytvareni fontu pres freetype
*/
/*
	$Id: freetype.h,v 1.5 2005/04/01 01:19:51 gejza Exp $
	$Author: gejza $
	$Date: 2005/04/01 01:19:51 $
	$Revision: 1.5 $
*/ 

#ifndef _HOE_FREE_TYPE_H_
#define _HOE_FREE_TYPE_H_

#include <ft2build.h>
#include FT_FREETYPE_H
#include FT_GLYPH_H
#include FT_OUTLINE_H
#include FT_TRIGONOMETRY_H

class HoeFreeType
{
	FT_Library library;
public:
	HoeFreeType();
	bool Init();
	void Close();

	inline FT_Library GetLibrary() { return library; }
	static const char * GetError(FT_Error e);
};

class HoeFreeTypeFont
{
public:
	struct FreeChar
	{
		uint width; // sirka znaku
		uint pitch; // posun
		uint height; // vyska znaku
		int left; // posun znaku
		int top; // posun znaku dolu
		int movex; // odsun znaku
		unsigned char* buffer; // data
	};
private:
	HoeFreeType * freetype;

	FT_Face face;
	FT_Glyph glyph;// Glyph objekt
	FT_Bitmap * bitmap;
public:
	HoeFreeTypeFont(HoeFreeType * ft);
	~HoeFreeTypeFont();
	bool Load(const char * filename);
	void SetHeight(long height);
	bool GetChar(wchar_t c, FreeChar * out);
};

#endif // _HOE_FREE_TYPE_H_

