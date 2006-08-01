
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

#ifndef _HOE_SOUND_AL_H_
#define _HOE_SOUND_AL_H_

#define _HOE_OPENAL_

#include <AL/al.h>
#include <AL/alc.h>
//#include <AL/alut.h>

struct THoeInitSettings;

/**
  @brief	Zvuk pres OpenAL 
 */

class SoundSystemAl
{
	ALCdevice *Device;  
	ALCcontext *Context;
public:
	/** Konstruktor
	 */
	SoundSystemAl(void);

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
  @brief	OpenAL buffer 
 */

class HoeALBuffer
{
	ALuint	buffer; /**< AL buffer */
public:
	/** Kontruktor 
	 * @todo Dodelat destruktor 
	 */
	HoeALBuffer();

	/** Kontruktor 
	 * @param b AL buffer 
	 */
	HoeALBuffer(ALuint b);

	/**
	 * Provizorni nacteni ze souboru
	 * @todo predelat nacitani z file systemu 
	 */
	bool LoadFromFile(const char * filename);

	/**
	 * Funkce pro pristup k bufferu
	 * @return ALuint buffer
	 */
	ALuint & GetBuffer() { return buffer; }
};

/**
  @brief	OpenAL player 
 */

class HoeALPlayer
{
	ALuint	source; /**< AL source */
protected:
	
public:
	/** Kontruktor 
	 * @todo Dodelat destruktor 
	 */
	HoeALPlayer();

	/**
	 * Play
	 * @param buff Buffer kterej se ma prehrat
	 */
	void Play(HoeALBuffer * buff);
};

#endif // _HOE_SOUND_AL_H_

