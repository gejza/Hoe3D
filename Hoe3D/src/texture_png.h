
#ifndef _TEXTURE_CONVERTER_PNG_H_
#define _TEXTURE_CONVERTER_PNG_H_

#include "texture_converter.h"
#include <png.h>
#define PRESENT_PNG 1

class TextureConverterPNG : public TextureConverter
{
	dword m_width;
	dword m_height;
	HOEFORMAT m_format;
	png_structp png_ptr;
	png_infop info_ptr;
public:
	TextureConverterPNG(TextureLoader *,HoeLog *);
	~TextureConverterPNG();

	virtual bool Run();
	virtual void Destroy();
	virtual bool Get(byte * p,dword pitch);

	virtual dword GetWidth() { return m_width;}
	virtual dword GetHeight() { return m_height;}
	HOEFORMAT GetFormat() { return m_format;}
	HOEFORMAT GetSourceFormat();
};

#endif // _TEXTURE_CONVERTER_PNG_H_


