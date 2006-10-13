
#include "StdAfx.h"
#include "shared.h"
#include "utils.h"
#include "../include/hoe3d.h"
#include "sound.h"

#ifdef _HOE_DS8_

#pragma comment (lib,"dsound.lib")

SoundSystemDS::SoundSystemDS()
{
	m_pDS = NULL;
	m_listener = NULL;
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

	// GetListener
    DSBUFFERDESC        dsbdesc;
    LPDIRECTSOUNDBUFFER pDSBPrimary = NULL;

    // Obtain primary buffer, asking it for 3D control
    ZeroMemory( &dsbdesc, sizeof(DSBUFFERDESC) );
    dsbdesc.dwSize = sizeof(DSBUFFERDESC);
    dsbdesc.dwFlags = DSBCAPS_CTRL3D | DSBCAPS_PRIMARYBUFFER;
    hRes = m_pDS->CreateSoundBuffer( &dsbdesc, &pDSBPrimary, NULL );
    checkres(hRes, "CreateSoundBuffer");

    hRes = pDSBPrimary->QueryInterface( IID_IDirectSound3DListener8,
                                                  (VOID**)&m_listener );
    checkres(hRes,"QueryInterface");

    // Release the primary buffer, since it is not need anymore
    SAFE_RELEASE( pDSBPrimary );

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
	m_buffer = NULL;
}

HoeDSBuffer::~HoeDSBuffer()
{
	SAFE_RELEASE(m_buffer);
	SAFE_RELEASE(m_3d);
}

bool HoeDSBuffer::Create(int channels, int freq, int byts, long samples, bool ctrl3D)
{
	WAVEFORMATEX wfx; 
  DSBUFFERDESC dsbdesc; 
  LPDIRECTSOUNDBUFFER pDsb = NULL;
  HRESULT hr; 
 
  // Set up WAV format structure. 

  memset(&wfx, 0, sizeof(WAVEFORMATEX)); 
  wfx.wFormatTag = WAVE_FORMAT_PCM; 
  wfx.nChannels = channels; 
  wfx.nSamplesPerSec = freq; 
  wfx.nBlockAlign = byts*channels; 
  wfx.nAvgBytesPerSec = wfx.nSamplesPerSec * wfx.nBlockAlign; 
  wfx.wBitsPerSample = byts*8; 
 
  // Set up DSBUFFERDESC structure. 
 
  memset(&dsbdesc, 0, sizeof(DSBUFFERDESC)); 
  dsbdesc.dwSize = sizeof(DSBUFFERDESC); 
  dsbdesc.dwFlags = ctrl3D ? DSBCAPS_CTRL3D:DSBCAPS_CTRLVOLUME; 
  dsbdesc.dwBufferBytes = channels * samples * byts; 
  dsbdesc.lpwfxFormat = &wfx; 
 
  // Create buffer. 
 
  hr = GetSound()->GetHandle()->CreateSoundBuffer(&dsbdesc, &pDsb, NULL); 
  checkres(hr, "CreateSoundBuffer");
  if (SUCCEEDED(hr)) 
  { 
     hr = pDsb->QueryInterface(IID_IDirectSoundBuffer8, (LPVOID*) &m_buffer);
	 if (ctrl3D)
		 hr = pDsb->QueryInterface(IID_IDirectSound3DBuffer8, (LPVOID*) &m_3d);
	 else
		 m_3d = NULL;
     pDsb->Release();
	  //m_buffer = pDsb;
  } 
  
  return SUCCEEDED(hr);

}

byte * HoeDSBuffer::Lock()
{


	m_buffer->Lock(0,          // Offset at which to start lock.
      0,          // Size of lock; ignored because of flag.
      &lpvWrite,  // Gets address of first part of lock.
      &dwLength,  // Gets size of first part of lock.
      NULL,       // Address of wraparound not needed. 
      NULL,       // Size of wraparound not needed.
      DSBLOCK_ENTIREBUFFER);  // Flag.
	return (byte*)lpvWrite;
}

void HoeDSBuffer::Unlock()
{
	m_buffer->Unlock(
      lpvWrite,   // Address of lock start.
      dwLength,   // Size of lock.
      NULL,       // No wraparound portion.
      0);         // No wraparound size.

m_buffer->SetCurrentPosition(0);
if (m_3d)
{
	m_3d->SetMaxDistance(100.f, DS3D_IMMEDIATE);
	m_3d->SetPosition(0,0.f,0,DS3D_IMMEDIATE);
HRESULT hr = m_buffer->Play(
	0,  // Unused.
	0,  // Priority for voice management.
	DSBPLAY_LOOPING); // Flags.
	
	assert(SUCCEEDED(hr));
}

}

//////////////////////////////////////

HoeDSPlayer::HoeDSPlayer()
{
}

void HoeDSPlayer::Play(HoeDSBuffer * buff, bool loop)
{
	m_buff = buff;
	if (!buff)
		return;
	HRESULT hr = buff->GetHandle()->Play(0,0,loop ? DSBPLAY_LOOPING:0); // Flags.
	checkres(hr,"IDirectSoundBuffer8::Play");
}

void HoeDSPlayer::Stop()
{
	if (!m_buff)
		return;
	m_buff->GetHandle()->Stop();
}

#endif // _HOE_DS8_

