
#include "StdAfx.h"
#include "utils.h"
#include "filesystem.h"
#include "hoe_format.h"
#include "../include/hfmt/htex_file.h"
#include "shared.h"
#include "texture_loader.h"

TextureLoader::TextureLoader(HoeLog * log)
{
	m_log = log;
}

TextureLoader::~TextureLoader()
{
}

bool TextureLoader::Load(const tchar * name)
{
	m_reader = GetFileSystem()->FindResource_(name,IDTEXHEADER);
	if (!m_reader)
	{
		return false;
	}
	if (!m_reader.Read(&m_head,sizeof(hft_texture)))
		return false;

	return true;
}

bool TextureLoader::GetData(void * buff,dword * size)
{
	if ((*size + m_reader.GetReadSize()) > (sizeof(hft_texture) + (uint)m_head.size))
	{
		*size = (sizeof(hft_texture) + (uint)m_head.size) - m_reader.GetReadSize();
	}
	return m_reader.Read(buff,*size)==*size;
}

bool TextureLoader::GetData(void * buff,dword size)
{
	dword s = size;
	return (GetData(buff, &s) && (s == size));
}

