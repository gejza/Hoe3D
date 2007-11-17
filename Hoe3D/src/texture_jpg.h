
#ifndef _TEXTURE_CONVERTER_JPG_H_
#define _TEXTURE_CONVERTER_JPG_H_

#include "texture_converter.h"

#define PRESENT_JPG 1

class TextureConverterJPG : public TextureConverter
{
	dword m_width;
	dword m_height;
	HOEFORMAT m_format;
	dword row_stride;
	void * jpginfo;

	byte * udata;
public:
	TextureConverterJPG(TextureLoader *,HoeLog *);
	~TextureConverterJPG();
	virtual bool Run();
	virtual void Destroy();
	virtual bool Get(byte * p,dword pitch);

	virtual dword GetWidth() { return m_width;}
	virtual dword GetHeight() { return m_height;}
	HOEFORMAT GetFormat() { return m_format;};

};


#endif // _TEXTURE_CONVERTER_JPG_H_


