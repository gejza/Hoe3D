
#ifndef _HOE_TEXTURE_H_
#define _HOE_TEXTURE_H_

class HoeTexture 
{
public:
	SysTexture m_texture;
public:
	struct LOCKRECT
	{
		byte * data;
		uint pitch;
	};
	uint width;
	uint height;
	HOEFORMAT format;

protected:
#ifdef _HOE_OPENGL_
	byte * pData;
#endif
public:
	char * name;

public:
	HoeTexture();
	/** Create texture */
	bool Create(uint w,uint h,HOEFORMAT f);
	bool BindData(byte *);

	bool Lock(LOCKRECT *);
	void Unlock();

	void Set();

	bool Load(const char * name, HoeLog * log = NULL);
	bool Reload();

	uint GetWidth() { return width; }
	uint GetHeight() { return height; }
};

#endif // _HOE_TEXTURE_H_

