
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
	IDirectSoundBuffer8 * m_buffer;
	IDirectSound3DBuffer8 * m_3d; /**< DS buffer */
 	LPVOID lpvWrite;
	DWORD  dwLength;
public:
	/** Kontruktor 
	 */
	HoeDSBuffer();
	/** Destruktor 
	 */
	~HoeDSBuffer();
	/** Vytvoreni bufferu
	* @param channels Pocet kanalu
	* @param freq Samplu za sekundu
	* @param byts Pocet bitu
	* @param samples Pocet samplu
	*/
	bool Create(int channels, int freq, int byts, long samples, bool ctrl3D);
	byte * Lock();
	void Unlock();
	IDirectSoundBuffer8 * GetHandle() { return m_buffer; }
	IDirectSound3DBuffer8 * GetHandle3D() { return m_3d; }
};

/**
  @brief	DirectSound player 
 */

class HoeDSPlayer
{
protected:
	HoeDSBuffer * m_buff;
public:
	/** Kontruktor 
	 * @todo Dodelat destruktor 
	 */
	HoeDSPlayer();

	/**
	 * Play
	 * @param buff Buffer kterej se ma prehrat
	 * @param loop Prehravat dokola
	 */
	void Play(HoeDSBuffer * buff, bool loop);
	void Stop();
};

/**
  @brief	Zvuk pres DirectSound
 */

class SoundSystemDS
{
	LPDIRECTSOUND8 m_pDS; /**< DirectSound objekt */
	IDirectSound3DListener8 * m_listener;
public:
	/** Konstruktor */
	SoundSystemDS();

	/**
	 * Zinicializuje zvuk.
	 * @param hWnd Handle na hlavni okno
	 */
	bool Init(THoeInitSettings *);

	LPDIRECTSOUND8 GetHandle() { return m_pDS; }
	IDirectSound3DListener8 * GetListener() { return m_listener; }
	/** 
	 * Uvolni z pameti vsechny zarizeni
	 */
	void Destroy();
};

#endif // _HOE_SOUND_DS_H_
