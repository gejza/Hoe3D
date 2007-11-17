
#include "StdAfx.h"
#include "shared.h"
#include "utils.h"
#include "config.h"
#include "hoe_format.h"
#include "../include/hfmt/htex_file.h"
#include "texture_loader.h"
#include "texture_converter.h"
#include "texture_hx.h"

TextureConverter::TextureConverter(TextureLoader * l,HoeLog * log)
{
	m_loader = l;
	m_log = log;
}

TextureConverter::~TextureConverter()
{
			/*dword size = loader.GetHeader().size + 10;
			byte * buff = new byte[size];
			loader.GetData(buff,&size);
#ifdef _HOE_OPENGL_
			if (loader.GetHeader().format == HOE_R8G8B8)
				TextureConverter::MixRGB(buff,size);
			if (loader.GetHeader().format == HOE_X8R8G8B8)
				TextureConverter::Convert(buff,size,loader.GetHeader().format,buff,loader.GetHeader().width*loader.GetHeader().height * 3,HOE_R8G8B8);
			glGenTextures(1,&tex->tex);
			glBindTexture(GL_TEXTURE_2D, tex->tex);// Typické vytváøení textury z bitmapy
			glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB8, loader.GetHeader().width, loader.GetHeader().height, 0, GL_RGB, GL_UNSIGNED_BYTE, buff);// Vlastní vytváøení textury
			glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_LINEAR);// Filtrování pøi zmenšení
			glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,GL_LINEAR);// Filtrování pøi zvìtšení
#endif // _HOE_OPENGL_
#ifdef _HOE_D3D9_
			if (FAILED(m_ref->Device()->CreateTexture(loader.GetHeader().width,loader.GetHeader().height,1,0,
				D3DFMT_X8R8G8B8,D3DPOOL_MANAGED,&tex->tex,NULL)))
				exit(0);
			D3DLOCKED_RECT Rect;
			tex->tex->LockRECT(0,&Rect,NULL,0);
			if (loader.GetHeader().format == HOE_X8R8G8B8)
				memcpy(Rect.pBits,buff,size);
			if (loader.GetHeader().format == HOE_B8G8R8)
				TextureConverter::Convert(buff,size,loader.GetHeader().format,(byte*)Rect.pBits,loader.GetHeader().width*loader.GetHeader().height * 4,HOE_X8R8G8B8);  
			tex->tex->UnlockRECT(0);
#endif // _HOE_D3D9_
		}

		delete [] buff;*/
}

byte * TextureConverter::Get(dword pitch)
{
	udata = new byte[pitch * this->GetHeight()];
	if (!Get(udata,pitch))
	{
		return NULL;
	}
	return udata;
}

void TextureConverter::MixRGB(void * buff,dword size)
{
	byte t;
	byte * b = reinterpret_cast<byte*>(buff);

	for (dword i = 0;i < size;i+=3)
	{
		t = b[i];
		b[i] = b[i+2];
		b[i+2] = t;
	}
}

/*void TextureConverter::Convert(byte * p1,dword s1,HOEFORMAT f1,byte * p2,dword s2,HOEFORMAT f2)
{
	dword fs1 = HoeFormatSize(f1) / 8;
	dword fs2 = HoeFormatSize(f2) / 8;
	s1 = s1 / fs1;
	s2 = s2 / fs2;
	dword s = (s1 < s2) ? s1:s2;

	parse_format_func get = HoeFormatGetFunc(f1,false);
	parse_format_func set = HoeFormatGetFunc(f2,true);

	HOECOLOR c;

	for (dword i=0;i < s;i++)
	{
		get(p1,&c);
		set(p2,&c);

		p1 += fs1;
		p2 += fs2;
	}
}*/


//////////////////////////////////////////

TextureConverterHX::TextureConverterHX(TextureLoader * l,HoeLog * log) : TextureConverter(l,log)
{
	udata = NULL;
}

TextureConverterHX::~TextureConverterHX()
{
	if (udata)
		delete [] udata;
}

bool TextureConverterHX::Run()
{
	m_width = m_loader->GetHeader().width;
	m_height = m_loader->GetHeader().height;
	m_format = m_loader->GetHeader().format;
	GetConfig()->CheckTexture(m_width,m_height,m_format);
	return true;
}

void TextureConverterHX::Destroy()
{
	delete this;
}

bool TextureConverterHX::Get(byte * p,dword pitch)
{
	dword size = m_loader->GetHeader().size;
#ifdef _DFMT_D3D9_
	m_loader->GetData(p,&size);
	if (size != (dword)m_loader->GetHeader().size)
		return false;
#endif
#ifdef _DFMT_OPENGL_
	byte a,r,g,b;
	byte * src = new byte[m_loader->GetHeader().pitch];
	for (uint y=0;y < m_height;y++)
	{
		byte * pd = p + (pitch * y);
		byte * ps = src;
		size = m_loader->GetHeader().pitch;
		m_loader->GetData(src,&size);
		for (uint x=0;x < m_width;x++)
		{
			b = *ps++;
			g = *ps++;
			r = *ps++;
			if (m_loader->GetHeader().format == HOE_A8R8G8B8)
				a = *ps++;
			*pd++ = r;
			*pd++ = g;
			*pd++ = b;
			if (m_format == HOE_A8R8G8B8)
				*pd++ = a;
		}
	}
	delete [] src;
#endif
	return true;
}






