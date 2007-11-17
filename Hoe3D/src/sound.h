
/**
   @file    sound.h
   @date    Sep 2004
   @version $Revision: 1.10 $
   @brief   Hlavni soubor se zvukem.
*/

#ifndef _HOE_SOUND_
#define _HOE_SOUND_

#include "../include/hoeinterfaces.h"

#ifdef _HOE_OPENAL_
	: public SoundSystemAl
#endif

#ifdef _HOE_DS8_
	: public SoundSystemDS
#endif 

#ifdef _HOE_SOUNDM_
	: public SoundSystemMobile
#endif 

#if defined (_USE_D3D9_) || defined (_USE_D3D8_) 
#include "sound_ds.h"
typedef SoundSystemDS SoundSystemDevice;
typedef HoeDSPlayer SoundPlayerBase;
typedef HoeDSBuffer SoundBufferBase;
#endif

#ifdef _USE_OPENGL_
#include "sound_al.h"
typedef SoundSystemAl SoundSystemDevice;
typedef HoeALPlayer SoundPlayerBase;
typedef HoeALBuffer SoundBufferBase;
#endif 

#ifdef _USE_D3DM_
#include "sound_mobile.h"
typedef SoundSystemMobile SoundSystemDevice;
typedef SoundMobilePlayer SoundPlayerBase;
typedef SoundMobileBuffer SoundBufferBase;
#endif

class XHoeFile;

struct TSoundSourceInfo
{
	int channels;
	int rate;
	int samples;
};

/**
  @brief	Zdroj zvuku
 */
class SoundSource
{
public:
	virtual size_t Read(byte* buff, size_t size) = 0;
	virtual bool GetInfo(TSoundSourceInfo* info) = 0;
};

/**
  @brief	Uloziste zvuku 
 */

class HoeSoundBuffer : public IHoeSound, public SoundBufferBase
{
	bool m_ctrl3D;
public:
	HoeSoundBuffer(bool ctrl3D);
	/**
	 * Provizorni nacteni ze souboru
	 * @todo predelat nacitani z file systemu 
	 */
	bool LoadFromFile(const tchar * filename);
	bool Load(SoundSource& source);
	virtual void HOEAPI Play(bool loop) {};
	virtual void HOEAPI Stop() {};
	virtual void HOEAPI Delete() { delete this; }
};

/**
  @brief	Prehravac zvuku 
 */

class HoeSoundPlayer : public SoundPlayerBase
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

class SoundSystem : public SoundSystemDevice
{
protected:
	/**
	 * Nahrani bufferu ze souboru
	 * @param name Jmeno zvuku, ktery se hleda ve File systemu
	 */
	HoeSoundBuffer * GetBuffer(const tchar * name);
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
	HoeSoundBuffer * GetSound(const tchar * name, bool ctrl3d);

};

#endif // _HOE_SOUND_


