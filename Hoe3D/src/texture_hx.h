
#ifndef _TEXTURE_CONVERTER_HX_H_
#define _TEXTURE_CONVERTER_HX_H_

#include "texture_converter.h"

class TextureConverterHX : public TextureConverter
{
	dword m_width;
	dword m_height;
	HOEFORMAT m_format;
public:
	TextureConverterHX(TextureLoader *,HoeLog *);
	~TextureConverterHX();

	virtual bool Run();
	virtual void Destroy();
	virtual bool Get(byte * p,dword pitch);

	virtual dword GetWidth() { return m_width;}
	virtual dword GetHeight() { return m_height;}
	HOEFORMAT GetFormat() { return m_format;};
};

#endif // _TEXTURE_CONVERTER_HX_H_


