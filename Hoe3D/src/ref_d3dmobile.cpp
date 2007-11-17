/**
   @file    ref_d3d9.cpp
   @date    Sep 2007
   @version 1.0
   @brief   Hlavni soubor implementace Direct Mobile
*/

#include "StdAfx.h"
#include "shared.h"
#include "utils.h"
#include "ref_d3dmobile.h"
#include <hoe_math.h>
#include "../include/hoe3d.h"
#include "config.h"
#include "hoe_stream.h"
#include "hoe_index.h"
#include "hoe_info.h"

// hoe
IDirect3DMobileDevice * RefD3DM::m_Dev = NULL;

RefD3DM::RefD3DM()
{
	Con_Print("------ Direct X Mobile ------");
	m_pD3D = Direct3DMobileCreate( D3DM_SDK_VERSION );

	// mode
	uint modes = m_pD3D->GetAdapterModeCount(0);
	for (int i=0;i < modes;i++)
	{
		D3DMDISPLAYMODE mode;
		m_pD3D->EnumAdapterModes(m_Adapter, i, &mode);
		i=i;
	}
}

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





