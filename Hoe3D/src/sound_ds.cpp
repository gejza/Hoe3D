
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

bool HoeDSBuffer::Create(int channels, int freq, int byts, long samples)
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
  wfx.nBlockAlign = 2; 
  wfx.nAvgBytesPerSec = wfx.nSamplesPerSec * wfx.nBlockAlign; 
  wfx.wBitsPerSample = byts*8; 
 
  // Set up DSBUFFERDESC structure. 
 
  memset(&dsbdesc, 0, sizeof(DSBUFFERDESC)); 
  dsbdesc.dwSize = sizeof(DSBUFFERDESC); 
  dsbdesc.dwFlags = 
    DSBCAPS_CTRL3D | DSBCAPS_CTRLVOLUME; 
  dsbdesc.dwBufferBytes = samples * 2; 
  dsbdesc.lpwfxFormat = &wfx; 
 
  // Create buffer. 
 
  hr = GetSound()->GetHandle()->CreateSoundBuffer(&dsbdesc, &pDsb, NULL); 
  if (SUCCEEDED(hr)) 
  { 
     hr = pDsb->QueryInterface(IID_IDirectSoundBuffer8, (LPVOID*) &m_buffer);
     hr = pDsb->QueryInterface(IID_IDirectSound3DBuffer8, (LPVOID*) &m_3d);
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
HRESULT hr = m_buffer->Play(
    0,  // Unused.
    0,  // Priority for voice management.
    DSBPLAY_LOOPING); // Flags.
assert(SUCCEEDED(hr));

m_3d->SetMaxDistance(1000.f, DS3D_IMMEDIATE);
m_3d->SetPosition(0,100.f,0,DS3D_IMMEDIATE);
}

//////////////////////////////////////

HoeDSPlayer::HoeDSPlayer()
{
}

void HoeDSPlayer::Play(HoeDSBuffer * buff)
{
}

#endif // _HOE_DS8_

