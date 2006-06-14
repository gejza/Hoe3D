
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
	hRes = D3DDevice()->CreateTexture(w,h,1,0,HoeFormatX(f),D3DPOOL_MANAGED,&m_texture RESERVE_PAR);
	checkres(hRes,"CreateTexture");
#endif
#ifdef _HOE_OPENGL_
	glGenTextures(1,&m_texture);
	checkgl("glGenTextures");
#endif
	return true;
}

bool HoeTexture::Create(uint w,uint h,HOEFORMAT f, SysTexture tex)
{
	width = w;
	height = h;
	format = f;
	m_texture = tex;
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
	checkgl("glBindTexture");
	switch (format)
	{
	case HOE_L8A8: 
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_LUMINANCE_ALPHA, GL_UNSIGNED_BYTE, pix);
		checkgl("glTexImage2D");
		break;
	default:
		glTexImage2D(GL_TEXTURE_2D, 0, HoeFormatX(format), width, height, 0, (HoeFormatSizeAlpha(format) > 0) ? GL_RGBA:GL_RGB, GL_UNSIGNED_BYTE, pix);// Vlastní vytváøení textury
		checkgl("glTexImage2D");
		break;
	}
	glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_LINEAR);// Filtrování pøi zmenšení
	glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,GL_LINEAR);// Filtrování pøi zvìtšení
	checkgl("glTexParameteri");
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

void HoeTexture::FillEmpty()
{
	LOCKRECT l;
	if (this->Lock(&l))
	{
		for (uint y=0;y < height;y++)
		{
			memset(l.data+y*l.pitch,0, l.pitch);
		}
		this->Unlock();
	}
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

///////////////////////////////////////////////////
bool HoeRenderTexture::Create(uint w,uint h,HOEFORMAT f)
{
	width = w;
	height = h;
	format = f;
#ifdef _HOE_D3D_
	HRESULT hRes;
	hRes = D3DDevice()->CreateTexture(w,h,1,D3DUSAGE_RENDERTARGET,HoeFormatX(f),D3DPOOL_DEFAULT,&m_texture RESERVE_PAR);
	assert(hRes==S_OK);
#endif
#ifdef _HOE_OPENGL_
	glGenTextures(1,&m_texture);
	FillEmpty();
#endif
	return true;
}

#ifdef _HOE_D3D_
#ifdef _HOE_D3D9_
IDirect3DSurface9 * HoeRenderTexture::GetSurface()
{
	IDirect3DSurface9 * sur = NULL;
#else
IDirect3DSurface8 * HoeRenderTexture::GetSurface()
{
	IDirect3DSurface8 * sur = NULL;
#endif
	assert(m_texture);
	HRESULT hRef = m_texture->GetSurfaceLevel(0,&sur);
	assert(hRef==S_OK);
	return sur;
}
#endif
