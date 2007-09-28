
/**
   @file    sound.h
   @date    Sep 2004
   @version $Revision: 1.10 $
   @brief   Hlavni soubor se zvukem.
*/

#ifndef _HOE_SOUND_
#define _HOE_SOUND_

#include "../include/hoeinterfaces.h"

#if defined (_USE_D3D9_) || defined (_USE_D3D8_) 
#include "sound_ds.h"
#endif

#ifdef _USE_OPENGL_
#include "sound_al.h"
#endif 

#ifdef _USE_D3DM_
#include "sound_mobile.h"
#endif

/**
  @brief	Uloziste zvuku 
 */

class HoeSoundBuffer : public IHoeSound 
#ifdef _HOE_OPENAL_
	, public HoeALBuffer
#endif

#ifdef _HOE_DS8_
	, public HoeDSBuffer
#endif 
{
	bool m_ctrl3D;
public:
	HoeSoundBuffer(bool ctrl3D);
	/**
	 * Provizorni nacteni ze souboru
	 * @todo predelat nacitani z file systemu 
	 */
	bool LoadFromFile(const char * filename);
	virtual void HOEAPI Play(bool loop) {};
	virtual void HOEAPI Stop() {};
	virtual void HOEAPI Delete() { delete this; }
};

/**
  @brief	Prehravac zvuku 
 */

class HoeSoundPlayer
#ifdef _HOE_OPENAL_
	: public HoeALPlayer
#endif

#ifdef _HOE_DS8_
	: public HoeDSPlayer
#endif 
{
};


/**
  @brief	Zvuk, obsahuje jeden Player a nekolik Bufferu 
 */
class HoeSound : public HoeSoundBuffer
{
	HoeSoundPlayer player; /**< player */
public:
	HoeSound() : HoeSoundBuffer(false) {}
	/**
	 * Prehraje aktualni buffer
	 */
	virtual void HOEAPI Play(bool loop);
	virtual void HOEAPI Stop();
	virtual void HOEAPI Delete() { delete this; }
	friend class SoundSystem;

};

/**
  @brief	Hlavni objekt pro vytvareni zvuku. 
 */

class SoundSystem
#ifdef _HOE_OPENAL_
	: public SoundSystemAl
#endif

#ifdef _HOE_DS8_
	: public SoundSystemDS
#endif 
{
protected:
	/**
	 * Nahrani bufferu ze souboru
	 * @param name Jmeno zvuku, ktery se hleda ve File systemu
	 */
	HoeSoundBuffer * GetBuffer(const char * name);
public:
	/** Konstruktor */
	SoundSystem();

	/** Destruktor */
	~SoundSystem();

	/**
	 * Vytvoreni zvuku
	 * @param name Jmeno zvuku, ktery se hleda ve File systemu
	 * @param ctrl3d Zvuk se prehrava ve 3d
	 */
	HoeSoundBuffer * GetSound(const char * name, bool ctrl3d);

};

#endif // _HOE_SOUND_


