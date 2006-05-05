
#ifndef _TEXTURE_CONVERTER_H_
#define _TEXTURE_CONVERTER_H_

#include "hoe_format.h"
#include <png.h>


/**
 * TextureConverter
 * konvertuje textury do jinych formatu
 */

class TextureConverter
{
protected:
	TextureLoader * m_loader;
	HoeLog * m_log;
	byte * udata;
public:
	TextureConverter(TextureLoader *,HoeLog *);
	~TextureConverter();
	/**
	* Funkce ma za ukol nacist hlavicku obrazku. Zjistit velikosti a vsechny ostatni udaje
	* aby se mohla vytvorit textura.
	* @return Pokud vse v poradku vraci true
	*/
	virtual bool Run() = 0;
	virtual void Destroy() = 0;

	virtual dword GetWidth() = 0;
	virtual dword GetHeight() = 0;
	virtual HOEFORMAT GetFormat() = 0;

	/**
	* Funkce pro ziskani dat.
	* @param p Kam zapsat vysledna data
	* @param pitch Pitch obrazku (tedy velikost jedne radky v bytech)
	* @return Pokud vse v poradku vraci true
	*/
	virtual bool Get(byte * p,dword pitch) = 0;
	virtual byte * Get(dword pitch);

	static void MixRGB(void * buff,dword size);
	static void Convert(byte * p1,dword s1,HOEFORMAT f1,byte * p2,dword s2,HOEFORMAT f2);
};

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
	HOEFORMAT GetFormat() { return m_format;};
};

#endif // _TEXTURE_CONVERTER_H_


