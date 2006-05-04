
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

#include "system.h"
#include "shared.h"
#include "utils.h"
#include "sound.h"

#if defined (_HOE_DS8_) || defined (_HOE_OPENAL_)

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

HoeSound * SoundSystem::GetSound(const char * name)
{
	HoeSound * sound = new HoeSound;
	sound->buffer.LoadFromFile(name);

	return sound;
}

HoeSoundBuffer * SoundSystem::GetBuffer(const char * name)
{
	HoeSoundBuffer * sb = new HoeSoundBuffer;
	sb->LoadFromFile(name);

	return sb;
}

////////////////////////////////////////

void HoeSound::Play()
{
	player.Play(&buffer);
}

#endif
