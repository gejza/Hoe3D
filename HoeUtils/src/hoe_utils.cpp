
#include "StdAfx.h"
#include "../include/HoeUtils/hoe_utils.h"

namespace HoeUtils {

#ifdef _WIN32
FindFile::FindFile(const HoeCore::CString mask)
{
	m_handle = _findfirst( mask, &m_fileinfo );
	m_dir = GetFileDir(mask);
}

FindFile::~FindFile()
{
	if (IsValid())
		_findclose(m_handle);
}

bool FindFile::IsValid()
{
	return m_handle != -1L;
}

void FindFile::Next()
{
	if (IsValid() && _findnext( m_handle, &m_fileinfo ) != 0)
	{
		_findclose(m_handle);
		m_handle = -1;
	}
}

const HoeCore::String FindFile::GetName()
{
	HoeCore::String str;
	str = m_dir;
	str.concat(m_fileinfo.name);
	return str;
}
#else
FindFile::FindFile(const HoeCore::CString mask)
{
}

FindFile::~FindFile()
{
}

bool FindFile::IsValid()
{
    return false;
}

void FindFile::Next()
{
}
#endif


FindFile::operator bool ()
{
	return IsValid();
}

FindFile& FindFile::operator ++ ()
{
	Next();
	return *this;
}

/////////////////////////////////////////////
const HoeCore::String GetFileName(const HoeCore::CString path, bool withex)
{
	int rl = path.FindLast('/');
	HoeCore::String str;
#ifdef _WIN32
	int wrl = path.FindLast('\\');
	if (wrl > rl)
		rl = wrl;
#endif
	if (rl < 0)
		str = path;
	else
		str = path.Sub(rl+1);
	if (!withex)
	{
		rl = str.FindLast('.');
		if (rl >= 0)
			str[rl] = T('\0');
	}
	return str;
}
/////////////////////////////////////////////
const HoeCore::String GetFileDir(const HoeCore::CString path)
{
	int rl = path.FindLast('/');
	HoeCore::String str;
#ifdef _WIN32
	int wrl = path.FindLast('\\');
	if (wrl > rl)
		rl = wrl;
#endif
	if (rl < 0)
		str = T("./");
	else
		str = path.Sub(0, rl+1);
	return str;
}

} // namespace HoeUtils

