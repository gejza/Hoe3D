
#ifndef HOE_MUSIC_H_
#define HOE_MUSIC_H_

#include "hoe_game.h"

class BaseConsole;

#ifndef _WIN32
typedef unsigned long DWORD;
#define CALLBACK
#endif

#ifndef BASSMOD_H 
typedef DWORD HSYNC;		// synchronizer handle 
#endif

BEGIN_HOEGAME

typedef DWORD HSYNC;

struct OffsetLoop
{
	DWORD start;
	DWORD end;
};

class HoeMusic_Module{
private:	
	int getOffsetPosByFactor(float factor);
	void setPos(DWORD pos);
	static void CALLBACK PosSyncProc(HSYNC handle, DWORD data, DWORD params);
	//	[0] start	[1] end
	//	end je prvni radkou stranky, ktera je prvni za koncem bloku
	OffsetLoop *offsets;	
	int offsetsLength;
	int actualOffsetIndex;
	BaseConsole * console;
public:
	HoeMusic_Module();
	HoeMusic_Module(OffsetLoop *Offsets);
	~HoeMusic_Module();

	bool Init(BaseConsole * con);
	bool Load(void *file);
	void Destroy();

	void Play();
	void Pause();
	void Stop();

	void setOffsets(OffsetLoop *Offsets);
	void setFactor(float factor);

};


class HoeMusic{

private:
	HoeMusic_Module module;

public:
	HoeMusic(HoeMusic_Module Module);
	~HoeMusic();

	bool Init();
	bool Load(void *file);
	void Play();
	void Pause();
	void Stop();
	void setFactor(float factor);
};

END_HOEGAME


#endif

