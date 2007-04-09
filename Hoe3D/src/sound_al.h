
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
	ALuint	m_buffer; /**< AL buffer */
	ALsizei m_size,m_freq; /**< velikost a frekvence */
	ALenum	m_format; /**< format */
	ALvoid	*m_data; /**< data pro inicializaci bufferu */

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
	 * Funkce pro pristup k bufferu
	 * @return ALuint buffer
	 */
	ALuint & GetBuffer() { return m_buffer; }

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
	void Play(HoeALBuffer * buff, bool loop);
	void Stop() {};
};

#endif // _HOE_SOUND_AL_H_

