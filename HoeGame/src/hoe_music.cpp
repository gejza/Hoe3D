
#include "StdAfx.h"
#include "../include/HoeGame/hoe_console.h"
#ifndef HOE_NO_BASS
#include "../include/HoeGame/bassmod.h"
#endif
#include "../include/HoeGame/hoe_music.h"

namespace HoeGame {

////////////////////////////////////////////////////
//	HoeMusic_Module
//
////////////////////////////////////////////////////
HoeMusic_Module::HoeMusic_Module(){

	offsets=NULL;
	offsetsLength=0;
	actualOffsetIndex=-1;
	console = NULL;
}

HoeMusic_Module::HoeMusic_Module(OffsetLoop *Offsets){
	offsets=NULL;
	offsetsLength=0;
	actualOffsetIndex=-1;
	console = NULL;
	setOffsets(Offsets);
}


HoeMusic_Module::~HoeMusic_Module(){
	
	Destroy();
}

	//////////////////init//////////////////////////////
bool HoeMusic_Module::Init(BaseConsole * con){
#ifndef HOE_NO_BASS
	
	char * desc = BASSMOD_GetDeviceDescription(-1);
	console = con;
	if (BASSMOD_ErrorGetCode() != BASS_OK)
		return false;
	if (con) con->Printf(T("BASS: Init %s"),desc);
	return BASSMOD_Init(-1,44100,0) != FALSE;
#else
	return true;
#endif
}

void HoeMusic_Module::Destroy(){
	
#ifndef HOE_NO_BASS
	BASSMOD_MusicStop();
	BASSMOD_MusicFree();
#endif
}


	//////////////////load//////////////////////////////
bool HoeMusic_Module::Load(void *file){
					
#ifndef HOE_NO_BASS
	if (!BASSMOD_MusicLoad(false,file,0,0,BASS_MUSIC_RAMP|BASS_MUSIC_FT2MOD|BASS_MUSIC_CALCLEN))
	{
		if (console) console->Printf(T("BASS: Failed load music from file %s, error=%d"),(char*)file,BASSMOD_ErrorGetCode());
		return false;
	}
#endif
	if (console) console->Printf(T("BASS: Load music from file: %s"), (char*)file);
	return true;
}

	//////////////////play/pause/stop///////////////////
void HoeMusic_Module::Play(){
#ifndef HOE_NO_BASS
	BASSMOD_MusicPlay();
#endif
}
void HoeMusic_Module::Pause(){
#ifndef HOE_NO_BASS
	BASSMOD_MusicPause();
#endif
}
void HoeMusic_Module::Stop(){
#ifndef HOE_NO_BASS
	BASSMOD_MusicStop();
#endif
}

//int HoeMusic_Module::getLength(){
//	return BASSMOD_MusicGetLength(false);
//}

	////////////////////////////////////////////////////
	//	factor udava "pokrok"  <0,1>
	//	vraci stanku odpovidajici faktoru
int HoeMusic_Module::getOffsetPosByFactor(float factor){
	
	if(factor<0 || factor>1) return -1;

	return (int)(((offsetsLength-1)*factor));
	
}
	////////////////////////////////////////////////////
//dword HoeMusic_Module::getPreviousPos(dword pos){
//}

	//////////////////set/position//////////////////////
void HoeMusic_Module::setPos(DWORD pos){
#ifndef HOE_NO_BASS
	BASSMOD_MusicSetPosition(pos);
	//console->Printf ("sync %d",(DWORD)pos);
#endif

}
	//////////////////synchro/proc//////////////////////
void CALLBACK HoeMusic_Module::PosSyncProc(HSYNC handle, DWORD data, DWORD params){

	HoeMusic_Module *m=(HoeMusic_Module *)params;
    m->setPos(m->offsets[m->actualOffsetIndex].start);
}
	//////////////////set/pos/index/////////////////////
void HoeMusic_Module::setOffsets(OffsetLoop *Offsets){
	offsets=Offsets;

	for (offsetsLength=0;offsets[offsetsLength].start != 0xffffffff;offsetsLength++);

	//actualOffsetIndex=0;
}

	//////////////////set/factor//waiting///////////////
void HoeMusic_Module::setFactor(float factor){
#ifndef HOE_NO_BASS
	
	if(factor<0.0f) factor=0.0f;
	if(factor>1.0f) factor=1.0f;
	
	int factorPos=getOffsetPosByFactor(factor);
	
	if(factorPos==actualOffsetIndex) return;
	
	if (actualOffsetIndex == -1)
	{
		setPos(offsets[factorPos].start);
		actualOffsetIndex = factorPos;
	}

	BASSMOD_MusicSetSync(BASS_SYNC_POS/*|BASS_SYNC_ONETIME*/, offsets[actualOffsetIndex].end, 
		PosSyncProc, (DWORD)this);

	actualOffsetIndex = factorPos;
#endif
}

////////////////////////////////////////////////////
//	HoeMusic
//
////////////////////////////////////////////////////
HoeMusic::HoeMusic(HoeMusic_Module Module){
	module=Module;
	module.Init(NULL);
}

HoeMusic::~HoeMusic() {}

bool HoeMusic::Load(void *file){
	
	return module.Load(file);
}

//void HoeMusic::setPos(dword pos){
//	module.setPos(pos);
//}

void HoeMusic::setFactor(float factor){
	module.setFactor(factor);
}

bool HoeMusic::Init(){
	return module.Init(NULL);
}

void HoeMusic::Play(){
	module.Play();
}

void HoeMusic::Pause(){
	module.Pause();
}

void HoeMusic::Stop(){
	module.Stop();
}
////////////////////////////////////////////////////

} // namespace HoeGame
