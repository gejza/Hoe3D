
#include "system.h"
#include "shared.h"
#include "utils.h"
#include "../include/hoe3d.h"
#include "sound.h"

#ifdef _HOE_DS8_

#pragma comment (lib,"dsound.lib")

SoundSystemDS::SoundSystemDS()
{
	m_pDS = NULL;
}

bool SoundSystemDS::Init(THoeInitSettings * his)
{
	HRESULT hRes;

	// Create IDirectSound using the primary sound device
    hRes = DirectSoundCreate8( NULL, &m_pDS, NULL );
	checkres(hRes,"DirectSoundCreate8");

    // Set DirectSound coop level 
	hRes = m_pDS->SetCooperativeLevel( his->win, DSSCL_PRIORITY );
	checkres(hRes,"SetCooperativeLevel");

	Con_Print("DirectSoundCreate8 OK");

	return true;
}

void SoundSystemDS::Destroy()
{
	SAFE_RELEASE(m_pDS);
}

//////////////////////////////////////

HoeDSBuffer::HoeDSBuffer()
{
}

bool HoeDSBuffer::LoadFromFile(const char *filename)
{
	return true;
}

//////////////////////////////////////

HoeDSPlayer::HoeDSPlayer()
{
}

void HoeDSPlayer::Play(HoeDSBuffer * buff)
{
}

#endif // _HOE_DS8_

