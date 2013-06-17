
#ifndef _TEXTURE_CONVERTER_H_
#define _TEXTURE_CONVERTER_H_

#include <HoeCore/hoe_format.h>

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

#endif // _TEXTURE_CONVERTER_H_


