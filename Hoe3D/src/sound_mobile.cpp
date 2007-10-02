
#include "StdAfx.h"
#include "shared.h"
#include "utils.h"
#include "sound_mobile.h"

SoundSystemMobile::SoundSystemMobile()
{
}

bool SoundSystemMobile::Init(THoeInitSettings *)
{
	return true;
}


void SoundSystemMobile::Destroy()
{
}

///////////////////////////////////////////////////////////////////

SoundMobileBuffer::SoundMobileBuffer()
{
}

bool SoundMobileBuffer::Create(int channels, int freq, int byts, long samples, bool ctrl3D)
{
	return true;
}

byte * SoundMobileBuffer::Lock()
{
    return (byte*)NULL;
}

void SoundMobileBuffer::Unlock()
{
}

/////////////////////////////////////////////////////////////

SoundMobilePlayer::SoundMobilePlayer()
{
}

void SoundMobilePlayer::Play(SoundMobileBuffer * buff, bool loop)
{

}

