
#ifndef _TEXTURE_LOADER_H_
#define _TEXTURE_LOADER_H_

#include "../include/hfmt/htex_file.h"
#include "../include/hoefs.h"

class TextureLoader
{
	HoeLog * m_log;

	HoeFileReader m_reader;
	hft_texture m_head;
public:
	TextureLoader(HoeLog * log = NULL);
	~TextureLoader();
    bool Load(const char * name);
	hft_texture & GetHeader() { return m_head; }
	bool GetData(void * buff,dword * size);
	bool GetData(void * buff,dword size);
};

#endif // _TEXTURE_LOADER_H_
