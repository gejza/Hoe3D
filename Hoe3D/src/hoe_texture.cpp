
#include "system.h"
#include <assert.h>
#include "utils.h"
#include "ref.h"
#include "hoe_format.h"
#include "hoe_texture.h"

HoeTexture::HoeTexture()
{
	this->m_texture = 0;
}

bool HoeTexture::Create(uint w,uint h,HOEFORMAT f)
{
	width = w;
	height = h;
	format = f;
#ifdef _HOE_D3D_

	HRESULT hRes;
	if (FAILED(hRes = D3DDevice()->CreateTexture(w,h,1,0,HoeFormatX(f),D3DPOOL_MANAGED,&m_texture RESERVE_PAR)))
	{
		Con_PrintHRes("Error on create texture",hRes);
		return false;
	}
#endif
#ifdef _HOE_OPENGL_
	glGenTextures(1,&m_texture);
#endif
	return true;
}

bool HoeTexture::BindData(byte * pix)
{
#ifdef _HOE_D3D_
	/** todo */
	assert(1);
	LOCKRECT lr;
	if (!Lock(&lr))
		return false;

	Unlock();
#endif
#ifdef _HOE_OPENGL_
	glBindTexture(GL_TEXTURE_2D, m_texture);
	
	switch (format)
	{
	case HOE_A8L8: 
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_LUMINANCE_ALPHA, GL_UNSIGNED_BYTE, pix);
		break;
	default:
        glTexImage2D(GL_TEXTURE_2D, 0, HoeFormatX(format), width, height, 0, (HoeFormatSizeAlpha(format) > 0) ? GL_RGBA:GL_RGB, GL_UNSIGNED_BYTE, pix);// Vlastní vytváøení textury
		break;
	}
	glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_LINEAR);// Filtrování pøi zmenšení
	glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,GL_LINEAR);// Filtrování pøi zvìtšení
#endif
	return true;
}

bool HoeTexture::Lock(LOCKRECT * lr)
{
#ifdef _HOE_D3D_
	D3DLOCKED_RECT rect;
	if (FAILED(m_texture->LockRect(0,&rect,NULL,0)))
	{
		return false;
	}
	lr->data = (byte*)rect.pBits;
	lr->pitch = rect.Pitch;
#endif
#ifdef _HOE_OPENGL_
	lr->pitch = width * HoeFormatSize(format) / 8;
	lr->data = pData = new byte[lr->pitch * height];
#endif
	return true;
}

void HoeTexture::Unlock()
{
#ifdef _HOE_D3D_
	m_texture->UnlockRect(0);
#endif
#ifdef _HOE_OPENGL_
	BindData(pData);
	delete [] pData;
#endif

}

void HoeTexture::Set()
{
#ifdef _HOE_D3D_
	HRESULT hRes;
	hRes = D3DDevice()->SetTexture(0,this->m_texture);

	if (FAILED(hRes))
		exit(0);
#endif // _HOE_D3D_
#ifdef _HOE_OPENGL_
	glBindTexture(GL_TEXTURE_2D,this->m_texture);
#endif // _HOE_OPENGL_
}

bool HoeTexture::Load(const char * name, HoeLog * log)
{
	return true;
}
