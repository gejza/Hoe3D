
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

#ifdef _WIN32_WINNT
typedef IDirectSound8 DirectSound;
typedef IDirectSoundBuffer8 DirectSoundBuffer;
typedef IDirectSound3DBuffer8 DirectSound3DBuffer;
typedef IDirectSound3DListener8 DirectSound3DListener;
#else
typedef IDirectSound DirectSound;
typedef IDirectSoundBuffer DirectSoundBuffer;
typedef IDirectSound3DBuffer DirectSound3DBuffer;
typedef IDirectSound3DListener DirectSound3DListener;
#endif

struct THoeInitSettings;

/**
  @brief	DirectSound buffer 
 */

class HoeDSBuffer
{
	DirectSoundBuffer * m_buffer;
	DirectSound3DBuffer * m_3d; /**< DS buffer */
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
	DirectSoundBuffer * GetHandle() { return m_buffer; }
	DirectSound3DBuffer * GetHandle3D() { return m_3d; }
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
	DirectSound* m_pDS; /**< DirectSound objekt */
	DirectSound3DListener * m_listener;
public:
	/** Konstruktor */
	SoundSystemDS();

	/**
	 * Zinicializuje zvuk.
	 * @param hWnd Handle na hlavni okno
	 */
	bool Init(THoeInitSettings *);

	DirectSound* GetHandle() { return m_pDS; }
	DirectSound3DListener* GetListener() { return m_listener; }
	/** 
	 * Uvolni z pameti vsechny zarizeni
	 */
	void Destroy();
};

#endif // _HOE_SOUND_DS_H_
