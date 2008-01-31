/**
   @file    ref_dd.cpp
   @date    Jan 2008
   @version 1.0
   @brief   Hlavni soubor implementace Direct Draw
*/

#include "StdAfx.h"
#include "shared.h"
#include "utils.h"
#include "ref_dd.h"
#include "../include/hoe3d.h"
#include "config.h"

RefDD::RefDD()
{
	Con_Print(T("------ Direct Draw ------"));
	//m_pD3D = Direct3DMobileCreate( D3DM_SDK_VERSION );
    HRESULT hRes = DirectDrawCreateEx(NULL, (VOID**)&m_pDD, IID_IDirectDraw7, NULL);
	checkres(hRes, "DirectDrawCreateEx");

	//// mode
	//uint modes = m_pD3D->GetAdapterModeCount(0);
	//for (int i=0;i < modes;i++)
	//{
	//	D3DMDISPLAYMODE mode;
	//	m_pD3D->EnumAdapterModes(m_Adapter, i, &mode);
	//	i=i;
	//}
}

bool RefDD::Init(THoeInitSettings * his)
{
	///////////////////////////////////////////////////////////////////////////
    // Create the main DirectDraw object
    ///////////////////////////////////////////////////////////////////////////
	HRESULT hRes;
	m_hWnd = his->win;
	
	m_Fullscreen = GetConfig()->IsFullscreen();
	m_Width = GetConfig()->GetWidthView();
	m_Height = GetConfig()->GetHeightView();

    // Get exclusive mode
    hRes = m_pDD->SetCooperativeLevel(m_hWnd, DDSCL_NORMAL/*DDSCL_EXCLUSIVE | DDSCL_FULLSCREEN*/);
	checkres(hRes, "SetCooperativeLevel");

    // Set the video mode to 640x480x8
    /*hRet = g_pDD->SetDisplayMode(640, 480, 8, 0, 0);
    checkres(hRes, "SetDisplayMode");
	*/
    // Create the primary surface with 1 back buffer
	DDSURFACEDESC2 ddsd;
    ZeroMemory(&ddsd, sizeof(ddsd));
    ddsd.dwSize = sizeof(ddsd);
    ddsd.dwFlags = DDSD_CAPS/* | DDSD_BACKBUFFERCOUNT*/;
    ddsd.ddsCaps.dwCaps = DDSCAPS_PRIMARYSURFACE/* | DDSCAPS_FLIP |
                          DDSCAPS_COMPLEX*/;
    ddsd.dwBackBufferCount = 1;
    hRes = m_pDD->CreateSurface(&ddsd, &m_pDDSPrimary, NULL);
    checkres(hRes, "CreateSurface");

	// Create a clipper object since this is for a Windowed render
	LPDIRECTDRAWCLIPPER pClipper;
    hRes = m_pDD->CreateClipper(0, &pClipper, NULL);
    checkres(hRes, "CreateClipper");

    // Associate the clipper with the window
    pClipper->SetHWnd(0, m_hWnd);
    m_pDDSPrimary->SetClipper(pClipper);
    pClipper->Release();
    pClipper = NULL;

	// Get the backbuffer. For fullscreen mode, the backbuffer was created
        // along with the primary, but windowed mode still needs to create one.
    ddsd.dwFlags        = DDSD_WIDTH | DDSD_HEIGHT | DDSD_CAPS;
    ddsd.dwWidth        = 640;
    ddsd.dwHeight       = 480;
    ddsd.ddsCaps.dwCaps = DDSCAPS_OFFSCREENPLAIN;
    hRes = m_pDD->CreateSurface(&ddsd, &m_pDDSBack, NULL);
    checkres(hRes, "CreateSurface");

	return true;
}

bool RefDD::Begin()
{
	DDBLTFX fx;
	memset(&fx,0, sizeof(fx));
	fx.dwSize = sizeof(fx);
	fx.dwFillColor = 0x00ff;
	RECT rect;
	rect.top = rect.left = 0;
	rect.bottom = 480;
	rect.right = 640;
	HRESULT hRes = m_pDDSBack->Blt(&rect, NULL, NULL, DDBLT_COLORFILL,&fx);
	checkres(hRes, "Blt");
	return true;
}

void RefDD::End()
{
	RECT g_rcWindow;             // Saves the window size & pos.
	RECT g_rcViewport;           // Pos. & size to blt from
	RECT g_rcScreen;             // Screen pos. for blt 
	// Get the dimensions of the viewport and screen bounds
	GetClientRect(m_hWnd, &g_rcViewport);
	GetClientRect(m_hWnd, &g_rcScreen);
	ClientToScreen(m_hWnd, (POINT*)&g_rcScreen.left);
	ClientToScreen(m_hWnd, (POINT*)&g_rcScreen.right);

	HDC hdc;
	HRESULT hRes;
	hRes = m_pDDSBack->GetDC(&hdc);
	checkres(hRes, "GetDC");
	static int v = 0;
	v++;
	HoeCore::String_s<50> aaa;
	DWORD sl;
	m_pDD->GetScanLine(&sl);

	aaa.printf(T("%d  -  %d   "), v, sl);

	SetBkColor( hdc, RGB( 0, 0, 255 ) );
	SetTextColor( hdc, RGB( 255, 255, 0 ) );
	TextOut( hdc, 10, 10, aaa, HoeCore::string::len(aaa));
	m_pDDSBack->ReleaseDC(hdc);

	hRes = m_pDDSPrimary->Blt(&g_rcScreen, m_pDDSBack,
       &g_rcViewport, DDBLT_WAIT, NULL);
	checkres(hRes, "Primary::Blt");

}

const tchar * RefDD::GetErrorString(HRESULT hRes)
{
	switch (hRes)
	{
#define CASE_ERR(err) case err: return T( #err );
	CASE_ERR(DD_OK)
	CASE_ERR(DDERR_INCOMPATIBLEPRIMARY  )
	CASE_ERR(DDERR_INVALIDCAPS  )
	CASE_ERR(DDERR_INVALIDOBJECT  )
	CASE_ERR(DDERR_INVALIDPARAMS  )
	CASE_ERR(DDERR_INVALIDPIXELFORMAT  )
	CASE_ERR(DDERR_NOALPHAHW  )
	CASE_ERR(DDERR_NOCOOPERATIVELEVELSET  )
	CASE_ERR(DDERR_NODIRECTDRAWHW  )
	CASE_ERR(DDERR_NOEMULATION  )
	CASE_ERR(DDERR_NOEXCLUSIVEMODE  )
	CASE_ERR(DDERR_NOFLIPHW  )
	CASE_ERR(DDERR_NOMIPMAPHW  )
	CASE_ERR(DDERR_NOOVERLAYHW  )
	CASE_ERR(DDERR_NOZBUFFERHW  )
	CASE_ERR(DDERR_OUTOFMEMORY  )
	CASE_ERR(DDERR_OUTOFVIDEOMEMORY  )
	CASE_ERR(DDERR_PRIMARYSURFACEALREADYEXISTS  )
	CASE_ERR(DDERR_UNSUPPORTEDMODE  )
	CASE_ERR(DDERR_CURRENTLYNOTAVAIL)
#undef CASE_ERR
	}
	static HoeCore::String_s<20> buff;
	int f = (hRes >> 16) & 0x7FFF;
	int c = hRes & 0xFFFF;
	if (f == _FACDD)
		buff.printf(T("DD Error(%d)"), f, c);
	else
		buff.printf(T("Fac:%x(%d)"), f, c);
	return buff;
}



#if 0
bool RefD3DM::Init(THoeInitSettings * his)
{
	HRESULT hRes;
	m_hWnd = his->win;
	
	m_Fullscreen = GetConfig()->IsFullscreen();
	m_Width = GetConfig()->GetWidthView();
	m_Height = GetConfig()->GetHeightView();

	D3DMADAPTER_IDENTIFIER ai;
	hRes = m_pD3D->GetAdapterIdentifier(m_Adapter,0,&ai);
	if (FAILED(hRes))
	{
		Con_Print("IDirect3DMobile::GetAdapterIdentifier failed");
		return false;
	}
	Con_Print("%s %s",ai.Driver,ai.Description);

    /*hRes = m_pD3D->CheckDeviceType(m_Adapter,D3DMDEVTYPE_DEFAULT,D3DFMT_X8R8G8B8,D3DFMT_X8R8G8B8,FALSE);
	if (FAILED(hRes))
	{
		Con_Print("HAL on adapter %s not supported!",m_Adapter);
		return false;
	}*/

	D3DMPRESENT_PARAMETERS d3dpp; 
	ZeroMemory( &d3dpp, sizeof(d3dpp) );
	/*if (!m_Fullscreen)
	{
		d3dpp.Windowed = TRUE;
		d3dpp.SwapEffect = D3DSWAPEFFECT_DISCARD;
		d3dpp.BackBufferFormat = D3DFMT_UNKNOWN;
		d3dpp.EnableAutoDepthStencil = TRUE;
		d3dpp.AutoDepthStencilFormat = D3DFMT_D24X8;
	}
	else
	{*/
	    //D3DMPRESENT_PARAMETERS d3dpp; 
    /*memset( &d3dpp, 0, sizeof(d3dpp) );
    d3dpp.Windowed = TRUE;
    d3dpp.SwapEffect = D3DMSWAPEFFECT_DISCARD;
    d3dpp.BackBufferFormat = D3DMFMT_UNKNOWN;
    d3dpp.EnableAutoDepthStencil = TRUE;
    d3dpp.AutoDepthStencilFormat = D3DMFMT_D16; 
	*/
	    int iScreenWidth = GetSystemMetrics(SM_CXSCREEN);
    int iScreenHeight = GetSystemMetrics(SM_CYSCREEN);

		d3dpp.Windowed = FALSE;
		d3dpp.SwapEffect = D3DMSWAPEFFECT_FLIP;
		d3dpp.BackBufferFormat = D3DMFMT_R5G6B5;
		d3dpp.EnableAutoDepthStencil = TRUE;
		d3dpp.AutoDepthStencilFormat = D3DMFMT_D16;
		d3dpp.BackBufferWidth = iScreenWidth;
		d3dpp.BackBufferHeight = iScreenHeight;
		d3dpp.BackBufferCount = 1;
	//}

	hRes = m_pD3D->CreateDevice( m_Adapter, D3DMDEVTYPE_DEFAULT, m_hWnd,
                                  0,
                                  &d3dpp, &m_Dev );
	if (FAILED( hRes ))
	{
		Con_Print("Hardware vertex processing not supported!");
		/*hRes = m_pD3D->CreateDevice( m_Adapter, D3DDEVTYPE_HAL, m_hWnd,
									D3DCREATE_MIXED_VERTEXPROCESSING,
									&d3dpp, &m_Dev );*/
	}

    // Turn off culling
    m_Dev->SetRenderState( D3DMRS_CULLMODE, D3DMCULL_NONE );

    // Turn off D3D lighting
    m_Dev->SetRenderState( D3DMRS_LIGHTING, FALSE );

    // Turn on the zbuffer
    m_Dev->SetRenderState( D3DMRS_ZENABLE, TRUE );


	/*if (FAILED( hRes ))
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
	m_AdapterFormat = d3dpp.BackBufferFormat;
	m_Dev->GetDeviceCaps(&m_Caps);

	return true;*/
	return true;
}

bool RefD3DM::Begin()
{
	// Begin the scene.
	m_Dev->BeginScene();
	return true;
}

void RefD3DM::End()
{
	// End the scene.
	m_Dev->EndScene();
	m_Dev->Present( NULL, NULL, NULL, NULL );
}

void RefD3DM::ClearBuffers(bool target, bool depth)
{
	// Clear the back buffer to a blue color
	m_Dev->Clear( 0, NULL, (target?D3DMCLEAR_TARGET:0) | (depth?D3DMCLEAR_ZBUFFER:0), m_BackColor, 1.0f, 0 );
}

void RefD3DM::Destroy()
{
    if( m_Dev != NULL)
        m_Dev->Release();
	m_Dev = NULL;

    if( m_pD3D != NULL)
        m_pD3D->Release();

	m_pD3D = NULL;
	Con_Print("Ref destroyed.");
}

void RefD3DM::DrawStdObject(HoeStream * stream, HoeIndex * index)
{
	stream->Set(0);

	D3DDevice()->SetIndices(index->GetIndexBuffer());
	register dword numtri = index->GetNumIndices()/3;
	HRESULT hRes = D3DDevice()->DrawIndexedPrimitive( D3DMPT_TRIANGLELIST, 0, 0, stream->GetNumVert(), 0,  numtri);
	checkres(hRes,"DrawIndexedPrimitive");
	GetInfo()->AddStatTriangles(numtri);
}

void RefD3DM::DrawStdObject(HoeStream * stream, HoeIndex * index, dword vert, dword ind)
{
	/*stream->Set(0);
	D3DDevice()->SetIndices(index->GetIndexBuffer());
	HRESULT hRes = D3DDevice()->DrawIndexedPrimitive( D3DPT_TRIANGLELIST, 0, 0, vert, 0,  ind/3);
	//hRes = D3DERR_INVALIDCALL;
	checkres(hRes,"DrawIndexedPrimitive");	
	GetInfo()->AddStatTriangles(ind/3);*/
}

void RefD3DM::DrawStdObjectFT(HoeStream * stream, HoeIndex * index, dword start, dword num)
{
	/*stream->Set(0);
	D3DDevice()->SetIndices(index->GetIndexBuffer());
	HRESULT hRes = D3DDevice()->DrawIndexedPrimitive( D3DPT_TRIANGLELIST, 0, 0, stream->GetNumVert(), start,  num/3);
	//hRes = D3DERR_INVALIDCALL;
	checkres(hRes,"DrawIndexedPrimitive");	
	GetInfo()->AddStatTriangles(num/3);*/
}

void RefD3DM::DrawIndex(HoeIndex * index, dword offset, dword count)
{
	/*assert(!"nefunkcni procedura, kvuli pevne nastavenemu poctu vertexu");
	D3DDevice()->SetIndices(index->GetIndexBuffer());
	HRESULT hRes = D3DDevice()->DrawIndexedPrimitive( D3DPT_TRIANGLELIST, 0, 0, 1000, offset,  count/3);
	checkres(hRes,"DrawIndexedPrimitive");
	GetInfo()->AddStatTriangles(count/3);*/
}

void RefD3DM::DrawFanObject(dword offset, dword count)
{
	/*HRESULT hRes = D3DDevice()->DrawPrimitive( D3DPT_TRIANGLEFAN, offset, count-2);
	checkres(hRes,"DrawPrimitive");
	GetInfo()->AddStatTriangles(count-2);*/
}

void RefD3DM::DrawPointObject(class HoeStream * stream, int vertCount)
{
	/*stream->Set(0);
	HRESULT hRes = D3DDevice()->DrawPrimitive( D3DPT_POINTLIST, 0, vertCount);
	checkres(hRes,"DrawPrimitive");*/
}

void RefD3DM::DrawLineObject(class HoeStream * stream, int lineCount)
{
	/*stream->Set(0);
	HRESULT hRes = D3DDevice()->DrawPrimitive( D3DPT_LINELIST, 0, lineCount);
	checkres(hRes,"DrawPrimitive");*/
}

// check

bool RefD3DM::IsTextureFormatOk( HOEFORMAT TextureFormat) 
{
	/*HRESULT hr = m_pD3D->CheckDeviceFormat( m_Adapter,
                                          D3DDEVTYPE_HAL,
										  m_AdapterFormat,
                                          0,
                                          D3DRTYPE_TEXTURE,
                                          HoeFormatX(TextureFormat));
    
    return SUCCEEDED( hr );*/
	return 1;
}

IDirect3DMobileVertexBuffer * RefD3DM::CreateVertexBuffer(dword size, HoeFVF& fvf,
		bool dynamic)
{
	IDirect3DMobileVertexBuffer * vb;
	const DWORD dfvf = fvf.GetFVF();
	HRESULT hRes = m_Dev->CreateVertexBuffer( size,
		dynamic ? D3DUSAGE_DYNAMIC:0 /* Usage */, dfvf, D3DMPOOL_SYSTEMMEM, &vb );
	checkres(hRes, "CreateVertexBuffer");
	return vb;
}

const char * RefD3DM::GetErrorString(HRESULT hRes)
{
	switch (hRes)
	{
#define CASE_ERR(err) case err: return #err;
	CASE_ERR(D3DM_OK)
	CASE_ERR(D3DM_GENERICFAILURE)
	CASE_ERR(D3DMERR_DRIVERINTERNALERROR)
	CASE_ERR(D3DMERR_INVALIDDEVICE)
	CASE_ERR(D3DMERR_INVALIDCALL)
	CASE_ERR(D3DMERR_DRIVERINVALIDCALL)
	CASE_ERR(D3DMERR_DRIVERUNSUPPORTED)
	CASE_ERR(D3DMERR_WRONGTEXTUREFORMAT)
	CASE_ERR(D3DMERR_UNSUPPORTEDCOLOROPERATION)
	CASE_ERR(D3DMERR_UNSUPPORTEDCOLORARG)
	CASE_ERR(D3DMERR_UNSUPPORTEDALPHAOPERATION)
	CASE_ERR(D3DMERR_UNSUPPORTEDALPHAARG)
	CASE_ERR(D3DMERR_TOOMANYOPERATIONS)
	CASE_ERR(D3DMERR_CONFLICTINGTEXTUREFILTER)
	CASE_ERR(D3DMERR_UNSUPPORTEDFACTORVALUE)
	CASE_ERR(D3DMERR_CONFLICTINGRENDERSTATE)
	CASE_ERR(D3DMERR_UNSUPPORTEDTEXTUREFILTER)
	CASE_ERR(D3DMERR_CONFLICTINGTEXTUREPALETTE)
	CASE_ERR(D3DMERR_NOTFOUND)
	CASE_ERR(D3DMERR_MOREDATA)
	CASE_ERR(D3DMERR_DEVICELOST)
	CASE_ERR(D3DMERR_DEVICENOTRESET)
	CASE_ERR(D3DMERR_NOTAVAILABLE)
	CASE_ERR(D3DMERR_MEMORYPOOLEMPTY)
	CASE_ERR(D3DMERR_ALREADYLOCKED)
#undef CASE_ERR
	default:
		return "unknown";
	};
}

#endif






