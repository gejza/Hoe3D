
#include "StdAfx.h"
#include "utils.h"
#include "hoe_input.h"
#include "hoe_input_di.h"
#include "shared.h"
#include "states.h"
#include "../include/hoe3d.h"

#ifdef _INPUT_DI

#pragma comment (lib,"dinput8.lib")
#pragma comment (lib,"dxguid.lib")

#define SAMPLE_BUFFER_SIZE 10

HoeInputDI::HoeInputDI()
{
	m_lpDI = NULL;
	m_lpDIKeyb = NULL;
	m_lpDIMous = NULL;
	m_type = MT_ForceDW;

	Con_Print(T("DirectInput system created."));
}

const tchar * HoeInputDI::GetName()
{
	return T("DirectInput System");
}

bool HoeInputDI::Init(THoeInitSettings * his)
{
	HRESULT hRes;
	m_inst = his->hInst;
	m_hwnd = his->win;
	
	hRes = DirectInput8Create(m_inst, DIRECTINPUT_VERSION, 
        IID_IDirectInput8, (void**)&m_lpDI, NULL); 

	if (FAILED(hRes))
	{
		m_lpDI = 0;
		Con_Print(T("DirectInput8Create FAILED: 0x%x"),hRes);
		return false;
	}

	Con_Print(T("DirectInput8Create OK"));

	return true;
}

bool HoeInputDI::UseKeyb()
{
	HRESULT hRes;

	if (m_lpDIKeyb)
		return true;

	hRes = m_lpDI->CreateDevice(GUID_SysKeyboard, &m_lpDIKeyb, NULL); 
	checkres(hRes,"CreateDevice");

	hRes = m_lpDIKeyb->SetDataFormat(&c_dfDIKeyboard); 
	checkres(hRes,"SetDataFormat");

	hRes = m_lpDIKeyb->SetCooperativeLevel(m_hwnd, 
		DISCL_FOREGROUND | DISCL_NONEXCLUSIVE); 
	checkres(hRes,"SetCooperativeLevel");

	// imediate
	DIPROPDWORD dipdw;
        dipdw.diph.dwSize       = sizeof(DIPROPDWORD);
        dipdw.diph.dwHeaderSize = sizeof(DIPROPHEADER);
        dipdw.diph.dwObj        = 0;
        dipdw.diph.dwHow        = DIPH_DEVICE;
        dipdw.dwData            = SAMPLE_BUFFER_SIZE; // Arbitary buffer size

     hRes = m_lpDIKeyb->SetProperty( DIPROP_BUFFERSIZE, &dipdw.diph );

	m_lpDIKeyb->Acquire();

	return true;
}

bool HoeInputDI::InstallMouse(MouseType mt)
{
	HRESULT hRes;

	if (m_lpDIMous == NULL)
	{
		hRes = m_lpDI->CreateDevice(GUID_SysMouse, &m_lpDIMous, NULL); 
		checkres(hRes,"CreateDevice");

		hRes = m_lpDIMous->SetDataFormat(&c_dfDIMouse2); 
		checkres(hRes,"SetDataFormat");
	}
	else
	{
		if (mt != m_type)
			m_lpDIMous->Unacquire();
	}

	if (mt != m_type)
	{
		if (mt == MT_Foreground || mt == MT_Cursored)
			hRes = m_lpDIMous->SetCooperativeLevel(m_hwnd, DISCL_EXCLUSIVE | DISCL_FOREGROUND); 
		else
			hRes = m_lpDIMous->SetCooperativeLevel(m_hwnd, DISCL_NONEXCLUSIVE | DISCL_BACKGROUND); 
		checkres(hRes,"SetCooperativeLevel");

		m_lpDIMous->Acquire();
		m_type = mt;
	}

	return true;
}

void HoeInputDI::UninstallMouse()
{
	if (m_lpDIMous != NULL)
	{
		m_lpDIMous->Unacquire();
		SAFE_RELEASE(m_lpDIMous);
	}
}

void HoeInputDI::Process(float time)
{
	HRESULT hr;
	if (m_lpDIKeyb)
	{
		DIDEVICEOBJECTDATA didod[SAMPLE_BUFFER_SIZE];  // Receives buffered data 
		DWORD              dwElements;
		DWORD				i;

		dwElements = SAMPLE_BUFFER_SIZE;
		hr = m_lpDIKeyb->GetDeviceData( sizeof(DIDEVICEOBJECTDATA),
										 didod, &dwElements, 0 );
		if( hr != DI_OK ) 
		{
			hr = m_lpDIKeyb->Acquire();
			while( hr == DIERR_INPUTLOST ) 
				hr = m_lpDIKeyb->Acquire();
			return; 
		}

		if (m_keyb)
		{
			for(i = 0; i < dwElements; i++ ) 
			{
				if (didod[ i ].dwData & 0x80)
					m_keyb->_KeyDown(didod[i].dwOfs);
				else
					m_keyb->_KeyUp(didod[i].dwOfs);
			}
		}
	}

	static byte laststate[8] = {0};

	if (m_lpDIMous)
	{
		DIMOUSESTATE2 dims2;      // DirectInput mouse state structure

		// Get the input's device state, and put the state in dims
		ZeroMemory( &dims2, sizeof(dims2) );
		hr = m_lpDIMous->GetDeviceState( sizeof(DIMOUSESTATE2), &dims2 );

		if (FAILED(hr)) 
		{
			 hr = m_lpDIMous->Acquire();
			while( hr == DIERR_INPUTLOST ) 
				hr = m_lpDIMous->Acquire();
			return;
		}

		if (m_mouse)
		{
			if (dims2.lX || dims2.lY)
			{
				if (m_absolutMouse)
				{
					POINT point;
					GetCursorPos(&point);
					ScreenToClient(m_hwnd,&point);
					MoveAbsolut((float)point.x, (float)point.y);
				}
				else
				{
					m_mouse->_MouseMove((float)dims2.lX, (float)dims2.lY);
				}
			}

			if (dims2.lZ != 0)
				m_mouse->_Wheel(dims2.lZ);

			for (uint u=0; u < 3;u++)
			{
				if (!laststate[u] && dims2.rgbButtons[u])
					m_mouse->_ButtonDown(u);
				else if (laststate[u] && !dims2.rgbButtons[u])
					m_mouse->_ButtonUp(u);

				laststate[u] = dims2.rgbButtons[u];
			}
		}
		
	}
}


#endif

