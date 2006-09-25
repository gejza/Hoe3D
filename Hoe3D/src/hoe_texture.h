
#ifndef _HOE_TEXTURE_H_
#define _HOE_TEXTURE_H_

#include "hoe_format.h"
#include "ref.h"

class HoeLog;

class HoeTexture 
{
public:
	struct LOCKRECT
	{
		byte * data;
		uint pitch;
	};
protected:
	SysTexture m_texture;
	char * name;
	uint width;
	uint height;
	HOEFORMAT format;
#ifdef _HOE_OPENGL_
	byte * pData;
#endif
public:
	HoeTexture();
	~HoeTexture();
	/** Create texture */
	bool Create(uint w,uint h,HOEFORMAT f);
	bool Create(uint w,uint h,HOEFORMAT f, SysTexture tex);
	bool BindData(byte *);

	bool Lock(LOCKRECT *);
	void Unlock();

	void Set();

	bool Load(const char * name, HoeLog * log = NULL);
	bool Reload();

	uint GetWidth() const { return width; }
	uint GetHeight() const { return height; }

	SysTexture GetTexture() { return m_texture; }

	void FillEmpty();

	friend class TextureSystem;
};

class HoeRenderTexture : public HoeTexture
{
public:
	bool Create(uint w,uint h,HOEFORMAT f);
#ifdef _HOE_D3D9_
	IDirect3DSurface9 * GetSurface();
#endif
#ifdef _HOE_D3D8_
	IDirect3DSurface8 * GetSurface();
#endif
};

#endif // _HOE_TEXTURE_H_

