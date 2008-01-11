
#include "StdAfx.h"
#include "error.h"

using namespace HoeCore;

Error::Error(void)
{
}

Error::~Error(void)
{
}

UnknownError::UnknownError(const CString name, const CString type)
{
	m_str.printf("Unknown %s `%s'.", (const tchar*)type, (const tchar*) name);
}

ConvertError::ConvertError(const HoeCore::CString name, 
				 const HoeCore::CString from,
				 const HoeCore::CString to)
{
	m_str.printf("Cannot convert attribute `%s' from type `%s' to `%s'.", 
				(const tchar*)name,(const tchar*)from,(const tchar*)to); 
}

