
#include "StdAfx.h"
#include "../include/hoe_utils.h"

namespace HoeUtils {

FindFile::FindFile(const HoeCore::CString mask)
{
	m_handle = _findfirst( mask, &m_fileinfo );
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

FindFile::operator bool ()
{
	return IsValid();
}

FindFile& FindFile::operator ++ ()
{
	Next();
	return *this;
}

} // namespace HoeUtils

