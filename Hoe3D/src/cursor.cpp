
#include "system.h"
#include "shared.h"
#include "utils.h"
#include "ref.h"
#include "../include/interfaces.h"
#include "states.h"
#include "texture_system.h"
#include "cursor.h"
#include "hoe_input.h"
#include "camera.h"

#include "hoe_format.h"
#include "hoe_texture.h"
#include "hoe_font.h"

using namespace HoeInput;
#if 0
#ifdef _HOE_D3D9_
//-----------------------------------------------------------------------------
// Name: D3DUtil_SetDeviceCursor
// Desc: Gives the D3D device a cursor with image and hotspot from hCursor.
//-----------------------------------------------------------------------------
HRESULT D3DUtil_SetDeviceCursor( LPDIRECT3DDEVICE9 pd3dDevice, HCURSOR hCursor,
                                 BOOL bAddWatermark )
{
    HRESULT hr = E_FAIL;
    ICONINFO iconinfo;
    BOOL bBWCursor;
    LPDIRECT3DSURFACE9 pCursorSurface = NULL;
    HDC hdcColor = NULL;
    HDC hdcMask = NULL;
    HDC hdcScreen = NULL;
    BITMAP bm;
    DWORD dwWidth;
    DWORD dwHeightSrc;
    DWORD dwHeightDest;
    COLORREF crColor;
    COLORREF crMask;
    UINT x;
    UINT y;
    BITMAPINFO bmi;
    COLORREF* pcrArrayColor = NULL;
    COLORREF* pcrArrayMask = NULL;
    DWORD* pBitmap;
    HGDIOBJ hgdiobjOld;

    ZeroMemory( &iconinfo, sizeof(iconinfo) );
    if( !GetIconInfo( hCursor, &iconinfo ) )
        goto End;

    if (0 == GetObject((HGDIOBJ)iconinfo.hbmMask, sizeof(BITMAP), (LPVOID)&bm))
        goto End;
    dwWidth = bm.bmWidth;
    dwHeightSrc = bm.bmHeight;

    if( iconinfo.hbmColor == NULL )
    {
        bBWCursor = TRUE;
        dwHeightDest = dwHeightSrc / 2;
    }
    else 
    {
        bBWCursor = FALSE;
        dwHeightDest = dwHeightSrc;
    }

    // Create a surface for the fullscreen cursor
    if( FAILED( hr = pd3dDevice->CreateOffscreenPlainSurface( dwWidth, dwHeightDest, 
        D3DFMT_A8R8G8B8, D3DPOOL_SCRATCH, &pCursorSurface, NULL ) ) )
    {
        goto End;
    }

    pcrArrayMask = new DWORD[dwWidth * dwHeightSrc];

    ZeroMemory(&bmi, sizeof(bmi));
    bmi.bmiHeader.biSize = sizeof(bmi.bmiHeader);
    bmi.bmiHeader.biWidth = dwWidth;
    bmi.bmiHeader.biHeight = dwHeightSrc;
    bmi.bmiHeader.biPlanes = 1;
    bmi.bmiHeader.biBitCount = 32;
    bmi.bmiHeader.biCompression = BI_RGB;

    hdcScreen = GetDC( NULL );
    hdcMask = CreateCompatibleDC( hdcScreen );
    if( hdcMask == NULL )
    {
        hr = E_FAIL;
        goto End;
    }
    hgdiobjOld = SelectObject(hdcMask, iconinfo.hbmMask);
    GetDIBits(hdcMask, iconinfo.hbmMask, 0, dwHeightSrc, 
        pcrArrayMask, &bmi, DIB_RGB_COLORS);
    SelectObject(hdcMask, hgdiobjOld);

    if (!bBWCursor)
    {
        pcrArrayColor = new DWORD[dwWidth * dwHeightDest];
        hdcColor = CreateCompatibleDC( hdcScreen );
        if( hdcColor == NULL )
        {
            hr = E_FAIL;
            goto End;
        }
        SelectObject(hdcColor, iconinfo.hbmColor);
        GetDIBits(hdcColor, iconinfo.hbmColor, 0, dwHeightDest, 
            pcrArrayColor, &bmi, DIB_RGB_COLORS);
    }

    // Transfer cursor image into the surface
    D3DLOCKED_RECT lr;
    pCursorSurface->LockRect( &lr, NULL, 0 );
    pBitmap = (DWORD*)lr.pBits;
    for( y = 0; y < dwHeightDest; y++ )
    {
        for( x = 0; x < dwWidth; x++ )
        {
            if (bBWCursor)
            {
                crColor = pcrArrayMask[dwWidth*(dwHeightDest-1-y) + x];
                crMask = pcrArrayMask[dwWidth*(dwHeightSrc-1-y) + x];
            }
            else
            {
                crColor = pcrArrayColor[dwWidth*(dwHeightDest-1-y) + x];
                crMask = pcrArrayMask[dwWidth*(dwHeightDest-1-y) + x];
            }
            if (crMask == 0)
                pBitmap[dwWidth*y + x] = 0xff000000 | crColor;
            else
                pBitmap[dwWidth*y + x] = 0x00000000;

            // It may be helpful to make the D3D cursor look slightly 
            // different from the Windows cursor so you can distinguish 
            // between the two when developing/testing code.  When
            // bAddWatermark is TRUE, the following code adds some
            // small grey "D3D" characters to the upper-left corner of
            // the D3D cursor image.
            if( bAddWatermark && x < 12 && y < 5 )
            {
                // 11.. 11.. 11.. .... CCC0
                // 1.1. ..1. 1.1. .... A2A0
                // 1.1. .1.. 1.1. .... A4A0
                // 1.1. ..1. 1.1. .... A2A0
                // 11.. 11.. 11.. .... CCC0

                const WORD wMask[5] = { 0xccc0, 0xa2a0, 0xa4a0, 0xa2a0, 0xccc0 };
                if( wMask[y] & (1 << (15 - x)) )
                {
                    pBitmap[dwWidth*y + x] |= 0xff808080;
                }
            }
        }
    }
    pCursorSurface->UnlockRect();

    // Set the device cursor
    if( FAILED( hr = pd3dDevice->SetCursorProperties( iconinfo.xHotspot, 
        iconinfo.yHotspot, pCursorSurface ) ) )
    {
        goto End;
    }

    hr = S_OK;

End:
    if( iconinfo.hbmMask != NULL )
        DeleteObject( iconinfo.hbmMask );
    if( iconinfo.hbmColor != NULL )
        DeleteObject( iconinfo.hbmColor );
    if( hdcScreen != NULL )
        ReleaseDC( NULL, hdcScreen );
    if( hdcColor != NULL )
        DeleteDC( hdcColor );
    if( hdcMask != NULL )
        DeleteDC( hdcMask );
    SAFE_DELETE_ARRAY( pcrArrayColor );
    SAFE_DELETE_ARRAY( pcrArrayMask );
    SAFE_RELEASE( pCursorSurface );
    return hr;
}
#endif

HoeCursor * HoeCursor::m_act = NULL;

HoeCursor::HoeCursor()
{
	m_show = false;
	m_msgcb = NULL;
}

void HoeCursor::Show()
{
	m_act = this;
	m_show = true;
}

void HoeCursor::SetPos(int _x,int _y)
{
	this->x = _x;
	this->y = _y;
}

void HoeCursor::GetVirtualPos(float *_x, float *_y)
{
	int w,h;
	HoeCamera::sGetSize(&w,&h);
	*_x = (float)this->x / (float)w;
	*_y = (float)this->y / (float)h;
}

struct CURSOR_VERTEX
{
	float x, y, z, rhw; 
	float       tu, tv;   // The texture coordinates
};

#define CURSOR_VERTEX_FVF (D3DFVF_XYZRHW|D3DFVF_TEX1)

#define CUR_SIZE 32

void HoeCursor::Draw()
{
	if (m_act && m_act->m_show) 
	{
		GetHoeStates()->SetupCursor();
		GetTextureSystem()->SetTexture(0,m_act->res);

		const float cx = (float)m_act->x - m_act->XHotSpot;
		const float cy = (float)m_act->y - m_act->YHotSpot;
#ifdef _HOE_OPENGL_
		glColor4f(1.0f,1.0f,1.0f,1.0f);// Plný jas, 50% alfa
		glBegin(GL_QUADS);// Zaèátek kreslení obdélníkù
		glTexCoord2f( 0.f , 0.f );glVertex2f( cx , cy );// Levý horní bod
		glTexCoord2f( 1.f, 0.f);glVertex2f( cx + CUR_SIZE , cy);// Pravý horní bod
		glTexCoord2f( 1.f , 1.f);glVertex2f( cx + CUR_SIZE, cy + CUR_SIZE );// Pravý dolní bod
		glTexCoord2f( 0.f, 1.f);glVertex2f( cx, cy + CUR_SIZE);// Levý dolní bod
		glEnd();// Konec kreslení obdélníkù
#endif // _HOE_OPENGL_
#ifdef _HOE_D3D9_
		//D3DDevice()->SetTextureStageState( 0, D3DTSS_COLOROP,   D3DTOP_MODULATE );
	//   D3DDevice()->SetTextureStageState( 0, D3DTSS_COLORARG1, D3DTA_TEXTURE );
	//   D3DDevice()->SetTextureStageState( 0, D3DTSS_COLORARG2, D3DTA_DIFFUSE );
	//   //D3DDevice()->SetTextureStageState( 0, D3DTSS_ALPHAOP,   D3DTOP_DISABLE );
		CURSOR_VERTEX pv[] = 
		{
			{ cx , cy , 0.5f, 1.0f, 0.f , 0.f}, 
			{ cx + CUR_SIZE , cy , 0.5f, 1.0f, 1.f, 0.f}, 
			{ cx + CUR_SIZE, cy + CUR_SIZE , 0.5f, 1.0f, 1.f , 1.f},
			{ cx, cy + CUR_SIZE , 0.5f, 1.0f, 0.f, 1.f}
		};
		D3DDevice()->SetFVF(CURSOR_VERTEX_FVF);
		D3DDevice()->DrawPrimitiveUP(D3DPT_TRIANGLEFAN,2,pv,sizeof(CURSOR_VERTEX));
#endif // _HOE_D3D9_
	}
}

bool HoeCursor::LoadCursor(int px,int py,const char * lpCursorName)
{
	res = GetTextureSystem()->GetTexture(lpCursorName);
	XHotSpot = px;
	YHotSpot = py;
	return true;
}

void HoeCursor::Hide()
{
	m_show = false;
}

bool HoeCursor::AcquireInput()
{
	HoeInput::InputMouseBase * ms = (HoeInput::InputMouseBase *)GetInput()->CreateDevice(HoeInput::Mouse);
	ms->AcquireDevice();
	ms->SetAxis(0,AxisFunc);
	ms->SetAxis(1,AxisFunc);
	ms->SetAxis(2,AxisFunc);
	ms->SetButton(0,ButtonFunc);
	ms->SetButton(1,ButtonFunc);
	ms->SetButton(2,ButtonFunc);
	return true;
}

void HoeCursor::UnacquireDevice()
{
}

void HoeCursor::SetCallback(HoeInput::MsgCallback cb)
{
	m_msgcb = cb;
}

void HoeCursor::AxisFunc(const int axis, const float time)
{
	if (m_act == NULL)
		return;
	switch (axis)
	{
	case 0:
		m_act->x += (int)time;
		break;
	case 1:
		m_act->y += (int)time;
		break;
	case 2:
		if (m_act->m_msgcb != NULL) 
		{
			float xx,yy;
			m_act->GetVirtualPos(&xx,&yy);
			m_act->m_msgcb(MWheel,xx,yy,(int)time);
		}
		break;
	};
}

void HoeCursor::ButtonFunc(const int butt, const int phase)
{
	if (m_act == NULL || m_act->m_msgcb == NULL)
		return;

	float xx,yy;
	m_act->GetVirtualPos(&xx,&yy);

	switch (butt)
	{
	case 0:
		if (phase == 3) m_act->m_msgcb(LClick,xx,yy,0);
		break;
	case 1:
		if (phase == 3) m_act->m_msgcb(RClick,xx,yy,0);
		break;
	};	
}

#endif

