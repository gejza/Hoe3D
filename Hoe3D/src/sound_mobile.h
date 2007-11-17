
/**
   @file    sound_al.h
   @date    Sep 2004
   @version $Revision: 1.7 $
   @brief   Hlavni definice zvuku pres OpenAL.
*/
/*
	$Id: sound_al.h,v 1.7 2004/10/27 00:36:24 gejza Exp $
	$Author: gejza $
	$Date: 2004/10/27 00:36:24 $
	$Revision: 1.7 $
*/ 

#ifndef _HOE_SOUND_MOBILE_H_
#define _HOE_SOUND_MOBILE_H_

#define _HOE_SOUNDM_

struct THoeInitSettings;

/**
  @brief	Zvuk pres SoundMobile 
 */

class SoundSystemMobile
{
public:
	/** Konstruktor
	 */
	SoundSystemMobile(void);

	/**
	 * Zinicializuje zvuk.
	 * @param window Handle na hlavni okno
	 */
	bool Init(THoeInitSettings *);
	
	/** 
	 * Uvolni z pameti vsechny zarizeni
	 */
	void Destroy(void);
};

/**
  @brief	SoundMobile buffer 
 */

class SoundMobileBuffer
{
public:
	/** Kontruktor 
	 * @todo Dodelat destruktor 
	 */
	SoundMobileBuffer();

    /** Vytvoreni bufferu
    * @param channels Pocet kanalu
    * @param freq Samplu za sekundu
    * @param byts Pocet bitu
    * @param samples Pocet samplu
    */
    bool Create(int channels, int freq, int byts, long samples, bool ctrl3D);
    byte * Lock();
    void Unlock();

};

/**
  @brief	SoundMobile player 
 */

class SoundMobilePlayer
{
protected:
	
public:
	/** Kontruktor 
	 * @todo Dodelat destruktor 
	 */
	SoundMobilePlayer();

	/**
	 * Play
	 * @param buff Buffer kterej se ma prehrat
	 */
	void Play(SoundMobileBuffer * buff, bool loop);
	void Stop() {};
};

#endif // _HOE_SOUND_MOBILE_H_

