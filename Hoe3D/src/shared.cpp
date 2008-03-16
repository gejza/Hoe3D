
#include "StdAfx.h"
#include "shared.h"

#ifndef NULL
#define NULL 0
#endif 

namespace shared {
	HoeEngine * hoe = NULL;
	Config * config = NULL;
	Ref * ref = NULL;
	HoeRes::XResourceMgr * resmgr = NULL;
	TextureSystem * ts = NULL;
	HoeStates * states = NULL;
	MaterialSystem * material = NULL;
	LightSystem * light = NULL;
	Hoe2D * hoe2d = NULL; 
	Physics * physics = NULL;
	SoundSystem * sound = NULL;
	InfoSystem * info = NULL;
	HoeInput * input = NULL;
	CmdExec * exec = NULL;
	MainCodePage * codepage = NULL;
}


