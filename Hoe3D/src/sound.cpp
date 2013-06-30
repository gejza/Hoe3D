
/**
   @file    sound.cpp
   @date    Sep 2004
   @version $Revision: 1.6 $
   @brief   Definice zvuku pro Hoe.
*/
/*
	$Id: sound.cpp,v 1.6 2005/02/03 14:56:04 gejza Exp $
	$Author: gejza $
	$Date: 2005/02/03 14:56:04 $
	$Revision: 1.6 $
*/ 

#include "StdAfx.h"
#include "shared.h"
#include "utils.h"
#include "sound.h"
#include "../include/hoefs.h"
#include "sound_ogg.h"

#if defined (_HOE_DS8_) || defined (_HOE_OPENAL_) || defined (_HOE_SOUNDM_) 
SoundSystem::SoundSystem()
{
	assert(shared::sound == NULL);
	shared::sound = this;
}

SoundSystem::~SoundSystem()
{
	assert(shared::sound != NULL);
	shared::sound = NULL;
}

HoeSoundBuffer * SoundSystem::GetSound(const tchar * name, bool ctrl3d)
{
	if (ctrl3d)
	{
		HoeSoundBuffer * sound = new HoeSoundBuffer(true);
		sound->LoadFromFile(name);
		return sound;
	}
	else
	{
		HoeSound * sound = new HoeSound;
		sound->LoadFromFile(name);
		return sound;
	}
}

HoeSoundBuffer * SoundSystem::GetBuffer(const tchar * name)
{
	HoeSoundBuffer * sb = new HoeSoundBuffer(true);
	sb->LoadFromFile(name);

	return sb;
}

////////////////////////////////////////
HoeSoundBuffer::HoeSoundBuffer(bool ctrl3D) 
{
	m_ctrl3D = ctrl3D;
}

////////////////////////////////////////

void HoeSound::Play(bool loop)
{
	player.Play(this, loop);
}

void HoeSound::Stop()
{
	player.Stop();
}

bool HoeSoundBuffer::LoadFromFile(const tchar *filename)
{
#ifdef HOE_OGG_PRESENT
	SoundSourceOgg ogg;
	return ogg.Open(filename) && Load(ogg);
#else
	return false;
#endif
}

bool HoeSoundBuffer::Load(SoundSource& source)
{
	TSoundSourceInfo info;

	if (!source.GetInfo(&info))
		return false;
	// read
	byte * l = Lock();
	if (!l)
		return false;

	// read to lock < aa
	int cs;
	size_t bys = 2*info.samples*info.channels;
	while (bys>0)
	{
		long r = source.Read(l,bys); 
		assert(r > 0);
		bys-=r;
		l+=r;
	}

	Unlock();

	return true;
}
#else
#error Check for sound errors
#endif
