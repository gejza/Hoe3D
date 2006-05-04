
/**
   @file    sound_ds.h
   @date    Sep 2004
   @version $Revision: 1.5 $
   @brief   Hlavni definice zvuku pres DirectSound8.
*/
/*
	$Id: sound_ds.h,v 1.5 2004/10/27 00:36:24 gejza Exp $
	$Author: gejza $
	$Date: 2004/10/27 00:36:24 $
	$Revision: 1.5 $
*/ 

#ifndef _HOE_SOUND_DS_H_
#define _HOE_SOUND_DS_H_

#define _HOE_DS8_

#include <dsound.h>

struct THoeInitSettings;

/**
  @brief	DirectSound buffer 
 */

class HoeDSBuffer
{
	//ALuint	buffer; /**< DS buffer */
public:
	/** Kontruktor 
	 * @todo Dodelat destruktor 
	 */
	HoeDSBuffer();

	/**
	 * Provizorni nacteni ze souboru
	 * @todo predelat nacitani z file systemu 
	 */
	bool LoadFromFile(const char * filename);
};

/**
  @brief	DirectSound player 
 */

class HoeDSPlayer
{
protected:

public:
	/** Kontruktor 
	 * @todo Dodelat destruktor 
	 */
	HoeDSPlayer();

	/**
	 * Play
	 * @param buff Buffer kterej se ma prehrat
	 */
	void Play(HoeDSBuffer * buff);
};

/**
  @brief	Zvuk pres DirectSound
 */

class SoundSystemDS
{
	LPDIRECTSOUND8 m_pDS; /**< DirectSound objekt */
public:
	/** Konstruktor */
	SoundSystemDS();

	/**
	 * Zinicializuje zvuk.
	 * @param hWnd Handle na hlavni okno
	 */
	bool Init(THoeInitSettings *);

	/** 
	 * Uvolni z pameti vsechny zarizeni
	 */
	void Destroy();
};

#endif // _HOE_SOUND_DS_H_
