
/**
   @file    sound.h
   @date    Sep 2004
   @version $Revision: 1.10 $
   @brief   Hlavni soubor se zvukem.
*/

#ifndef _HOE_SOUND_OGG_
#define _HOE_SOUND_OGG_

#define HOE_OGG_PRESENT 1

#include <vorbis/codec.h>
#include <vorbis/vorbisfile.h> 
/**
  @brief	Nacitani souboru z ogg. 
 */

class SoundSourceOgg : public SoundSource
{
protected:
	FILE *f;
	OggVorbis_File vf; 
public:
	/** Konstruktor */
	SoundSourceOgg();

	/** Destruktor */
	~SoundSourceOgg();

	// provizorni
	bool Open(const tchar* name);

	/**
	 * Nahrani casti zvuku do bufferu
	 * @param buff Buffer
	 * @param size Velikost pro nahravani
	 * @return Pocet nactenych bytu
	 */
	virtual size_t Read(byte* buff, size_t size);

	virtual bool GetInfo(TSoundSourceInfo* info);

};

#endif // _HOE_SOUND_


