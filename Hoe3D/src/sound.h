
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

/**
  @brief	Uloziste zvuku 
 */

class HoeSoundBuffer
#ifdef _HOE_OPENAL_
	: public HoeALBuffer
#endif

#ifdef _HOE_DS8_
	: public HoeDSBuffer
#endif 
{
public:
	/**
	 * Provizorni nacteni ze souboru
	 * @todo predelat nacitani z file systemu 
	 */
	bool LoadFromFile(const char * filename);

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
class HoeSound : public IHoeSound
{
	HoeSoundPlayer player; /**< player */
	HoeSoundBuffer buffer; /**< @todo znicit */
public:
	/**
	 * Prehraje aktualni buffer
	 */
	void Play();

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
	 */
	HoeSound * GetSound(const char * name);
};

#endif // _HOE_SOUND_


