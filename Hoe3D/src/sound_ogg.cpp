
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
#include "sound_ogg.h"
#include "../include/hoefs.h"

SoundSourceOgg::SoundSourceOgg()
{
}

SoundSourceOgg::~SoundSourceOgg()
{
	ov_clear(&vf); 
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

bool SoundSourceOgg::Open(const tchar *name)
{
	ov_callbacks cbcs = {read_func, seek_func, close_func, tell_func};
	//XHoeFile * file = GetFS()->Open(filename);
	//if (filename[strlen(filename)-1]!='g')
	//	return true;
	// TODO vice formatu pro zvuk
	FILE *f = fopen(name, T("rb"));
	//FILE *f = _wfopen(name, T("rb"));
	if (!f)
		return false;
	if(ov_open(f, &vf, NULL, 0) < 0) 
	{
      Con_Print("Input does not appear to be an Ogg bitstream.\n");
      return false;
	} 

    char **ptr=ov_comment(&vf,-1)->user_comments;
    while(*ptr){
      Con_Print("%s",*ptr);
      ++ptr;
    }
	vorbis_info *vi=ov_info(&vf,-1);
	long samples = (long)ov_pcm_total(&vf,-1);
    Con_Print("Bitstream is %d channel, %ldHz",vi->channels,vi->rate);
    Con_Print("Decoded length: %ld samples", samples);
    Con_Print("Encoded by: %s",ov_comment(&vf,-1)->vendor);
	
	return true;
}

size_t SoundSourceOgg::Read(byte* buff, size_t size)
{
	int cs;
	return ov_read(&vf,(char*)buff,size,0,2,1,&cs); 
}

bool SoundSourceOgg::GetInfo(TSoundSourceInfo* info)
{
	vorbis_info *vi=ov_info(&vf,-1);
	info->channels = vi->channels;
	info->samples = (long)ov_pcm_total(&vf,-1);
	info->rate = vi->rate;
	return true;
}



