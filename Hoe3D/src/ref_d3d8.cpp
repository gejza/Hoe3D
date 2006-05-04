/**
   @file    ref_d3d8.cpp
   @date    Mar 2006
   @version 1.0
   @brief   Hlavni soubor implementace Direct3D8.
*/

#include "system.h"
#include "shared.h"
#include "utils.h"
#include "ref_d3d8.h"
#include "hoe3d_math.h"
#include "../include/hoe3d.h"
#include "config.h"
#include "hoe_stream.h"
#include "hoe_index.h"
#include "hoe_info.h"

#pragma comment (lib,"d3d8.lib")

// hoe
IDirect3DDevice8 * RefD3D8::m_Dev = NULL;

RefD3D8::RefD3D8()
{
	Con_Print("------ Direct X 8.1 ------");
	m_pD3D = Direct3DCreate8(D3D_SDK_VERSION);

	m_Adapter = D3DADAPTER_DEFAULT;
}

bool RefD3D8::Init(THoeInitSettings * his)
{
	HRESULT hRes;
	m_hWnd = his->win;
	
	m_Fullscreen = GetConfig()->IsFullscreen();
	m_Width = GetConfig()->GetWidthView();
	m_Height = GetConfig()->GetHeightView();

	D3DADAPTER_IDENTIFIER8 ai;
	hRes = m_pD3D->GetAdapterIdentifier(m_Adapter,0,&ai);
	if (hRes == D3DERR_INVALIDCALL)
	{
		Con_Print("IDirect3D9::GetAdapterIdentifier failed");
		return false;
	}
	Con_Print("%s %s",ai.Driver,ai.Description);
	

    hRes = m_pD3D->CheckDeviceType(m_Adapter,D3DDEVTYPE_HAL,D3DFMT_X8R8G8B8,D3DFMT_X8R8G8B8,FALSE);
	if (hRes == D3DERR_INVALIDCALL)
	{
		Con_Print("HAL on adapter %s not supported!",m_Adapter);
		return false;
	}

	D3DPRESENT_PARAMETERS d3dpp; 
	ZeroMemory( &d3dpp, sizeof(d3dpp) );
	if (!m_Fullscreen)
	{
		d3dpp.Windowed = TRUE;
		d3dpp.SwapEffect = D3DSWAPEFFECT_DISCARD;
		d3dpp.BackBufferFormat = D3DFMT_X8R8G8B8;
		d3dpp.EnableAutoDepthStencil = TRUE;
		d3dpp.BackBufferCount = 1;
		d3dpp.AutoDepthStencilFormat = D3DFMT_D16;
	}
	else
	{
		d3dpp.Windowed = FALSE;
		d3dpp.SwapEffect = D3DSWAPEFFECT_FLIP;
		d3dpp.BackBufferFormat = D3DFMT_X8R8G8B8;
		d3dpp.EnableAutoDepthStencil = TRUE;
		d3dpp.AutoDepthStencilFormat = D3DFMT_D16;
		d3dpp.BackBufferWidth = m_Width;
		d3dpp.BackBufferHeight = m_Height;
		d3dpp.BackBufferCount = 1;
		d3dpp.FullScreen_PresentationInterval = D3DPRESENT_INTERVAL_IMMEDIATE;
	}

	hRes = m_pD3D->CreateDevice( m_Adapter, D3DDEVTYPE_HAL, m_hWnd,
                                  D3DCREATE_HARDWARE_VERTEXPROCESSING,
                                  &d3dpp, &m_Dev );
	if (FAILED( hRes ))
	{
		Con_Print("Hardware vertex processing not supported!");
		hRes = m_pD3D->CreateDevice( m_Adapter, D3DDEVTYPE_HAL, m_hWnd,
									D3DCREATE_MIXED_VERTEXPROCESSING,
									&d3dpp, &m_Dev );
	}

	if (FAILED( hRes ))
	{
		Con_Print("Use software vertex processing!");
		hRes = m_pD3D->CreateDevice( m_Adapter, D3DDEVTYPE_HAL, m_hWnd,
									D3DCREATE_SOFTWARE_VERTEXPROCESSING,
									&d3dpp, &m_Dev );
	}

	if( FAILED( hRes ) )
	{
		Con_Print("Failed create device HRESULT: 0x%x",hRes);
		return false;
	}		

	return true;
}

bool RefD3D8::Begin()
{
	// Clear the back buffer to a blue color
	m_Dev->Clear( 0, NULL, D3DCLEAR_TARGET | D3DCLEAR_ZBUFFER, m_BackColor, 1.0f, 0 );
	
	// Begin the scene.
	m_Dev->BeginScene();
	return true;
}

void RefD3D8::End()
{
	// End the scene.
	m_Dev->EndScene();
	m_Dev->Present( NULL, NULL, NULL, NULL );
}


void RefD3D8::Destroy()
{
    if( m_Dev != NULL)
        m_Dev->Release();
	m_Dev = NULL;

    if( m_pD3D != NULL)
        m_pD3D->Release();

	m_pD3D = NULL;
	Con_Print("Ref destroyed.");
}

void RefD3D8::DrawStdObject(HoeStream * stream, HoeIndex * index, dword vert, dword ind)
{
	stream->Set(0);
	D3DDevice()->SetIndices(index->GetIndexBuffer(),0);
	HRESULT hRes = D3DDevice()->DrawIndexedPrimitive( D3DPT_TRIANGLELIST, 0, vert, 0,  ind/3);
	assert(hRes == S_OK);
	
	GetInfo()->AddStatTriangles(ind/3);
}

void RefD3D8::DrawPointObject(class HoeStream * stream, int vertCount)
{
	stream->Set(0);
	HRESULT hRes = D3DDevice()->DrawPrimitive( D3DPT_POINTLIST, 0, vertCount);
	if (FAILED(hRes))
		exit(1);
}

void RefD3D8::DrawLineObject(class HoeStream * stream, int lineCount)
{
	stream->Set(0);
	HRESULT hRes = D3DDevice()->DrawPrimitive( D3DPT_LINELIST, 0, lineCount);
	if (FAILED(hRes))
		exit(1);
}





