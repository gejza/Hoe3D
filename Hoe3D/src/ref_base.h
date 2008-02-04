
/**
   @file    ref_base.h
   @date    Mar 2006
   @version 1.0
   @brief   Deklarace tridy RefBase.
*/

#ifndef _REF_BASE_H_
#define _REF_BASE_H_

#include "../include/hoeinterfaces.h"
#include <HoeCore/hoe_math.h>

class RefSurfaceBase
{
public:
	struct LockRect
	{
		byte * ptr;
		dword pitch;
	};
};

/** 
* @brief Zakladni trida ref.
*/
class RefBase : public IHoeRef
{
protected:
	bool m_Fullscreen; ///< Fullscreen rezim
	uint m_Width;      ///< Sirka vykreslovaciho okna
	uint m_Height;     ///< Vyska vykreslovaciho okna
	dword m_BackColor; ///< Aktualni barva pozadi
public:
	/** 
	* Kontruktor 
	*/
	RefBase();
	/** 
	* Destruktor 
	*/
	virtual ~RefBase() {}
	/** 
	* Nastaveni barvy na pozadi 
	* @param color Barva ve formatu 0xFFRRGGBB
	*/
	virtual void HOEAPI SetBackgroundColor(unsigned long color);
	/** 
	* Zjisteni zapnuteho rezimu.
	* @retval true Cela obrazovka
	* @retval false V okne
	*/
	virtual bool HOEAPI IsFullscreen() const { return m_Fullscreen; }
	/** 
	* Sirka obrazu 
	* @return Sirka obrazu v pixelech.
	*/
	uint GetWidth() const { return m_Width; }
	/** 
	* Vyska obrazu 
	* @return Vyska obrazu v pixelech.
	*/
	uint GetHeight() const { return m_Height; }
};

#endif // _REF_BASE_H_

