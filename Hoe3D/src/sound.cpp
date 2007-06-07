
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
#include <vorbis/codec.h>
#include <vorbis/vorbisfile.h> 

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

HoeSoundBuffer * SoundSystem::GetSound(const char * name, bool ctrl3d)
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

HoeSoundBuffer * SoundSystem::GetBuffer(const char * name)
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

static size_t read_func(void *ptr, size_t size, size_t nmemb, void *datasource)
{
	return reinterpret_cast<XHoeFile*>(datasource)->Read(ptr, size*nmemb) / size;
}

static int seek_func(void *datasource, ogg_int64_t offset, int whence)
{
	if (whence == SEEK_SET) 
		reinterpret_cast<XHoeFile*>(datasource)->Seek((size_t)offset);
	else if (whence == SEEK_CUR)
		reinterpret_cast<XHoeFile*>(datasource)->Skip((size_t)offset);
	//else if (whence == SEEK_END)
	//	reinterpret_cast
	return 0;
}

static int close_func(void *datasource)
{
	return 1;
}

static long tell_func(void *datasource)
{
	return (long)reinterpret_cast<XHoeFile*>(datasource)->Tell();
}

bool HoeSoundBuffer::LoadFromFile(const char *filename)
{
	OggVorbis_File vf; 
	ov_callbacks cbcs = {read_func, seek_func, close_func, tell_func};
	//XHoeFile * file = GetFS()->Open(filename);
	if (filename[strlen(filename)-1]!='g')
		return true;
	FILE *f = fopen(filename, "rb");
	if (!f)
		return false;
	if(ov_open(f, &vf, NULL, 0) < 0) 
	{
      Con_Print("Input does not appear to be an Ogg bitstream.\n");
      exit(1);
	} 
	
    char **ptr=ov_comment(&vf,-1)->user_comments;
    vorbis_info *vi=ov_info(&vf,-1);
    while(*ptr){
      Con_Print("%s",*ptr);
      ++ptr;
    }
	long samples = (long)ov_pcm_total(&vf,-1);
    Con_Print("Bitstream is %d channel, %ldHz",vi->channels,vi->rate);
    Con_Print("Decoded length: %ld samples",
	    samples);
    Con_Print("Encoded by: %s",ov_comment(&vf,-1)->vendor);
	Create(vi->channels,vi->rate, 2, samples, m_ctrl3D);


	byte * l = Lock();

	int cs;
	size_t bys = 2*samples*vi->channels;
	while (bys>0)
	{
		long r = ov_read(&vf,(char*)l,bys,0,2,1,&cs); 
		assert(r > 0);
		bys-=r;
		l+=r;
	}

	Unlock();

	ov_clear(&vf); 

	return true;
}
#else
#error aaa
#endif
