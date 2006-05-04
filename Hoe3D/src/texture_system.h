
#ifndef _HOE_TEXTURE_SYSTEM_
#define _HOE_TEXTURE_SYSTEM_

struct hft_texture;
class HoeTexture;

class TextureSystem
{
	class HoeLog * m_log;
public:
	typedef class HoeTexture HoeTexture;

	TextureSystem();
	~TextureSystem();
	HoeTexture * GetTexture(const char * name);
	void SetTexture(int,HoeTexture * t);
/*
bool Init();

HoeTexture * GetTexture(const char * name);
void SetTexture(HoeTexture * l);
void DeleteAll();
void PrintList();

Texture CreateTexture(int w,int h,HOEFORMAT f,int l,DWORD flags);
BYTE * LockTexture(Texture tex,int lev,const RECT *,int * pitch,DWORD flags);
void UnlockTexture(Texture tex);

Texture CreateTexture(int w,int h,HOEFORMAT f,void * data);

void SetTexture(Texture t);
*/
}; 

#endif //_HOE_TEXTURE_SYSTEM_
